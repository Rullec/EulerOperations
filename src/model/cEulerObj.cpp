#include "cEulerObj.hpp"

// base object
cBaseObj::cBaseObj()
{
    mObjType = ObjType::Undefined; 
    VAO = -1;
}

// Vertex object
cVertex::cVertex():cBaseObj()
{
    mObjType = ObjType::Euler_Vertex;
    mPos.setZero();
    // mOriHalfEdge = nullptr;
}

// half edge object
cHalfEdge::cHalfEdge():cBaseObj()
{
    mObjType = ObjType::Euler_HalfEdge;
    mOriVertex = nullptr, mDestVertex = nullptr;
    mAdjacentHF = nullptr;
    mNextHF = mPrevHF = nullptr;
}

// edge obj
cEdge::cEdge():cBaseObj()
{
    mObjType = ObjType::Euler_Edge;
    mHalfEdge1 = mHalfEdge2 = nullptr;
}

// face obj
cFace::cFace():cBaseObj()
{
    mObjType = Euler_Face;
    mFirstLoop = nullptr;
    mNextFace = nullptr;
}

// loop obj
cLoop::cLoop():cBaseObj()
{
    mObjType = Euler_Loop;
    mFirstHalfEdge = nullptr;
    mFirstVertex = nullptr;
    mNextLoop = nullptr;
}

// solid obj
cSolid::cSolid():cBaseObj()
{
    mObjType = Euler_Solid;
    // mFirstLoop = nullptr;
    mFirstFace = nullptr;
    // mVertexList.clear();
    // mHalfEdgeList.clear();
    // mEdgeList.clear();
    // mLoopList.clear();
    // mFaceList.clear();
}

