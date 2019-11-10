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
    void DrawFace();
    void DrawEdge();
    void DrawVertex();

    cEulerWorld * mEulerWorld;
};