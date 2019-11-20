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
    std::vector<Eigen::Vector3d> v_lst;
    Eigen::Vector3d p0, p1, p2, p3, p4, p5, p6, p7;
    p0 = Eigen::Vector3d(0, 0, 0), p4 = Eigen::Vector3d(0, 1, 0);
    p1 = Eigen::Vector3d(1, 0, 0), p5 = Eigen::Vector3d(1, 1, 0);
    p2 = Eigen::Vector3d(1, 0, -1), p6 = Eigen::Vector3d(1, 1, -1);
    p3 = Eigen::Vector3d(0, 0, -1), p7 = Eigen::Vector3d(0, 1, -1);
    // mvfs

    cSolid * Mainsolid = mvfs(p0);
    
    // mev * 3
    {
        mev(Mainsolid->mFirstFace->mFirstLoop, p0, p1);
        mev(Mainsolid->mFirstFace->mFirstLoop, p1, p2);
        mev(Mainsolid->mFirstFace->mFirstLoop, p2, p3);
        // Show(Mainsolid);
    }

    // mef 
    {
        // cVertex * v1 = FindVertexByPos(Mainsolid, p0),
        //         * v2 = FindVertexByPos(Mainsolid, p3);
        // Show(Mainsolid->mFirstLoop);
        mef(Mainsolid, p0, p3);
        // Show(Mainsolid);
    }

    // mev * 4
    {
        // find the face to be operated
        v_lst.clear();
        v_lst.push_back(p0), v_lst.push_back(p1), v_lst.push_back(p2), v_lst.push_back(p3);
        cLoop * cur_loop = FindLoopByVerticesInSolid(Mainsolid, v_lst);
        mev(cur_loop, p0, p4);
        mev(cur_loop, p1, p5);
        mev(cur_loop, p2, p6);
        mev(cur_loop, p3, p7);
        
    }

    // mef * 4
    {
        mef(Mainsolid, p4, p5);
        mef(Mainsolid, p5, p6);
        mef(Mainsolid, p6, p7);
        mef(Mainsolid, p7, p4);
    }

    Eigen::Vector3d p_middle_bottom[4], p_middle_top[4];
    p_middle_bottom[0] = Eigen::Vector3d(0.25, 0, -0.25);
    p_middle_bottom[1] = Eigen::Vector3d(0.75, 0, -0.25);
    p_middle_bottom[2] = Eigen::Vector3d(0.75, 0, -0.75);
    p_middle_bottom[3] = Eigen::Vector3d(0.25, 0, -0.75);
    for(int i=0; i<4; i++)
    {
        p_middle_top[i] = p_middle_bottom[i];
        p_middle_top[i][1] = 1;
    }
    
    // mev * 1 + kemr -> mev * 3 -> mef
    {
        // 1. mev
        // find loop, find the loop which include the top 4 vertices
        v_lst.clear();
        v_lst.push_back(p4), v_lst.push_back(p5), v_lst.push_back(p6), v_lst.push_back(p7);
        cLoop * cur_loop = FindLoopByVerticesInSolid(Mainsolid, v_lst);
        mev(cur_loop, p4, p_middle_top[0]);
        // Show(Mainsolid);

        // 2. kemr
        kemr(Mainsolid, p4, p_middle_top[0]);

        // 3. mev * 3
        v_lst.clear();
        v_lst.push_back(p_middle_top[0]);
        cur_loop = FindLoopByVerticesInSolid(Mainsolid, v_lst);
        mev(cur_loop, p_middle_top[0], p_middle_top[1]);
        mev(cur_loop, p_middle_top[1], p_middle_top[2]);
        mev(cur_loop, p_middle_top[2], p_middle_top[3]);
        
        // 4. mef
        mef(Mainsolid, p_middle_top[3], p_middle_top[0]);
        Show(cur_loop);
    }

    // mev * 4
    {
        v_lst.clear();
        v_lst.push_back(p_middle_top[0]);
        v_lst.push_back(p_middle_top[1]);
        v_lst.push_back(p_middle_top[2]);
        v_lst.push_back(p_middle_top[3]);
        cLoop * inner_loop_top = FindLoopByVerticesInSolid(Mainsolid, v_lst);
        mev(inner_loop_top, p_middle_top[0], p_middle_bottom[0]);
        mev(inner_loop_top, p_middle_top[1], p_middle_bottom[1]);
        mev(inner_loop_top, p_middle_top[2], p_middle_bottom[2]);
        mev(inner_loop_top, p_middle_top[3], p_middle_bottom[3]);

    }

    // mef * 4
    {
        mef(Mainsolid, p_middle_bottom[0], p_middle_bottom[1]);
        mef(Mainsolid, p_middle_bottom[1], p_middle_bottom[2]);
        mef(Mainsolid, p_middle_bottom[2], p_middle_bottom[3]);
        mef(Mainsolid, p_middle_bottom[3], p_middle_bottom[0]);
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
    mVertexList.push_back(new_vertex);
    mLoopList.push_back(new_loop);
    mFaceList.push_back(new_face);
    mSolidList.push_back(new_solid);

    // set vertex
    new_vertex->mPos = pos;
    // new_vertex->mOriHalfEdge = nullptr;

    // set loop
    new_loop->mFirstHalfEdge = nullptr;
    new_loop->mFirstVertex = new_vertex;

    // set face
    new_face->mFirstLoop = new_loop;

    // set solid, loops are under the manegment of face
    new_solid->mFirstFace = new_face;

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
        // 这里就不对了: 因为adjacentHF的是不一样的, 
        cHalfEdge * hf_to_v = hf_from_v->mPrevHF;
        hf_to_v->mNextHF = new_halfedge1;
        new_halfedge1->mPrevHF = hf_to_v;

        new_halfedge2->mNextHF = hf_from_v;
        hf_from_v->mPrevHF = new_halfedge2;
    }

}

