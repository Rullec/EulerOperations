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
    
    DrawVertex();
    DrawEdge();
    DrawFace();
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

void cEulerRender::DrawFace()
{
    std::vector<cFace *> face_list = mEulerWorld->GetFaceList();
    std::cout <<"[log] draw " << face_list.size() << " faces" << std::endl;
}

void cEulerRender::DrawEdge()
{
    std::vector<cEdge *> edge_list = mEulerWorld->GetEdgeList();
    std::cout <<"[log] draw " << edge_list.size() << " edges" << std::endl;
    std::vector<cHalfEdge *> halfedge_list = mEulerWorld->GetHalfEdgeList();
    std::cout <<"[log] draw " << halfedge_list.size() <<" halfedges" << std::endl;
}

void cEulerRender::DrawVertex()
{
    std::vector<cVertex *> vertex_list = mEulerWorld->GetVertexList();
    std::cout <<"[log] draw " << vertex_list.size() << " vertices " << std::endl; 
}
    