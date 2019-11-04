#include "cBaseRender.hpp"

class cEulerWorld;
class cBaseObj;
class cEulerRender: public cBaseRender{
public:
    cEulerRender(std::string conf_path);
    virtual void Init() override final;
    virtual void Update() override final;
    void SetEulerWorld(cEulerWorld *);

private:
    void ConstructMembers();
    void DrawFace(cBaseObj *);
    void DrawEdge(cBaseObj *);
    void DrawVertex(cBaseObj *);

    cEulerWorld * mEulerWorld;
};