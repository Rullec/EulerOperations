#include "cBaseRender.hpp"
#include "./renderobj/cBaseRenderObj.h"
#include "cBaseShader.hpp"
#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <GL/glew.h>
using namespace std;

// -------------------BaseRender begin-------------------
cBaseRender::cBaseRender(std::string conf_path)
{
    ConstructMembers(conf_path);
}

cBaseRender::~cBaseRender()
{

}

void cBaseRender::Init()
{
    // parse conf
    ParseConf();

    // create shader
    glewInit();
    InitShader();
}

void cBaseRender::Update()
{
    std::cout <<"[log] cBaseRender Update" << std::endl;
    glUseProgram(mShaderProgram);
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 创建VBO并且写入数据
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  // 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void cBaseRender::ConstructMembers(std::string conf_path)
{
    mConfPath = conf_path, mVertexShaderPath = "", mFragmentShaderPath = "";
    mShaderProgram = 0;
}

void cBaseRender::ParseConf()
{
    std::cout <<"[log] cBaseRender ParseConf: " << mConfPath << std::endl;

    // init shader
    Json::Reader reader;
    Json::Value root;
    
    // confirm its readable
    ifstream json_fin(mConfPath);
    if(json_fin.fail() == true)
    {
        std::cout <<"[error] cBaseRender load" << mConfPath << " failed" << std::endl;
        exit(1);
    }
    reader.parse(json_fin, root);

    Json::Value json_render = root["Render"];
    if(true == json_render.isNull())
    {
        std::cout <<"[error] There is no shader pathes in json!" << std::endl;
        exit(1);
    }

    // read value
    mVertexShaderPath = json_render["vertex_shader_path"].asString();
    mFragmentShaderPath = json_render["fragment_shader_path"].asString();
    // std::cout <<"vertex shader path = "<< mVertexShaderPath << std::endl;
    // std::cout <<"fragment shader path = "<< mFragmentShaderPath << std::endl;
}

void cBaseRender::InitShader()
{

    std::unique_ptr<cBaseShader> vertex_shader = (std::unique_ptr<cBaseShader>)(new cBaseShader(mVertexShaderPath, GL_VERTEX_SHADER)),
            fragment_shader = (std::unique_ptr<cBaseShader>)(new cBaseShader(mFragmentShaderPath, GL_FRAGMENT_SHADER));

    // create shader program after shaders
    int success = 1, logsize = 0;
    char * infoLog = nullptr;
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertex_shader->GetShaderHandle());
    glAttachShader(mShaderProgram, fragment_shader->GetShaderHandle());
    // std::cout <<"[cBaseRender] vertex shader handle = " << vertex_shader->GetShaderHandle() << std::endl;
    // std::cout <<"[cBaseRender] fragment shader handle = " << fragment_shader->GetShaderHandle() << std::endl;
    glLinkProgram(mShaderProgram);
    glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &logsize);
    infoLog = new char [logsize + 1];
    memset(infoLog, 0, sizeof(char) * (logsize + 1));
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
    if(GL_FALSE == success) 
    {
        glGetProgramInfoLog(mShaderProgram, logsize + 1, NULL, infoLog);
        std::cout << "[cBaseRender] Shaders Link Error: \n" << infoLog << std::endl;
    }
    
    // delete shaders after linking
    vertex_shader.reset();
    fragment_shader.reset();
}
// ------------------------------BaseRender end-----------------------