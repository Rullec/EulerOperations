#pragma once

#include "cEulerObj.hpp"
#define EPS 1e-6

void Show(cVertex * v)
{
    if(nullptr != v)
    {
        std::cout <<"[log] show vertex pos = " << v->mPos.transpose() << std::endl;
    }
    else
    {
        std::cout <<"[log] vertex "<<  v <<" is empty" << std::endl;
    }
}

void Show(cHalfEdge * hf)
{
    if(nullptr != hf)
    {
        std::cout <<"******************************" << std::endl;
        std::cout <<"[log] show prev halfedge ori_v = " << hf->mPrevHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mPrevHF->mDestVertex->mPos.transpose() << std::endl;
        std::cout <<"[log] show cur halfedge ori_v = " << hf->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mDestVertex->mPos.transpose() << std::endl;
        std::cout <<"[log] show adjacent halfedge ori_v = " << hf->mAdjacentHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mAdjacentHF->mDestVertex->mPos.transpose() << std::endl;
        std::cout <<"[log] show next halfedge ori_v = " << hf->mNextHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mNextHF->mDestVertex->mPos.transpose() << std::endl;
        std::cout <<"******************************" << std::endl;
    }
    else
    {
        std::cout <<"[log] halfedge "<< hf <<" is empty" << std::endl;
    }   
}

void Show(cLoop *loop)
{
    if(nullptr == loop)
    {
        std::cout <<"[log] loop " << loop << " is empty" << std::endl;
    }
    else
    {
        int cnt = 0;
        cHalfEdge * first_hf = loop->mFirstHalfEdge;
        cHalfEdge * cur_hf = first_hf;
        std::cout <<"**********Show loop begin************" << std::endl;
        do{
            std::cout <<"********loop " << loop << " hf " << cnt++ << std::endl;
            Show(cur_hf);
            cur_hf = cur_hf->mNextHF;
        }while(cur_hf != first_hf);
        std::cout <<"**********Show loop end************" << std::endl;
    }
    
}

cHalfEdge * FindHalfEdgeByOriVertex(cLoop * loop, cVertex * v)
{
    if(nullptr == loop || nullptr == v)
    {
        std::cout <<"[error] FindHalfEdgeByOutVertex input null" << std::endl;
        exit(1);
    }
    cHalfEdge * cur_edge = loop->mFirstHalfEdge;
    while(cur_edge && cur_edge->mOriVertex != v)
    {
        cur_edge = cur_edge->mNextHF;
    }

    if(cur_edge && cur_edge->mOriVertex == v) return cur_edge;
    else return nullptr;
}

cVertex * FindVertexByPos(cLoop * loop, const Eigen::Vector3d &pos)
{
    if(nullptr == loop)
    {
        std::cout <<"[error] FindVertexByPos: the loop is empty" << std::endl;
        return nullptr;
    }

    cVertex * res = nullptr;
    cHalfEdge * cur_halfedge = loop->mFirstHalfEdge;
    if(cur_halfedge == nullptr)
    {
        // if there is no halfedge in this loop
        // we can infer that it has only 1 point
        if(loop->mFirstVertex != nullptr)
        {
            if((loop->mFirstVertex->mPos - pos).squaredNorm() < EPS) 
            {
                return loop->mFirstVertex;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            std::cout <<"[error] FindVertexByPos: empty loop" <<  std::endl;
            exit(1);
        }
        
    }

    do{
        Show(cur_halfedge);
        if((cur_halfedge->mOriVertex->mPos - pos).squaredNorm() < EPS)
        {
            res = cur_halfedge->mOriVertex;
            break;
        }

        cur_halfedge = cur_halfedge->mNextHF;
    }while(cur_halfedge != loop->mFirstHalfEdge && cur_halfedge != nullptr);

    return res;
    
}

cVertex * FindVertexByPos(cSolid * solid, const Eigen::Vector3d &pos)
{
    cLoop * first_loop;
    if(solid == nullptr || (first_loop = solid->mFirstLoop) == nullptr)
    {
        std::cout <<"[error] FindVertexByPos input solid is empty" << std::endl;
        exit(1);
    }

    cLoop * cur_loop = first_loop;
    cVertex * v = nullptr;
    do{
        
        v = FindVertexByPos(cur_loop, pos);
        if(v!= nullptr)
        {
            break;
        }
        cur_loop = cur_loop->mNextLoop;
    }while(cur_loop != nullptr && cur_loop != first_loop);

    return v;
}

int CheckVertexInLoop(cLoop * loop, cVertex *v)
{
    if(loop == nullptr && v == nullptr)
    {
        std::cout <<"[error] CheckVertexInLoop: input empty " << std::endl;
        exit(1);
    }

    cHalfEdge * cur_hf = loop->mFirstHalfEdge, * first_hf = loop->mFirstHalfEdge;
    if(cur_hf == nullptr)
    {
        std::cout <<"[error] CheckVertexInLoop: first hf loop illegal: " << std::endl;
    };

    do{
        if(v == cur_hf->mOriVertex || v == cur_hf->mDestVertex) return 1;
        cur_hf = cur_hf->mNextHF;
    }while(cur_hf != first_hf);

    return 0;
}

cLoop * FindLoopBy2Vertices(cSolid * solid, cVertex *v1, cVertex *v2)
{
    // check input
    if(nullptr == solid || nullptr == v1 || nullptr == v2)
    {
        std::cout <<"[error] FindLoopBy2Vertices: empty input " << std::endl;
        exit(1);
    }

    cLoop * cur_loop = solid->mFirstLoop;
    while(nullptr != cur_loop)
    {
        // check whether v1 and v2 are included in this loop?
        if(CheckVertexInLoop(cur_loop, v1) && CheckVertexInLoop(cur_loop, v2))
        {
            return cur_loop;
        }
        cur_loop = cur_loop->mNextLoop;
    }
    return nullptr;
}
