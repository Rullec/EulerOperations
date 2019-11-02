#include <memory>
#include <string>
class cScene 
{   
public:
    cScene();
    ~cScene();
    virtual void Init() = 0;
    virtual void Update() = 0;

protected:
    enum SceneStatus{
        BeforeInit = 0,
        InitSucc,
        Destroied,
        ScenStatusNum
    };
    
    std::string SceneStatusStr[ScenStatusNum] = {
        "BeforeInit", 
        "InitSucc", 
        "Destoried" 
    };
    enum SceneStatus mSceneStatus;
};