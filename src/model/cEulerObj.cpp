#include "cEulerObj.hpp"

// base object
cBaseObj::cBaseObj(int obj_id)
{
    mObjID = obj_id;
    mObjType = ObjType::Undefined; 
}

// Vertex object
cVertex::cVertex(int obj_id):cBaseObj(obj_id)
{
    mObjType = ObjType::Euler_Vertex;
    mPos.setZero();
}

// half edge object
cHalfEdge::cHalfEdge(int obj_id):cBaseObj(obj_id)
{
    mObjType = ObjType::Euler_HalfEdge;
    mParentEdge = nullptr;
    mOriVertex = nullptr, mDestVertex = nullptr;
    mLoop = nullptr;
    mAdjacentHF = nullptr;
    mNextHF = mPrevHF = nullptr;
}

// edge obj
cEdge::cEdge(int obj_id):cBaseObj(obj_id)
{
    mObjType = ObjType::Euler_Edge;
    mHE1 = mHE2 = nullptr;
    mNextEdge = mPrevEdge = nullptr;
}

// face obj
cFace::cFace(int obj_id):cBaseObj(obj_id)
{
    mObjType = Euler_Face;
    mSolid = nullptr;
    mFirstLoop = nullptr;
    mNextFace = mPrevFace = nullptr;
}

// loop obj
cLoop::cLoop(int obj_id):cBaseObj(obj_id)
{
    mObjType = Euler_Loop;
    mFirstHalfEdge = nullptr;
    mFace = nullptr;
    mNextLoop = mPrevLoop = nullptr;
}

// solid obj
cSolid::cSolid(int obj_id):cBaseObj(obj_id)
{
    mObjType = Euler_Solid;
    mFirstFace = nullptr;
    mFirstEdge = nullptr;
    mFirstVertex = nullptr;
    mNextSolid = mPrevSolid = nullptr;
}

