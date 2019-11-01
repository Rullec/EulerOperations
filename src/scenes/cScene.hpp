#include <memory>

class cScene 
{
    enum SceneStatus{
        BeforeInit,
        InitSucc,
        Destroied
    };
    
public:
    cScene();
    ~cScene();
    virtual void Init() = 0;
    virtual void Update() = 0;

private:
    enum SceneStatus mSceneStatus;
    int mSceneId;
};