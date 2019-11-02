#include "cScene.hpp"
#include <iostream>

cScene::cScene()
{
    // std::cout << "[log] base scene constructed" << std::endl;
    mSceneStatus = BeforeInit;
}

cScene::~cScene()
{
    // std::cout << "[log] base scene deconstructed" << std::endl;
}