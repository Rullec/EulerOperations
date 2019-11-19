#include "cEulerWorld.hpp"
#include "cEulerTools.hpp"
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
    Eigen::Vector3d p0, p1, p2, p3, p4, p5, p6, p7;
    p0 = Eigen::Vector3d(0, 0, 0), p4 = Eigen::Vector3d(0, 1, 0);
    p1 = Eigen::Vector3d(1, 0, 0), p5 = Eigen::Vector3d(1, 1, 0);
    p2 = Eigen::Vector3d(1, 0, -1), p6 = Eigen::Vector3d(1, 1, -1);
    p3 = Eigen::Vector3d(0, 0, -1), p7 = Eigen::Vector3d(0, 1, -1);
    // mvfs

    cSolid * Mainsolid = mvfs(p0);
    
    // mev * 3
    {
        mev(Mainsolid->mFirstLoop, p0, p1);
        mev(Mainsolid->mFirstLoop, p1, p2);
        mev(Mainsolid->mFirstLoop, p2, p3);
    }


    // mef 
    {
        cVertex * v1 = FindVertexByPos(Mainsolid, p0),
                * v2 = FindVertexByPos(Mainsolid, p3);
        Show(Mainsolid->mFirstLoop);
        mef(Mainsolid, v1, v2);
    }

    // mev * 4
    {
        mev(Mainsolid->mFirstLoop, p0, p4);
        mev(Mainsolid->mFirstLoop, p1, p5);
        mev(Mainsolid->mFirstLoop, p2, p6);
        mev(Mainsolid->mFirstLoop, p3, p7);
    }

    // mef * 4
    {
        cVertex * cur_v1 = FindVertexByPos(Mainsolid, p4),
                * cur_v2 = FindVertexByPos(Mainsolid, p5),
                * cur_v3 = FindVertexByPos(Mainsolid, p6),
                * cur_v4 = FindVertexByPos(Mainsolid, p7);
        mef(Mainsolid, cur_v1, cur_v2);
        mef(Mainsolid, cur_v2, cur_v3);
        mef(Mainsolid, cur_v3, cur_v4);
        mef(Mainsolid, cur_v4, cur_v1);
    }

    // mev * 1 -> kemr -> mev * 3 -> mef
    {
        
    }

    // mev * 4
    {

    }

    // mef * 4
    {

    }

    // kfmrh 
    {

    }
    
    // finished
}

cSolid * cEulerWorld::mvfs(Eigen::Vector3d pos)
{
    // create new vertex, face and solid...
    cVertex * new_vertex = new cVertex();
    cLoop * new_loop = new cLoop();
    cFace * new_face = new cFace();
    cSolid * new_solid = new cSolid();

    // set vertex
    new_vertex->mPos = pos;
    // new_vertex->mOriHalfEdge = nullptr;

    // set loop
    new_loop->mFirstHalfEdge = nullptr;
    new_loop->mFirstVertex = new_vertex;

    // set face
    new_face->mFirstLoop = new_loop;

    // set solid
    new_solid->mFirstLoop = new_loop;
    mVertexList.push_back(new_vertex);
    mLoopList.push_back(new_loop);
    mFaceList.push_back(new_face);
    mSolidList.push_back(new_solid);

    return new_solid;
}

// make a new vertex and an edge
void cEulerWorld::mev(cLoop * loop, const Eigen::Vector3d & origin_pos, const Eigen::Vector3d & pos)
{
    cVertex * ori_v = FindVertexByPos(loop, origin_pos);
    if(nullptr != ori_v)
    {
        mev(loop, ori_v, pos);
    }
    else
    {
        std::cout <<"[error] mev: the origin vertex is null" << std::endl;
        exit(1);
    }
    
}

