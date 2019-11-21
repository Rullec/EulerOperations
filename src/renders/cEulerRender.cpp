#include "cEulerRender.hpp"
#include "../model/cEulerWorld.hpp"
#include "GL/glew.h"
#include "camera/Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
cEulerRender::cEulerRender(std::string conf_path):cBaseRender(conf_path)
{
    ConstructMembers();
}

void cEulerRender::Init()
{
    // base class
    cBaseRender::Init();

    // Init Camera

    mCamera = (std::unique_ptr<Camera>)(new Camera());
    
    // init the location of uniform vars in the shader
    glUseProgram(mShaderProgram);
    
    mProjectionLocation = glGetUniformLocation(mShaderProgram, "projection");
    mViewLocation = glGetUniformLocation(mShaderProgram, "view");
    mModelLocation = glGetUniformLocation(mShaderProgram, "model");
    if(mProjectionLocation == -1 || mViewLocation == -1 || mModelLocation == -1)
    {
        std::cout <<"[error] cEulerRender::Init: get uniform location from glsl error" << std::endl;
    }
}

void cEulerRender::Update()
{
    std::cout <<"[log] cEulerRender Update" << std::endl;
    if(nullptr == mEulerWorld)
    {
        std::cout <<"[log] cEulerRender: ptr to Eulerworld is empty, return" << std::endl;
        return;
    }
    
    // before set matrix: Eigen is col-major but gl is row-major, so we need to transpose
    
    mProjectMat.transposeInPlace();
    mViewMat.transposeInPlace();
    mModelMat.transposeInPlace();
    
    std::cout <<"[warning] cEulerRender::Update, potential bug here" << std::endl;
    glm::mat4 view_matrix = mCamera->ViewMatrix();
    std::cout << glm::to_string(view_matrix) << std::endl;
    
    glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, mProjectMat.data());
    // glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, mViewMat.data());
    glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, mModelMat.data());

    DrawVertex();
    DrawEdge();
    DrawFace();

    cBaseRender::Update();

}
void cEulerRender::KeyBoardEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': mCamera->moveForward();break;
    case 'a': mCamera->moveLeft();break;
    case 's': mCamera->moveBackward();break;
    case 'd': mCamera->moveRight();break;
    default: 
        break;
    }
}
void cEulerRender::MouseMoveEvent(int x, int y)
{
    std::cout <<"[log] cEulerRender::MouseMove Event "<< x <<" " << y << std::endl;
    mCamera->mouseMove(x, y);
}

void cEulerRender::SetEulerWorld(cEulerWorld * world)
{
    if(nullptr != mEulerWorld || nullptr == world)
    {
        std::cout <<"[error] cEulerRender euler world set repeated && error" << std::endl;
        exit(1);
    }

    mEulerWorld = world;
}

void cEulerRender::ConstructMembers()
{
    mEulerWorld = nullptr;
    mCamera = nullptr;
    mModelMat.setIdentity(), mProjectMat.setIdentity(), mViewMat.setIdentity();

}

void cEulerRender::DrawFace()
{
    std::vector<cFace *> face_list = mEulerWorld->GetFaceList();
    // std::cout <<"[log] draw " << face_list.size() << " faces" << std::endl;
}

void cEulerRender::DrawEdge()
{
    std::vector<cEdge *> edge_list = mEulerWorld->GetEdgeList();
    // std::cout <<"[log] draw " << edge_list.size() << " edges" << std::endl;
    std::vector<cHalfEdge *> halfedge_list = mEulerWorld->GetHalfEdgeList();
    // std::cout <<"[log] draw " << halfedge_list.size() <<" halfedges" << std::endl;
}

void cEulerRender::DrawVertex()
{
    std::vector<cVertex *> vertex_list = mEulerWorld->GetVertexList();
    // std::cout <<"[log] draw " << vertex_list.size() << " vertices " << std::endl; 
}
    