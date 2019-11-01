#include "cScene.hpp"

/*
    class cDrawScene:
        Add rendering functionalities to its base class cScene, which is aim at displaying on the screen
*/
class cDrawScene : public cScene{
public:
    cDrawScene();
    ~cDrawScene();
    virtual void Init() override;
    virtual void Update() override;
    virtual void AddObj();


private:
    void InitMembers();
    int mObjNum;
};