#include "cEulerWorld.hpp"
#include <cstring>

cEulerWorld::cEulerWorld()
{
    mObjManager.clear();
    mObjId = 0;
}

void cEulerWorld::Construct()
{
    std::cout <<"[log] cEulerWorld construct begin" << std::endl;
    cSolid * Mainsolid = mvfs(0.0, 0.0, 0.0);
}

cSolid * cEulerWorld::mvfs(double x, double y, double z)
{
    // create new vertex, face and solid...
    cVertex * new_vertex = new cVertex(mObjId++);
    cSolid * new_solid = new cSolid(mObjId++);
    cFace * new_face = new cFace(mObjId++);

    // set vertex
    new_vertex->mPos = Eigen::Vector3d(x, y, z);

    // set solid
    new_solid->mFirstFace = new_face;
    new_solid->mFirstVertex = new_vertex;

    // set face, create loop for setting face
    cLoop * new_loop = new cLoop(mObjId++);
    new_face->mSolid = new_solid;
    new_face->mFirstLoop = new_loop;

    // set loop
    new_loop->mFace = new_face;

    // add objs to ObjManager
    mObjManager.push_back(new_vertex);
    mObjManager.push_back(new_solid);
    mObjManager.push_back(new_face);
    mObjManager.push_back(new_loop);

    return new_solid;
}

// make a new vertex and an edge
void cEulerWorld::mev(cSolid * solid, cFace * face, cVertex * Vertex, double x, double y, double z)
{
    // should check the validity of solid-face and face-Vertex
    cHalfEdge * new_hf = new cHalfEdge(mObjId++);
    cVertex * new_vertex = new cVertex(mObjId++);
    cEdge * new_edge = new cEdge(mObjId++);

    // 这是创建一条新边，而不是一条新的半边...
    // 要变化的有: 
    // 1. Vertex中的出发的半边改变掉了，原先的半边要删除掉。
    // 2. 增加新的两条半边，反向新半边应该指向以前Vertex中半边的目的点
    
}