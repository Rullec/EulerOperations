#include "cDrawScene.hpp"
#include <iostream>
using namespace std;


cDrawScene::cDrawScene()
{
    std::cout <<"[log] cDrawScene constructed" << std::endl;
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

    }
    catch (const char *msg)
    {
        std::cerr <<"[error] Exception when cDrawScene init: " << msg << std::endl;
        exit(1);
    }

}

void cDrawScene::Update()
{
    // std::cout <<"[log] cDrawScene Update" << std::endl;
}

void cDrawScene::AddObj()
{
    std::cout <<"[log] cDrawScene AddObj called, before that the num of obj = " << this->mObjNum << std::endl;
}

void cDrawScene::InitMembers()
{ 
    std::cout << "[log] cDrawScene InitMembers called" << std::endl;

}
