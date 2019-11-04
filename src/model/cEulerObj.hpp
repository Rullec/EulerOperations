#include <iostream>
#include <memory>
#include <Eigen/Dense>

enum ObjType{
    Undefined,
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
    cBaseObj(int obj_id);

    int mObjID;
    enum ObjType mObjType;
};

// Vertex obj
struct cVertex: public cBaseObj{
    cVertex(int obj_id);
    Eigen::Vector3d mPos;
};

// half edge obj
struct cHalfEdge : public cBaseObj{
    cHalfEdge(int obj_id);
    cEdge * mParentEdge;
    cVertex * mOriVertex, * mDestVertex;
    cLoop * mLoop;
    cHalfEdge * mAdjacentHF;
    cHalfEdge *mNextHF, *mPrevHF;
};

// edge obj
struct cEdge: public cBaseObj{
    cEdge(int obj_id);
    cHalfEdge * mHE1, *mHE2;  // 2 half edges
    cEdge *mNextEdge, *mPrevEdge;
};

// face obj
struct cFace : public cBaseObj{
    cFace(int obj_id);
    cSolid * mSolid;
    cLoop * mFirstLoop; // outer loop
    cFace *mNextFace, *mPrevFace;
};

// loop obj
struct cLoop: public cBaseObj{
    cLoop(int obj_id);
    cHalfEdge * mFirstHalfEdge;  // Vertex to a edge which consists of the loop.
    cFace * mFace;
    cLoop *mNextLoop, *mPrevLoop;
};

// solid obj
struct cSolid : public cBaseObj{
    cSolid(int obj_id);
    cFace * mFirstFace;  
    cEdge * mFirstEdge;
    cVertex * mFirstVertex;
    cSolid *mNextSolid, *mPrevSolid;
};