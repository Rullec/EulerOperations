#include <iostream>
#include "scenes/cEulerScene.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

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
    // std::cout <<"[log] Display called" << std::endl;
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in display(), please init scene at frist!" << std::endl;
        exit(1);
    }
    scene->Update();
    glutPostRedisplay();
    glutSwapBuffers();
}

void KeyFunc(unsigned char key, int x, int y)
{
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in keyfunc(), please init scene at frist!" << std::endl;
        exit(1);
    }
    
    scene->KeyBoardEvent(key, x, y);
    scene->Update();
}

void Mouse(int x0, int y0, int x1, int y1)
{
    // std::cout <<"mouse called" << std::endl;
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in mouse(), please init scene at frist!" << std::endl;
        exit(1);
    }
    // scene->MouseEvent(x1, y1);
    scene->Update();
}

void OnMouseMove(int x, int y)
{
    unsigned int width = glutGet(GLUT_WINDOW_WIDTH), height = glutGet(GLUT_WINDOW_HEIGHT);
    // std::cout <<"[log] mouse move " <<x <<" " << y << std::endl;
    // std::cout <<"[log] total size " <<width <<" " << height << std::endl;
    if(nullptr == scene)
    {
        std::cout <<"[error] scene in nullptr in OnMouseMove(), please init scene at frist!" << std::endl;
        exit(1);
    }
    scene->MouseMoveEvent(x, y);
}

int main(int argc, char ** argv)
{
    std::cout << "It is only a simple example"<< std::endl;
    std::string conf_path = "../config/test.conf";
    scene = std::unique_ptr<cDrawScene>(new cEulerScene(argc, argv, conf_path));
    scene->Init();
    glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(Mouse);
	glutMotionFunc(OnMouseMove);
    scene->MainLoop();
    return 0;
}