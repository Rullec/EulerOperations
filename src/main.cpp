#include <iostream>
#include "scenes/cDrawScene.hpp"
#include "GL/glut.h"
using namespace std;
std::unique_ptr<cDrawScene> scene;

void Reshape(int w, int h)
{
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in Reshape(), please init scene at frist!" << std::endl;
        exit(1);
    }
}

void Display()
{
    std::cout <<"[log] Display called" << std::endl;
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in display(), please init scene at frist!" << std::endl;
        exit(1);
    }
    scene->Update();
    glutSwapBuffers();
}

void KeyFunc(unsigned char , int, int)
{
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in keyfunc(), please init scene at frist!" << std::endl;
        exit(1);
    }
}

void Mouse(int x0, int y0, int x1, int y1)
{
    std::cout <<"mouse called" << std::endl;
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in mouse(), please init scene at frist!" << std::endl;
        exit(1);
    }
}

void OnMouseMove(int, int)
{
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in OnMouseMove(), please init scene at frist!" << std::endl;
        exit(1);
    }
}

int main(int argc, char ** argv)
{
    std::cout << "It is only a simple example"<< std::endl;
    std::string conf_path = "../config/test.conf";
    scene = std::unique_ptr<cDrawScene>(new cDrawScene(argc, argv, conf_path));
    scene->Init();
    glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(Mouse);
	glutMotionFunc(OnMouseMove);
    scene->MainLoop();
    return 0;
}