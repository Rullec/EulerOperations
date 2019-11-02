#include "cBaseRender.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <cstring>
// -------------------BaseShader begin-----------------------
cBaseShader::cBaseShader(std::string src_path, unsigned int shader_type)
{
    if(GL_VERTEX_SHADER != shader_type && GL_FRAGMENT_SHADER != shader_type)
    {
        std::cout <<"[error] cBase Shader Unsupported shader type: " << shader_type << std::endl;
        exit(1);
    }

    // init member vars
    mSrcPath = src_path;
    mSrc.clear();
    mShaderHandle = 0;
    mShaderType = shader_type;

    // read src from file
    ReadSrcFromFile(mSrcPath);

    // compile the source code 
    CompileSrc();
}

cBaseShader::~cBaseShader()
{
    if(INVALID_HANDLE != mShaderHandle)
    {
        glDeleteShader(mShaderHandle);
    }
}

void cBaseShader::ReadSrcFromFile(std::string filename)
{
    // load source code
    mSrc.clear();
    std::ifstream shader_fin(filename.c_str());
    if(shader_fin.fail() == true)
    {
        std::cout <<"[error] Load shader file " << filename << " failed." << std::endl;
    }
    while(EOF != shader_fin.peek())
    {
        std::string tmp;
        std::getline(shader_fin, tmp);
        mSrc = mSrc + tmp + "\n";
    }
} 

unsigned int cBaseShader::GetShaderHandle()
{
    // get shader
    if(INVALID_HANDLE == mShaderHandle)
    {
        std::cout <<"[error] shader handle is invalid for " << mSrcPath << std::endl;
        exit(1);
    }
    return mShaderHandle;
}

void cBaseShader::CompileSrc()
{
    char * src_ptr = new char[mSrc.size() + 1];
    std::memcpy(src_ptr, mSrc.c_str(), sizeof(char) * mSrc.size());
    // const char * src_ptr = mSrc.c_str();
    // std::cout << src_ptr << std::endl;
    
    mShaderHandle = glCreateShader(mShaderType);
    
    glShaderSource(mShaderHandle, 1, (const char * const *)(&src_ptr), NULL);
    glCompileShader(mShaderHandle);

    // check the success of compliation
    int success, logsize;
    glGetShaderiv(mShaderHandle, GL_INFO_LOG_LENGTH, &logsize);
    char * infoLog = new char[logsize + 1];
    std::cout << "[log] cBaseShader logsize = " << logsize << std::endl;
    memset(infoLog, 0, sizeof(char) * (logsize + 1));
    glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &success);

    if(GL_FALSE == success)
    {
        glGetShaderInfoLog(mShaderHandle, logsize + 1, NULL, infoLog);
        std::cout << "[error] Failed to compile shader " << mSrcPath << ": \n\t"<< infoLog << std::endl;
        exit(1);
    }
}
// -------------------BaseShader end---------------------