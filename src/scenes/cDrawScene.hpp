#pragma once
#include "cScene.hpp"

/*
    class cDrawScene:
        Add rendering functionalities to its base class cScene, which is aim at displaying on the screen
*/
class cBaseRender;

class cDrawScene : public cScene{
    enum RenderType{
        Base = 0,
        Euler,
        RenderTypeNum
    };

    struct MainWindowInfo{
        MainWindowInfo(int _argc = 1, char ** _argv = nullptr, int start_x = 100, int start_y = 100, 
            int width = 600, int height = 800);
        int argc;
        char ** argv;
        int start_x, start_y;
        int width, height;
        double clear_color[4];
    };

public:
    cDrawScene(int argc, char ** argv, std::string);
    ~cDrawScene();
    virtual void Init() override;
    virtual void Update() override;
    virtual void KeyBoardEvent(unsigned char key, int x, int y) override;
    virtual void MouseMoveEvent(int, int) override;
    virtual void MouseEvent(int button, int state, int x, int y) override;
    virtual void MainLoop();

protected:
    // construct memebers
    void ConstructMembers(int argc, char ** argv, std::string );

    // init funcs
    void ParseConf();
    void InitGL();
    virtual void InitRender();

    // member vars
    std::string mConfPath;
    enum RenderType mRenderType; // "Base" or "Euler"
    std::unique_ptr<struct MainWindowInfo> mMainWindowInfo;
    std::unique_ptr<cBaseRender> mRender;
};