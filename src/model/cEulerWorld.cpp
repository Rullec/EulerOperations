#include "cEulerWorld.hpp"
#include <cstring>

cEulerWorld::cEulerWorld()
{
    mVertexList.clear();
    mFaceList.clear();
    mHalfEdgeList.clear();
    mEdgeList.clear();
    mLoopList.clear();
    mSolidList.clear();
}

void cEulerWorld::Construct()
{
    std::cout <<"[log] cEulerWorld construct begin" << std::endl;
    cSolid * Mainsolid = mvfs(0.0, 0.0, 0.0);

}

cSolid * cEulerWorld::mvfs(double x, double y, double z)
{
    // create new vertex, face and solid...
    cVertex * new_vertex = new cVertex();
    cHalfEdge * new_halfedge = new cHalfEdge();
    cLoop * new_loop = new cLoop();
    cFace * new_face = new cFace();
    cSolid * new_solid = new cSolid();
    
    // set vertex
    new_vertex->mPos = Eigen::Vector3d(x, y, z);
    new_vertex->mOriHalfEdge = new_halfedge;

    // set halfedges
    new_halfedge->mOriVertex = new_vertex;
    new_halfedge->mDestVertex = new_vertex;
    new_halfedge->mAdjacentHF = nullptr;
    new_halfedge->mNextHF = new_halfedge->mPrevHF = nullptr;

    // set loop
    new_loop->mFirstHalfEdge = new_halfedge;

    // set face
    new_face->mFirstLoop = new_loop;

    // set solid
    new_solid->mFirstHalfEdge = new_halfedge;
    mVertexList.push_back(new_vertex);
    mHalfEdgeList.push_back(new_halfedge);
    mLoopList.push_back(new_loop);
    mFaceList.push_back(new_face);
    mSolidList.push_back(new_solid);
    
    return new_solid;
}

// make a new vertex and an edge
void cEulerWorld::mev(cSolid * solid, cFace * face, cVertex * Vertex, double x, double y, double z)
{
    // should check the validity of solid-face and face-Vertex
    cHalfEdge * new_hf = new cHalfEdge();
    cVertex * new_vertex = new cVertex();
    cEdge * new_edge = new cEdge();

    // 这是创建一条新边，而不是一条新的半边...
    // 要变化的有: 
    // 1. Vertex中的出发的半边改变掉了，原先的半边要删除掉。
    // 2. 增加新的两条半边，反向新半边应该指向以前Vertex中半边的目的点
}