#include "cScene.hpp"

/*
    class cDrawScene:
        Add rendering functionalities to its base class cScene, which is aim at displaying on the screen
*/
class cBaseRender;
class cDrawScene : public cScene{
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
    virtual void MainLoop();

private:
    // construct memebers
    void ConstructMembers(int argc, char ** argv, std::string );

    // init funcs
    void ParseConf();
    void InitGL();
    void InitRender();

    // member vars
    std::string mConfPath;
    std::unique_ptr<struct MainWindowInfo> mMainWindowInfo;
    std::unique_ptr<cBaseRender> mRender;
};