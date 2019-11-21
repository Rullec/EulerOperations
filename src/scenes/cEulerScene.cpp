#include "cEulerScene.hpp"
#include "../model/cEulerWorld.hpp"
#include "../renders/cEulerRender.hpp"

cEulerScene::cEulerScene(int argc, char ** argv, std::string conf_path):cDrawScene(argc, argv, conf_path)
{
    ConstructMembers();
}

void cEulerScene::Init()
{
    std::cout <<"[log] cEulerScene Init" << std::endl;
    if(SceneStatus::BeforeInit != mSceneStatus)
    {
        std::cout <<"[warning] Scene status is " <<  SceneStatusStr[mSceneStatus] <<" but not " << 
        SceneStatusStr[SceneStatus::BeforeInit] << ", return" << std::endl;
        return ;        
    }

    // construct euler world
    mEulerWorld = (std::unique_ptr<cEulerWorld>)(new cEulerWorld());
    mEulerWorld->Construct();

    cDrawScene::ParseConf();
    cDrawScene::InitGL();
    InitRender();

    mSceneStatus = SceneStatus::InitSucc;
    std::cout <<"[log] cEulerScene Init succ" << std::endl;
}

void cEulerScene::Update()
{
    std::cout <<"[log] cEulerScene Update" << std::endl;
    if(SceneStatus::InitSucc != mSceneStatus)
    {
        std::cout <<"[warning] Scene status is " <<  SceneStatusStr[mSceneStatus] <<" but not " << 
        SceneStatusStr[SceneStatus::InitSucc] << ", return" << std::endl;
        return ;        
    }
    
    mRender->Update();
}

void cEulerScene::KeyBoardEvent(unsigned char key, int x, int y)
{
    cDrawScene::KeyBoardEvent(key, x, y);
    
    cEulerRender * euler_render = dynamic_cast<cEulerRender *>(mRender.get());
    if(nullptr == euler_render)
    {
        std::cout <<"[error] cEulerScene::KeyBoardEvent: dynamic_cast failed" << std::endl;
        exit(1);
    }
    euler_render->KeyBoardEvent(key, x, y);
}

void cEulerScene::MouseMoveEvent(int x, int y)
{
    cDrawScene::MouseMoveEvent(x, y);

    cEulerRender * euler_render = dynamic_cast<cEulerRender *>(mRender.get());
    if(nullptr == euler_render)
    {
        std::cout <<"[error] cEulerScene::MouseMoveEvent: dynamic_cast failed" << std::endl;
        exit(1);
    }
    euler_render->MouseMoveEvent(x, y);
}

void cEulerScene::MainLoop()
{
    cDrawScene::MainLoop();
}

void cEulerScene::ConstructMembers()
{
    mEulerWorld = nullptr;
}

void cEulerScene::InitRender(){
    mRender = (std::unique_ptr<cBaseRender>)(new cEulerRender(mConfPath));
    mRender->Init();

    cEulerRender * tmp_render = dynamic_cast<cEulerRender *>(mRender.get());
    if(tmp_render == nullptr)
    {
        std::cout <<"[error] cEulerScene::InitRender failed" << std::endl;
        exit(1);
    }
    
    tmp_render->SetEulerWorld(mEulerWorld.get());
}