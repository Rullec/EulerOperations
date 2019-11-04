#include "cDrawScene.hpp"
#include "../renders/cBaseRender.hpp"
#include <Eigen/Core>
#include <json/json.h>
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

    // Init Render
    InitRender();
   
    // Init done
    mSceneStatus = SceneStatus::InitSucc;
}

void cDrawScene::Update()
{
    // check status
    std::cout <<"[log] cDrawScene Update" << std::endl;
    if(SceneStatus::InitSucc != mSceneStatus)
    {
        std::cout <<"[warning] Scene status is " <<  SceneStatusStr[mSceneStatus] <<" but not " << 
        SceneStatusStr[SceneStatus::InitSucc] << ", return" << std::endl;
        return ;        
    }
    
    mRender->Update();
}

void cDrawScene::MainLoop()
{
    glutMainLoop();
}

void cDrawScene::ConstructMembers(int argc, char ** argv, std::string conf_path)
{ 
    mConfPath = conf_path;
    mRenderType = RenderType::Base;
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

}

void cDrawScene::InitRender()
{
    if(nullptr != mRender)
    {
        std::cout <<"[error] Render Init repeated" << std::endl;
        exit(1);
    }

    mRender = (std::unique_ptr<cBaseRender>)(new cBaseRender(mConfPath));
    mRender->Init();
    
}