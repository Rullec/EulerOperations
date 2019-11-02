#include "cBaseRender.hpp"
#include "./renderobj/cBaseRenderObj.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <json/json.h>
using namespace std;

// -------------------BaseRender begin-------------------
cBaseRender::cBaseRender(std::string conf_path)
{
    ConstructMembers(conf_path);
}

cBaseRender::~cBaseRender()
{

}

void cBaseRender::ConstructMembers(std::string conf_path)
{
    mConfPath = conf_path;
    mVertexShader = nullptr, mFragmentShader = nullptr;
    mRenderList.clear();
}

void cBaseRender::Init()
{
    std::cout <<"[log] BaseRender begin to parse conf file: " << mConfPath << std::endl;

    // init shader
    Json::Reader reader;
    Json::Value root;
    
    // confirm its readable
    ifstream json_fin(mConfPath);
    if(json_fin.fail() == true)
    {
        std::cout <<"[error] BaseRender load" << mConfPath << " failed" << std::endl;
        exit(1);
    }

    Json::Value json_render = root["Render"];

    // parse json
    // Json::Reader reader;
    // Json::Value root;
    // if(false == reader.parse(json_fin, root))
    // {
    //     std::cout <<"[error] BaseRender load succ but failed to parse json " << mConfPath << std::endl;
    //     exit(1);
    // }

}

// ------------------------------BaseRender end-----------------------