void cEulerWorld::mef(cSolid * solid, const Eigen::Vector3d & v1_pos, const Eigen::Vector3d & v2_pos)
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

    // 1. create new face 
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
    
    // 2. create new edge and new loop
    cEdge * new_edge = new cEdge();
    cHalfEdge   * new_hf1 = new cHalfEdge(),        // v1 -> v2
                * new_hf2 = new cHalfEdge();        // v2 -> v1
    mEdgeList.push_back(new_edge);
    mHalfEdgeList.push_back(new_hf1);
    mHalfEdgeList.push_back(new_hf2);

    // 2.1 set new edge, new halfedges
    new_edge->mHalfEdge1 = new_hf1;
    new_edge->mHalfEdge2 = new_hf2;

    new_hf1->mOriVertex = v1, new_hf1->mDestVertex = v2;
    new_hf2->mOriVertex = v2, new_hf2->mDestVertex = v1;
    new_hf1->mAdjacentHF = new_hf2;
    new_hf2->mAdjacentHF = new_hf1;

    // 2.3 find the loop which includes v1 and v2
    std::vector<cVertex *> v_lst;
    v_lst.push_back(v1), v_lst.push_back(v2);
    cLoop * cur_loop = FindLoopByVerticesInSolid(solid, v_lst);

    if(nullptr == cur_loop)
    {
        std::cout <<"[error] mef: no loop includes v1 and v2 " << std::endl;
        exit(1);
    }

    // in cur_loop, find the halfedge which pointed from v1 and pointed to v2
    cHalfEdge * hf_to_v1, * hf_to_v2, * hf_from_v1, * hf_from_v2;
    hf_from_v1 = FindHalfEdgeByOriVertex(cur_loop, v1);
    hf_from_v2 = FindHalfEdgeByOriVertex(cur_loop, v2);
    hf_to_v1 = hf_from_v1->mPrevHF;
    hf_to_v2 = hf_from_v2->mPrevHF;

    // 2.4 set the relationship between halfedges
    hf_to_v1->mNextHF = new_hf1;
    new_hf1->mPrevHF = hf_to_v1;
    new_hf1->mNextHF = hf_from_v2;
    hf_from_v2->mPrevHF = new_hf1;

    hf_from_v1->mPrevHF = new_hf2;
    new_hf2->mNextHF = hf_from_v1;
    new_hf2->mPrevHF = hf_to_v2;
    hf_to_v2->mNextHF = new_hf2;

    // 2.5 create a new loop and set the relationship between loop and halfedges
    cLoop * new_loop = new cLoop();
    mLoopList.push_back(new_loop);
    
    // 2.6 set relation between loop and hf & vertices
    cur_loop->mFirstHalfEdge = new_hf1;
    cur_loop->mFirstVertex = new_hf1->mOriVertex;
    new_loop->mFirstHalfEdge = new_hf2;
    new_loop->mFirstVertex = new_hf2->mOriVertex;

    // 2.7 loop and faces
    new_face->mFirstLoop = new_loop;
}

