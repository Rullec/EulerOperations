#include "cScene.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
/*
    class cDrawScene:
        Add rendering functionalities to its base class cScene, which is aim at displaying on the screen
*/
class cDrawScene : public cScene{
    struct MainWindowInfo{
        MainWindowInfo(int _argc = 1, char ** _argv = nullptr, int width = 600, int length = 800);
        int argc;
        char ** argv;
        int width, length;
    };

public:
    cDrawScene(int argc, char ** argv);
    ~cDrawScene();
    virtual void Init() override;
    virtual void Update() override;
    virtual void MainLoop();
    virtual void AddObj();

private:
    void InitMembers(int argc, char ** argv);
    void InitGL();
    int mObjNum;
    std::unique_ptr<struct MainWindowInfo> mMainWindowInfo;
};