void cEulerWorld::mev(cLoop * loop, cVertex * ori_vertex, const Eigen::Vector3d & pos)
{
    // if the loop is null, we can not find hfs
    if(nullptr == loop)
    {
        std::cout <<"[error] mev: input loop is null, can not find the desired halfedge" << std::endl;
        exit(1);
    }
    if(nullptr == ori_vertex)
    {
        std::cout <<"[error] mev: input ori_vertex in null" << std::endl;
        exit(1);
    }

    // 创建顶点
    cVertex * new_vertex = new cVertex();
    mVertexList.push_back(new_vertex);
    new_vertex->mPos = pos;    

    // create 2 halfedges
    cHalfEdge * new_halfedge1 = new cHalfEdge();
    cHalfEdge * new_halfedge2 = new cHalfEdge();
    cEdge * new_edge = new cEdge();
    
    // storaged
    mHalfEdgeList.push_back(new_halfedge1);
    mHalfEdgeList.push_back(new_halfedge2);
    mEdgeList.push_back(new_edge);

    // set edge
    new_edge->mHalfEdge1 = new_halfedge1;
    new_edge->mHalfEdge2 = new_halfedge2;

    // set half edges
    new_halfedge1->mOriVertex = ori_vertex;
    new_halfedge1->mDestVertex = new_vertex;
    new_halfedge2->mOriVertex = new_vertex;
    new_halfedge2->mDestVertex = ori_vertex;
    new_halfedge1->mAdjacentHF = new_halfedge2;
    new_halfedge2->mAdjacentHF = new_halfedge1;
    new_halfedge1->mNextHF = new_halfedge2;
    new_halfedge2->mPrevHF = new_halfedge1;
    
    // arrange the loop and pointers
    if(nullptr == loop->mFirstHalfEdge)
    {
        // if there is only 1 vertex and no halfedges
        new_halfedge1->mNextHF = new_halfedge1->mPrevHF = new_halfedge2;
        new_halfedge2->mNextHF = new_halfedge2->mPrevHF = new_halfedge1;
        loop->mFirstHalfEdge = new_halfedge1;
    }
    else
    {
        // in this loop, find the specified halfedge pointed from this vertex;
        cHalfEdge * hf_from_v = FindHalfEdgeByOriVertex(loop, ori_vertex);
        if(hf_from_v == nullptr)
        {
            std::cout <<"[error] mev: find target halfedge failed in this loop" << std::endl;
            exit(1);
        }
        cHalfEdge * hf_to_v = hf_from_v->mAdjacentHF;
        hf_to_v->mNextHF = new_halfedge1;
        new_halfedge1->mPrevHF = hf_to_v;

        new_halfedge2->mNextHF = hf_from_v;
        hf_from_v->mPrevHF = new_halfedge2;
    }

}

void cEulerWorld::mef(cSolid * solid, Eigen::Vector3d v1_pos, Eigen::Vector3d v2_pos)
{
    cVertex * v1 = FindVertexByPos(solid, v1_pos), * v2 = FindVertexByPos(solid, v2_pos);
    if(nullptr != v1 && nullptr != v2)
    {
        mef(solid, v1, v2);
    }
    else
    {
        std::cout <<"[error] empty vertex in mef" << std::endl;
        exit(1);
    }

}

void cEulerWorld::mef(cSolid * solid, cVertex *v1, cVertex * v2)
{
    if(nullptr == solid || nullptr == v1 || nullptr == v2)
    {
        std::cout <<"[log] mef input empty, failed" << std::endl;
    }
    // this face should include a loop, now try to find the loop
    // mef will make a new edge and a new face, before that there should be a open loop, we must find it first the finish it and create a new loop

    // create new face 
    cFace * new_face = new cFace();
    mFaceList.push_back(new_face);
    if(nullptr == solid->mFirstFace)
    {
        solid->mFirstFace = new_face;
    }
    else
    {
        // insert this face into the end of face pointer chain
        cFace * cur_face = solid->mFirstFace;
        while (nullptr != cur_face->mNextFace)
        {
            cur_face = cur_face->mNextFace;
        }
        cur_face->mNextFace = new_face;
    }
    
    // create new edge, new loop
    cEdge * new_edge = new cEdge();
    cHalfEdge   * new_hf1 = new cHalfEdge(),        // v1 -> v2
                * new_hf2 = new cHalfEdge();        // v2 -> v1
    mEdgeList.push_back(new_edge);
    mHalfEdgeList.push_back(new_hf1);
    mHalfEdgeList.push_back(new_hf2);

    // set new edge
    new_edge->mHalfEdge1 = new_hf1;
    new_edge->mHalfEdge2 = new_hf2;

    // set 2 new halfedges
    new_hf1->mOriVertex = v1, new_hf1->mDestVertex = v2;
    new_hf2->mOriVertex = v2, new_hf2->mDestVertex = v1;
    new_hf1->mAdjacentHF = new_hf2;
    new_hf2->mAdjacentHF = new_hf1;

    // find the loop which includes v1 and v2
    // create a new loop, then set the relationship between halfedge
    cLoop * cur_loop = FindLoopBy2Vertices(solid, v1, v2);
    if(nullptr == cur_loop)
    {
        std::cout <<"[error] mef: no loop includes v1 and v2 " << std::endl;
        exit(1);
    }

    // create a new loop
    cLoop * new_loop = new cLoop();
    mLoopList.push_back(new_loop);
    
    // insert this new loop into the last cur_loop
    cLoop * final_loop = cur_loop;
    while(final_loop->mNextLoop != nullptr) final_loop = final_loop->mNextLoop;
    final_loop->mNextLoop = new_loop;
    
    // didn't finished
    exit(1);
}