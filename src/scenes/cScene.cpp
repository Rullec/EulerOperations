#include "cScene.hpp"
#include <iostream>

cScene::cScene()
{
    std::cout << "[log] base scene constructed" << std::endl;
    this->mSceneId = 0;
}

cScene::~cScene()
{
    std::cout << "[log] base scene deconstructed" << std::endl;
}