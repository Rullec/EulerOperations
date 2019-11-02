#include "cDrawScene.hpp"
#include "../renders/cBaseRender.hpp"
#include <Eigen/Core>
#include <json/json.h>
#include "GL/glew.h"
#include "GL/glut.h"

#include <iostream>
#include <fstream>
using namespace std;

cDrawScene::MainWindowInfo::MainWindowInfo(
    int _argc/* = 1*/, 
    char ** _argv/* = nullptr*/, 
    int _start_x/* = 100*/,
    int _start_y/* = 100*/, 
    int _width/* = 600,*/,
    int _height/* = 800*/): argc(_argc), argv(_argv), start_x(_start_x), start_y(_start_y), width(_width), height(_height)
{
    memset(clear_color, 0, 4 * sizeof(double));
}

cDrawScene::cDrawScene(int argc, char ** argv, std::string conf_path)
{
    // init vars 
    ConstructMembers(argc, argv, conf_path);
}

cDrawScene::~cDrawScene()
{
    
}

void cDrawScene::Init()
{
    if(SceneStatus::BeforeInit != mSceneStatus)
    {
        std::cout <<"[warning] Scene status is " <<  SceneStatusStr[mSceneStatus] <<" but not " << 
        SceneStatusStr[SceneStatus::BeforeInit] << ", return" << std::endl;
        return ;        
    }

    // parse conf json
    ParseConf();

    // InitGL
    InitGL();
   
}

void cDrawScene::Update()
{
    std::cout <<"[log] cDrawScene Update" << std::endl;
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

void cDrawScene::MainLoop()
{
    glutMainLoop();
}

void cDrawScene::ConstructMembers(int argc, char ** argv, std::string conf_path)
{ 
    mConfPath = conf_path;
    mMainWindowInfo = std::unique_ptr<MainWindowInfo>(new MainWindowInfo());
    mMainWindowInfo->argc = argc;
    mMainWindowInfo->argv = argv;
}

void cDrawScene::ParseConf()
{
    std::cout <<"[log] cDrawScene begin to parse conf file: " << mConfPath << std::endl;
    // confirm its readable
    ifstream json_fin(mConfPath);
    if(json_fin.fail() == true)
    {
        std::cout <<"[error] cDrawScene load" << mConfPath << " failed" << std::endl;
        exit(1);
    }

    // parse json
    Json::Reader reader;
    Json::Value root;
    if(false == reader.parse(json_fin, root))
    {
        std::cout <<"[error] cDrawScene load succ but failed to parse json " << mConfPath << std::endl;
        exit(1);
    }

    // MainWindowInfo request
    Json::Value mainwindow_info = root["MainWindowInfo"];
    mMainWindowInfo->width = mainwindow_info["Width"].asDouble();
    mMainWindowInfo->height = mainwindow_info["Height"].asDouble();
    mMainWindowInfo->start_x = mainwindow_info["StartX"].asDouble();
    mMainWindowInfo->start_y = mainwindow_info["StartY"].asDouble();
    for(int i=0; i< 4; i++ ) mMainWindowInfo->clear_color[i] = mainwindow_info["ClearColor"][i].asDouble();
}

void cDrawScene::InitGL()
{
    glutInit(&(mMainWindowInfo->argc), mMainWindowInfo->argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(mMainWindowInfo->width, mMainWindowInfo->height);
    glutCreateWindow("EulerOperationsMainWindow");

    // std::cout << " set clear color " << std::endl;
    glClearColor(mMainWindowInfo->clear_color[0],
                mMainWindowInfo->clear_color[1],
                mMainWindowInfo->clear_color[2],
                mMainWindowInfo->clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    // create shader
    glewInit();

    std::unique_ptr<cBaseShader> vertex_shader = (std::unique_ptr<cBaseShader>)(new cBaseShader("../src/shaders/vertex.glsl", GL_VERTEX_SHADER)),
            fragment_shader = (std::unique_ptr<cBaseShader>)(new cBaseShader("../src/shaders/fragement.glsl", GL_FRAGMENT_SHADER));

    
    // create shader program after shaders
    int success = 1, logsize = 0;
    char * infoLog = nullptr;
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader->GetShaderHandle());
    glAttachShader(shader_program, fragment_shader->GetShaderHandle());
    std::cout <<"vertex shader handle = " << vertex_shader->GetShaderHandle() << std::endl;
    std::cout <<"fragment shader handle = " << fragment_shader->GetShaderHandle() << std::endl;
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &logsize);
    infoLog = new char [logsize + 1];
    memset(infoLog, 0, sizeof(char) * (logsize + 1));
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(GL_FALSE == success) 
    {
        glGetProgramInfoLog(shader_program, logsize + 1, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shader_program);
    
    // delete shaders after linking
    vertex_shader.reset();
    fragment_shader.reset();
}