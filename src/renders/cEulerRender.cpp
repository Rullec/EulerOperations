#include "cEulerRender.hpp"
#include "../model/cEulerWorld.hpp"
#include <iostream>

cEulerRender::cEulerRender(std::string conf_path):cBaseRender(conf_path)
{
    ConstructMembers();
}

void cEulerRender::Init()
{
    // 基类初始化
    cBaseRender::Init();

}

void cEulerRender::Update()
{
    std::cout <<"[log] cEulerRender Update" << std::endl;
    if(nullptr == mEulerWorld)
    {
        std::cout <<"[log] cEulerRender: ptr to Eulerworld is empty, return" << std::endl;
        return;
    }
    
    std::vector<cBaseObj *> & objs = mEulerWorld->GetObjManager();
    std::cout << objs.size() << std::endl;
    for(int i=0; i< objs.size(); i++)
    {
        cBaseObj * cur = objs[i];

        switch(cur->mObjType)
        {
            case ObjType::Euler_Edge: break;
            case ObjType::Euler_Face: break;
            case ObjType::Euler_HalfEdge: break;
            case ObjType::Euler_Loop: break;
            case ObjType::Euler_Vertex: break;
            case ObjType::Euler_Solid: break;
            default: std::cerr << "[error] cEulerRender Unsupported type"; exit(1); break;
        }
        std::cout << cur->mObjID << " " << cur->mObjType << std::endl;
    }
    
}

void cEulerRender::SetEulerWorld(cEulerWorld * world)
{
    if(nullptr != mEulerWorld || nullptr == world)
    {
        std::cout <<"[error] cEulerRender euler world set repeated && error" << std::endl;
        exit(1);
    }

    mEulerWorld = world;
}

void cEulerRender::ConstructMembers()
{
    mEulerWorld = nullptr;

}

void cEulerRender::DrawFace(cBaseObj *)
{

}

void cEulerRender::DrawEdge(cBaseObj *)
{

}

void cEulerRender::DrawVertex(cBaseObj *)
{

}
    