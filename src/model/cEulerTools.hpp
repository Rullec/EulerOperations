#pragma once

#include "cEulerObj.hpp"
#define EPS 1e-6


bool IsTheSamePoint(const Eigen::Vector3d &a, const Eigen::Vector3d & b)
{
    if((a-b).squaredNorm() < EPS) return true;
    else return false;
}

bool IsTheSamePoint(const cVertex *a, const cVertex *b)
{
    return IsTheSamePoint(a->mPos, b->mPos);
}

bool IsTheSamePoint(const cVertex *a, const Eigen::Vector3d &b)
{
    return IsTheSamePoint(a->mPos, b);
}

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
        // std::cout <<"[log] show prev halfedge ori_v = " << hf->mPrevHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mPrevHF->mDestVertex->mPos.transpose() << std::endl;
        std::cout <<"[log] show cur halfedge ori_v = " << hf->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mDestVertex->mPos.transpose() << std::endl;
        // std::cout <<"[log] show adjacent halfedge ori_v = " << hf->mAdjacentHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mAdjacentHF->mDestVertex->mPos.transpose() << std::endl;
        // std::cout <<"[log] show next halfedge ori_v = " << hf->mNextHF->mOriVertex->mPos.transpose() << ", dest_v = " << hf->mNextHF->mDestVertex->mPos.transpose() << std::endl;
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
        if(first_hf == nullptr)
        {
            std::cout <<"loop " << loop <<" have only 1 vertex: " << loop->mFirstVertex->mPos.transpose() << std::endl;
        }
        else
        {
            do{
                std::cout <<"********loop " << loop << " hf " << cnt++ << std::endl;
                Show(cur_hf);
                cur_hf = cur_hf->mNextHF;
            }while(cur_hf != first_hf);
        }
        
        
        std::cout <<"**********Show loop end************" << std::endl;
    }
    
}
void Show(cFace * face)
{
    std::cout <<"**********Show face begin************" << std::endl;
    cLoop * cur_loop = face->mFirstLoop;
    int cnt = 0;
    while(cur_loop)
    {
        std::cout <<"**********solid loop " << cur_loop << " " << cnt++ << std::endl;
        Show(cur_loop);
        cur_loop = cur_loop->mNextLoop;
    }
    std::cout <<"**********Show face end************" << std::endl;
}

