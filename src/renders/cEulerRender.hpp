#pragma once
#include "cBaseRender.hpp"
#include <Eigen/Dense>

class cEulerWorld;
class cBaseObj;
class Camera;
class cLoop;
class cEulerRender: public cBaseRender{
public:
    cEulerRender(std::string conf_path);
    virtual void Init() override final;
    virtual void Update() override final;
    virtual void KeyBoardEvent(unsigned char key, int x, int y);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MouseEvent(int button, int state, int x, int y);

    void SetEulerWorld(cEulerWorld *);

private:
    // methods
    void ConstructMembers();
    void DrawFace();
    void DrawOuterLoop(cLoop *);
    void DrawInnerLoop(cLoop *);
    
    void DrawEdge();
    void DrawVertex();
    
    // members
    // 1. resource members
    cEulerWorld * mEulerWorld;

    // 2. GL members
    std::unique_ptr<Camera> mCamera;
    Eigen::Matrix4f mModelMat, mViewMat, mProjectMat;
    int mProjectionLocation, mViewLocation, mModelLocation, mColorLocation;
    std::vector<Eigen::Vector4f> mColorPool;
};