void cEulerWorld::kemr(cSolid * solid, const Eigen::Vector3d & ori_v, const Eigen::Vector3d & dest_v)
{
    // check input
    // 1. find the loop including ori_v and dest_v, 
    std::vector<Eigen::Vector3d> v_lst;
    v_lst.push_back(ori_v);
    v_lst.push_back(dest_v);
    cLoop * cur_loop = FindLoopByVerticesInSolid(solid, v_lst);
    // Show(cur_loop);
    if(nullptr == cur_loop || nullptr == cur_loop->mFirstHalfEdge)
    {
        std::cout <<"[error] kemr: failed to find the destinated loop " << std::endl;
        exit(1);
    }
    
    // 2. then find a pair of halfedges in this loop which point to <ori_v, dest_v>
    /*
        ----->hf1_o   ---->hf1    ------>hf1_d
        -----------ori--------dest-------- 
        <-----hf2_o   <----hf2    <------hf2_d
        
                        | |
                        | |
                        | |
                      \ | | /
                       \   /
                        \ /         

        ----->hf1_o               ------>hf1_d
        -----------ori         dest-------- 
        <-----hf2_o               <------hf2_d
    */
    cVertex * ori_v_ptr = FindVertexByPos(cur_loop, ori_v),
            * dest_v_ptr = FindVertexByPos(cur_loop, dest_v);
    cHalfEdge * hf1 = FindHalfEdgeBy2End(cur_loop, ori_v, dest_v);
    cHalfEdge * hf2 = hf1->mAdjacentHF;
    cHalfEdge   * hf1_o = hf1->mPrevHF,
                * hf2_o = hf2->mNextHF,
                * hf1_d = hf1->mNextHF,
                * hf2_d = hf2->mPrevHF;
    // if ori or dest is the end point of the new ring, set some hf to zero
    if(hf1_o == hf2) hf1_o = nullptr;
    if(hf2_o == hf1) hf2_o = nullptr;
    if(hf1_d == hf2) hf1_d = nullptr;
    if(hf2_d == hf1) hf2_d = nullptr;

    // ori & dest vertex ptrs managment
    if(hf1_o) hf1_o->mNextHF = hf2_o;
    if(hf2_o) hf2_o->mPrevHF = hf1_o;
    if(hf1_d) hf1_d->mPrevHF = hf2_d;
    if(hf2_d) hf2_d->mNextHF = hf1_d;

    // 3. create & set new loop, connect the new loop 
    cLoop * new_loop = new cLoop();
    if(cur_loop->mNextLoop != nullptr)
    {
        std::cout <<"[error] kemr: the next loop of cur_loop is not empty " << std::endl;
        exit(1);
    }
    
    // 3.1 attach the new loop to the cur_loop
    cur_loop->mNextLoop = new_loop;

    // 3.2 set the first vertex and first halfedge for the new loop
    if(CheckVertexInLoop(cur_loop, dest_v_ptr)) // just side
    {
        // if new_loop is in the origin side
        new_loop->mFirstVertex = ori_v_ptr;
        new_loop->mFirstHalfEdge = hf1_o;
        // if the first edge of cur_loop is hf1 or hf2, move it to hf1_o
        if(cur_loop->mFirstHalfEdge == hf1 || cur_loop->mFirstHalfEdge == hf2) cur_loop->mFirstHalfEdge = hf1_d;
    }
    else
    {
        // if new_loop is in the destination side
        new_loop->mFirstVertex = dest_v_ptr;
        new_loop->mFirstHalfEdge = hf1_d;
        // if the first edge of cur_loop is hf1 or hf2, move it to hf1_o
        if(cur_loop->mFirstHalfEdge == hf1 || cur_loop->mFirstHalfEdge == hf2) cur_loop->mFirstHalfEdge = hf1_o;
    }

    // 4. remove hf1 & hf2 & their edge accordly safely
    RemoveEdge(hf1, hf2);

}

void cEulerWorld::RemoveEdge(cHalfEdge * hf1, cHalfEdge * hf2)
{
    // check input
    if(hf1 == nullptr || hf2 == nullptr)
    {
        std::cout <<"[error] RemoveEdge: input empty" << std::endl;
        exit(1);
    }

    // find & remove edge
    bool is_find_edge =false;
    for(std::vector<cEdge *>::iterator it_e = this->mEdgeList.begin(); it_e != mEdgeList.end(); it_e++)
    {
        if  ( 
                ((*it_e)->mHalfEdge1 == hf1 && (*it_e)->mHalfEdge2 == hf2)
                    ||
                ((*it_e)->mHalfEdge1 == hf2 && (*it_e)->mHalfEdge2 == hf1)
            )
        {
            delete (*it_e);
            mEdgeList.erase(it_e);
            is_find_edge = true;
            break;
        }
    }
    if(false == is_find_edge)
    {
        std::cout <<"[error] RemoveEdge: find edge failed" << std::endl;
        exit(1);
    }

    // remove halfedge and edge
    bool is_remove_hf1 = false, is_remove_hf2 = false;
    std::vector<cHalfEdge *>::iterator it_hf = this->mHalfEdgeList.begin();
    while( it_hf != mHalfEdgeList.end())
    {
        if((*it_hf) == hf1) 
        {
            delete (*it_hf);
            mHalfEdgeList.erase(it_hf);
            is_remove_hf1 = true;
        }
        else if((*it_hf) == hf2)
        {
            delete (*it_hf);
            mHalfEdgeList.erase(it_hf);
            is_remove_hf2 = true;
        }
        else 
        {
            it_hf++;
        }
    }

    // if remove faile
    if(false == (is_remove_hf1 && is_remove_hf2))
    {
        std::cout <<"[error] RemoveEdge: remove halfedges failed" << std::endl;
        exit(1);
    }


}