void Show(cSolid * solid)
{
    std::cout <<"**********Show solid begin************" << std::endl;
    cFace * cur_face = solid->mFirstFace;
    int cnt = 0;
    while(cur_face)
    {
        std::cout <<"**********solid face " << cur_face << " " << cnt++ << std::endl;
        Show(cur_face);
        cur_face = cur_face->mNextFace;
    }
    std::cout <<"**********Show solid end************" << std::endl;
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
            if(IsTheSamePoint(loop->mFirstVertex->mPos, pos))
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
        // Show(cur_halfedge);
        if(IsTheSamePoint(cur_halfedge->mOriVertex->mPos, pos))
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
    cFace * cur_face = nullptr;
    if(solid == nullptr || (cur_face = solid->mFirstFace) == nullptr)
    {
        std::cout <<"[error] FindVertexByPos input solid is empty" << std::endl;
        exit(1);
    }

    while(cur_face)
    {
        cLoop * cur_loop = cur_face->mFirstLoop;
        cVertex * v = nullptr;
        while(cur_loop != nullptr)
        {
            
            v = FindVertexByPos(cur_loop, pos);
            if(v!= nullptr)
            {
                return v;
            }
            cur_loop = cur_loop->mNextLoop;
        };
        cur_face = cur_face->mNextFace;
    }
    
    return nullptr;
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

cHalfEdge * FindHalfEdgeBy2End(cLoop * loop, const Eigen::Vector3d & ori_v, const Eigen::Vector3d & dest_v)
{
    // check input
    if(loop == nullptr || loop->mFirstHalfEdge == nullptr)
    {
        std::cout <<"[error] FindHalfEdgeByOriVertex: input loop empty" << std::endl;
        exit(1);
    }

    cHalfEdge * first_hf = loop->mFirstHalfEdge;
    cHalfEdge * cur_hf = first_hf;
    do{
        if(IsTheSamePoint(cur_hf->mOriVertex, ori_v) && IsTheSamePoint(cur_hf->mDestVertex, dest_v))
        {
            return cur_hf;
        }
        cur_hf = cur_hf->mNextHF;
    }while(cur_hf != first_hf);

    return nullptr;
}

int CheckVertexInLoop(cLoop * loop, cVertex *v)
{
    // Show(loop);
    if(loop == nullptr && v == nullptr)
    {
        std::cout <<"[error] CheckVertexInLoop: input empty " << std::endl;
        exit(1);
    }

    cHalfEdge * cur_hf = loop->mFirstHalfEdge, * first_hf = loop->mFirstHalfEdge;
    if(cur_hf == nullptr)
    {
        if(loop->mFirstVertex == v)
        {
            return 1;
        } 
        else
        {
            std::cout <<"[error] CheckVertexInLoop: first hf loop illegal: " << std::endl;
            exit(1);
        }
    };

    do{
        if(v == cur_hf->mOriVertex || v == cur_hf->mDestVertex) return 1;
        cur_hf = cur_hf->mNextHF;
    }while(cur_hf != first_hf);

    return 0;
}

cLoop * FindLoopByVerticesInFace(cFace * face, const std::vector<cVertex *> & v_lst)
{
    // check input
    if(nullptr == face)
    {
        std::cout <<"[error] FindLoopByVerticesInFace: empty input face" << std::endl;
        exit(1);
    }
    if(v_lst.size() == 0)
    {
        std::cout <<"[error] FindLoopByVerticesInFace: empty input vertices list" << std::endl;
        exit(1);
    }

    for(int i=0; i<v_lst.size(); i++)
    {
        if(v_lst[i] == nullptr)
        {
            std::cout <<"[error] FindLoopByVerticesInFace: empty input vertices list" << std::endl;
            exit(1);
        }
    }

    cLoop * cur_loop = face->mFirstLoop;
    while(nullptr != cur_loop)
    {
        // check whether v_lst are included in this loop?
        bool all_vertices_in_the_loop = true;
        for(int i=0; i<v_lst.size(); i++)
        {
            if(false == CheckVertexInLoop(cur_loop, v_lst[i]))
            {
                all_vertices_in_the_loop = false;
                break;
            }
        }
        if(true == all_vertices_in_the_loop)
        {
            return cur_loop;
        }
        
        cur_loop = cur_loop->mNextLoop;
    }
    return nullptr;
}


cLoop * FindLoopByVerticesInSolid(cSolid * solid, const std::vector<cVertex *> & v_lst)
{
    // check input
    if(nullptr == solid)
    {
        std::cout <<"[error] FindLoopByVerticesInSolid: empty input solid" << std::endl;
        exit(1);
    }
    if(v_lst.size() == 0)
    {
        std::cout <<"[error] FindLoopByVerticesInSolid: empty input vertices list" << std::endl;
        exit(1);
    }

    for(int i=0; i<v_lst.size(); i++)
    {
        if(v_lst[i] == nullptr)
        {
            std::cout <<"[error] FindLoopByVerticesInSolid: empty input vertices list" << std::endl;
            exit(1);
        }
    }

    cFace * cur_face = solid->mFirstFace;
    cLoop * target_loop = nullptr;
    while(cur_face)
    {
        target_loop = FindLoopByVerticesInFace(cur_face, v_lst);
        if(nullptr != target_loop)
        { 
            break;
        }
        cur_face = cur_face->mNextFace;
    }
    
    return target_loop;
}

cLoop * FindLoopByVerticesInSolid(cSolid * solid, const std::vector<Eigen::Vector3d> & v_lst_pos)
{
    // check input
    if(solid == nullptr)
    {
        std::cout <<"[error] FindLoopByVerticesInSolid: empty input solid" << std::endl;
        exit(1);
    }

    std::vector<cVertex *> v_lst_ptr;
    for(int i=0; i<v_lst_pos.size(); i++)
    {
        cVertex * cur_v = FindVertexByPos(solid, v_lst_pos[i]);
        if(cur_v == nullptr)
        {
            std::cout <<"[error] FindLoopByVerticesInSolid: find vertex failed" << std::endl;
            exit(1);
        }
        
        v_lst_ptr.push_back(cur_v);
    }
    
    cFace * cur_face = solid->mFirstFace;
    while(cur_face)
    {
        cLoop * cur_loop = FindLoopByVerticesInFace(cur_face, v_lst_ptr);
        if( nullptr != cur_loop )
        {
            // find it!
            return cur_loop;
        }
        cur_face = cur_face->mNextFace;
    }

    return nullptr;
}