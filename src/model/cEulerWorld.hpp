#pragma once
#include "cEulerObj.hpp"
#include <vector>

class cEulerWorld{
public:
    cEulerWorld();
    void EulerValid();      // check the validity of euler operation
    void Construct();       // default construct method

    // base euler ops
    void mev(cLoop *loop, const Eigen::Vector3d &, const Eigen::Vector3d &);         // mev overloaded
    void mev(cLoop *loop, cVertex * Vertex, const Eigen::Vector3d &);        // make a vertex and connect it with an existing one with an edge
    void mef(cSolid * solid, const Eigen::Vector3d &, const Eigen::Vector3d &);      // mef overloaded
    void mef(cSolid * solid, cVertex *v1, cVertex * v2);             // make a new edge and a face
    cSolid *mvfs(Eigen::Vector3d );            // make vertex, face, solid
    void kemr(cSolid * solid, const Eigen::Vector3d & ori_v, const Eigen::Vector3d & dest_v);            // kill edge make ring
    void kfmrh();           // kill face make ring and h

    std::vector<struct cVertex *> & GetVertexList(){return mVertexList;}
    std::vector<struct cHalfEdge *> & GetHalfEdgeList(){return mHalfEdgeList;}
    std::vector<struct cEdge *> & GetEdgeList(){return mEdgeList;}
    std::vector<struct cLoop *> & GetLoopList(){return mLoopList;}
    std::vector<struct cFace *> & GetFaceList(){return mFaceList;}
    std::vector<struct cSolid *> & GetSolidList(){return mSolidList;}

private:

    void RemoveEdge(cHalfEdge * hf1, cHalfEdge * hf2);

    //ObjTypeNum vectors, prepare a vector for each obj type
    std::vector<struct cVertex *> mVertexList;
    std::vector<struct cHalfEdge *> mHalfEdgeList;
    std::vector<struct cEdge *> mEdgeList;
    std::vector<struct cLoop *> mLoopList;
    std::vector<struct cFace *> mFaceList;
    std::vector<struct cSolid *> mSolidList;
};
