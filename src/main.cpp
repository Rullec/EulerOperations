#include <iostream>
#include "scenes/cDrawScene.hpp"
using namespace std;
std::unique_ptr<cDrawScene> scene;

void reshape(int w, int h)
{
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in reshape(), please init scene at frist!" << std::endl;
        exit(1);
    }
}

int main(int argc, char ** argv)
{
    std::cout << "It is only a simple example"<< std::endl;
    scene = std::unique_ptr<cDrawScene>(new cDrawScene(argc, argv));
    scene->Init();
    glutReshapeFunc(reshape);
	// glutDisplayFunc(Display);
	// glutKeyboardFunc(KeyFunc);
	// glutMouseFunc(Mouse);
	// glutMotionFunc(OnMouseMove);
    scene->MainLoop();
    return 0;
}