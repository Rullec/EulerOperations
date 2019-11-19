#pragma once

#include "cEulerObj.hpp"
#define EPS 1e-6

cHalfEdge * FindHalfEdgeByOutVertex(cLoop * loop, cVertex * v)
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

cVertex * FindVertexByPos(cLoop * loop, Eigen::Vector3d &pos)
{
    if(nullptr == loop || nullptr == loop->mFirstHalfEdge)
    {
        std::cout <<"[error] FindVertexByPos: the loop is empty" << std::endl;
        return nullptr;
    }

    cVertex * res = nullptr;
    cHalfEdge * cur_halfedge = loop->mFirstHalfEdge;

    do{
        if((cur_halfedge->mOriVertex->mPos - pos).squaredNorm() < EPS)
        {
            return res;
        }

        cur_halfedge = cur_halfedge->mNextHF;
    }while(cur_halfedge != loop->mFirstHalfEdge && cur_halfedge != nullptr);

    return res;
    
}

cVertex * FindVertexByPos(cSolid * solid, Eigen::Vector3d &pos)
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

