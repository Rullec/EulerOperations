#include "cDrawScene.hpp"
#include <Eigen/Core>

#include <iostream>
using namespace std;

cDrawScene::MainWindowInfo::MainWindowInfo(
    int _argc/* = 1*/, 
    char ** _argv/* = nullptr*/, 
    int _width/* = 600,*/,
    int _length/* = 800*/): argc(_argc), argv(_argv), width(_width), length(_length)
{
}

cDrawScene::cDrawScene(int argc, char ** argv)
{
    // init vars 
    InitMembers(argc, argv);
}

cDrawScene::~cDrawScene()
{
    std::cout <<"[log] cDrawScene deconstructed" << std::endl;
}

void cDrawScene::Init()
{
    std::cout <<"[log] cDrawScene init begin." << std::endl;
    try
    {
        // Init GL
        InitGL();
    }
    catch (const char *msg)
    {
        std::cerr <<"[error] Exception when cDrawScene init: " << msg << std::endl;
        exit(1);
    }
    
}

void cDrawScene::Update()
{
    std::cout <<"[log] cDrawScene Update" << std::endl;
}

void cDrawScene::MainLoop()
{
    glutMainLoop();
}

void cDrawScene::AddObj()
{
    std::cout <<"[log] cDrawScene AddObj called, before that the num of obj = " << this->mObjNum << std::endl;
}

void cDrawScene::InitMembers(int argc, char ** argv)
{ 
    // std::cout << "[log] cDrawScene InitMembers called" << std::endl;
    // base 

    mObjNum = 0;
    mMainWindowInfo = std::unique_ptr<MainWindowInfo>(new MainWindowInfo());
    mMainWindowInfo->argc = argc;
    mMainWindowInfo->argv = argv;
}

void cDrawScene::InitGL()
{
    glutInit(&(mMainWindowInfo->argc), mMainWindowInfo->argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(mMainWindowInfo->length, mMainWindowInfo->width);
    glutCreateWindow("EulerOperationsMainWindow");
}