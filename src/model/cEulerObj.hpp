#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <Eigen/Dense>

enum ObjType{
    Undefined = 0,
    Euler_Vertex,
    Euler_HalfEdge,
    Euler_Edge,
    Euler_Face,
    Euler_Loop,
    Euler_Solid,
    ObjTypeNum
};

struct cBaseObj;
struct cVertex;
struct cHalfEdge;
struct cEdge;
struct cFace;
struct cLoop;
struct cSolid;

// base objs
struct cBaseObj{
    cBaseObj();
    enum ObjType mObjType;
};

// vertex
struct cVertex: public cBaseObj{
    cVertex();
    Eigen::Vector3d mPos;
    struct cHalfEdge * mOriHalfEdge;
};

// half edge
struct cHalfEdge : public cBaseObj{
    cHalfEdge();
    cVertex * mOriVertex, * mDestVertex;
    cHalfEdge * mAdjacentHF;
    cHalfEdge *mNextHF, *mPrevHF;
};

// edge obj
struct cEdge: public cBaseObj{
    cEdge();
    cHalfEdge * mHalfEdge1, *mHalfEdge2;  // 2 half edges
};

// face obj
struct cFace : public cBaseObj{
    cFace();
    cLoop * mFirstLoop; // outer loop
    cFace * mPrevFace, * mNextFace;
};

// loop obj
struct cLoop: public cBaseObj{
    cLoop();
    cHalfEdge * mFirstHalfEdge;  // Vertex to a edge which consists of the loop.
    cLoop * mNextLoop, * mPrevLoop;
};

// solid obj
struct cSolid : public cBaseObj{
    cSolid();
    cLoop * mFirstLoop;
    cFace * mFirstFace;
};