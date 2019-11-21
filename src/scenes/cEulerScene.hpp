#pragma once
#include "cDrawScene.hpp"

class cEulerWorld;

class cEulerScene : public cDrawScene{
public:
    cEulerScene(int argc, char ** argv, std::string conf_path);
    virtual void Init() override final;
    virtual void Update() override final;
    virtual void KeyBoardEvent(unsigned char key, int x, int y) override final;
    virtual void MouseMoveEvent(int, int) override final;
    virtual void MainLoop() override final;

private:
    void ConstructMembers();
    virtual void InitRender() override final;
    std::unique_ptr<cEulerWorld> mEulerWorld;
};