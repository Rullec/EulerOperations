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
    tmp_render->SetEulerWorld(mEulerWorld.get());
}