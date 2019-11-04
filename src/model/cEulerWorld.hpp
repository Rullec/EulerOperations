#include "cEulerObj.hpp"
#include <vector>

class cEulerWorld{
public:
    cEulerWorld();
    void EulerValid();      // check the validity of euler operation
    void Construct();       // default construct method

    // base euler ops
    void mev(cSolid * solid, cFace * face, cVertex * Vertex, double x, double y, double z);             // make a vertex and connect it with an existing one with an edge
    void mef();             // make a new edge and a face
    cSolid *mvfs(double x, double y, double z);            // make vertex, face, solid
    void kemr();            // kill edge make ring
    void kfmrh();           // kill face make ring and h

    // get & set method
    std::vector<cBaseObj *> & GetObjManager(){return mObjManager;};

private:
    std::vector<cBaseObj *> mObjManager; // all objs collection
    unsigned int mObjId;
    
};
