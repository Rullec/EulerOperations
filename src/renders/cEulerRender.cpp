#include "cEulerRender.hpp"
#include "../model/cEulerWorld.hpp"
#include "GL/glew.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include "GL/glut.h"
#endif

#include "camera/Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

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
    mColorLocation = glGetUniformLocation(mShaderProgram, "color");
    if(mProjectionLocation == -1 || mViewLocation == -1 || mModelLocation == -1 | mColorLocation == -1)
    {
        std::cout <<"[error] cEulerRender::Init: get uniform location from glsl error" << std::endl;
    }

    // init color pool
    mColorPool.clear();
    mColorPool.push_back(Eigen::Vector4f(0.5, 0.5, 0.5, 1.0));
    mColorPool.push_back(Eigen::Vector4f(0.1, 0.2, 0.1, 1.0));
    mColorPool.push_back(Eigen::Vector4f(0.2, 0.3, 0.6, 1.0));
    mColorPool.push_back(Eigen::Vector4f(0.5, 0.8, 0.3, 1.0));
    mColorPool.push_back(Eigen::Vector4f(0.7, 0.9, 0.5, 1.0));
    mColorPool.push_back(Eigen::Vector4f(0.8, 0.1, 0.6, 1.0));
}

void cEulerRender::Update()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // std::cout <<"[log] cEulerRender Update" << std::endl;
    if(nullptr == mEulerWorld)
    {
        std::cout <<"[log] cEulerRender: ptr to Eulerworld is empty, return" << std::endl;
        return;
    }
    
    // before set matrix: Eigen is col-major but gl is row-major, so we need to transpose
    
    mProjectMat.transposeInPlace();
    mViewMat.transposeInPlace();
    mModelMat.transposeInPlace();
    
    glm::mat4 view_matrix = mCamera->ViewMatrix();
    glm::mat4 proj_matrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
    
    glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, glm::value_ptr(proj_matrix));
    glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, mModelMat.data());

    DrawVertex();
    DrawEdge();
    DrawFace();

}
void cEulerRender::KeyBoardEvent(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': mCamera->moveForward();break;
    case 'a': mCamera->moveLeft();break;
    case 's': mCamera->moveBackward();break;
    case 'd': mCamera->moveRight();break;
    case 'z': mCamera->moveDown();break;
    case 'x': mCamera->moveUp();break;
    default: break;
    }
}
void cEulerRender::MouseMoveEvent(int x, int y)
{
    // std::cout <<"[log] cEulerRender::MouseMove Event "<< x <<" " << y << std::endl;
    mCamera->mouseMove(x, y);
}

void cEulerRender::MouseEvent(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // std::cout <<"[log] cEulerRender::MouseEvent: left button push down, reset camera <x,y>" << std::endl;
        mCamera->setFirstPos(x, y);
    }
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

void cEulerRender::DrawLoop(cLoop * cur_loop)
{
    std::vector<Eigen::Vector3d> vertice_lst;
    cHalfEdge * first_he = cur_loop->mFirstHalfEdge;
    cHalfEdge * cur_he = first_he;
    do{
        vertice_lst.push_back(cur_he->mOriVertex->mPos);
        cur_he = cur_he->mNextHF;
    }while(first_he != cur_he);
    
    // std::cout <<"vertices num = " << vertice_lst.size() << std::endl;
    
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 创建VBO并且写入数据
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  // 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
    // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertice_lst.size() * 3, vertice_lst[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    
    Eigen::Vector4f color = mColorPool[0];
    glUniform4f(mColorLocation, color[0], color[1], color[2], color[3]);

    #ifdef __APPLE__
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertice_lst.size());  
    #else
        glDrawArrays(GL_POLYGON, 0, vertice_lst.size()); 
    #endif
}

void cEulerRender::DrawFace()
{
    glEnable(GL_STENCIL_TEST);
    glUseProgram(mShaderProgram);
    std::vector<cFace *> face_list = mEulerWorld->GetFaceList();
    for(int i=0; i< face_list.size(); i++)
    {
        cFace * cur_face = face_list[i];
        // std::cout <<"[log] draw face " << i << std::endl;
        // 绘制内环 
        cLoop * outer_loop = cur_face->mFirstLoop;
        cLoop * inner_loop = nullptr;
        if(outer_loop->mNextLoop != nullptr) inner_loop = outer_loop->mNextLoop;
        else continue;
        while(inner_loop)
        {
            /*
                glstencilMask(0xFF;// 允许所有位置模板缓冲写入
glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // 禁止修改颜色缓存
glDepthMask(GL_FALSE); // 禁止修改深度缓存（存疑），但应该是这样，因为我们当他不存在。
glStencilFunc(GL_NEVER, 1, 0xFF); // 永远测试失败，然后就要写入1
glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP) // 对于深度缓存失败的，要替换掉(改写为1)
            */
            glStencilMask(0xFF);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            glStencilFunc(GL_NEVER, 0x01, 0xFF);
            glStencilOp(GL_ONE, GL_KEEP, GL_KEEP);
            DrawLoop(inner_loop);
            inner_loop = inner_loop->mNextLoop;
        }
        
        // 绘制外环
        /*
禁止所有位置模板缓冲写入
允许修改颜色缓存
允许修改深度缓存。（存疑，但应该是必要的，现实存在的面必须参加深度测试）
glStencilFunc(GL_EQUAL, 0, 0xFF); // 1的地方要失败，0的地方要成功，就是等于0
glStencilOp(GL_Keep, gl_keep, gl_keep);
        */
        
        
        glStencilMask(0x00);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // 我发现这里即使是never, 他该画还是画
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        // 内环究竟绘制了没有？通过在这里按continue，我发现内环并没有出现，也就是说内环的确没有绘制。
        // 所以，中间那部分其实还是外环画上去的。通过只画2个面，我发现。
        // 那么错误有2种可能，一是模板测试没有做好(缺省值可能不对)
        // continue;
        DrawLoop(outer_loop);

    }
    
    // glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

void cEulerRender::DrawEdge()
{
    // set up vertices array for edges
    std::vector<cEdge *> edge_list = mEulerWorld->GetEdgeList();
    int num_edge = edge_list.size(), num_vertices = 2 * num_edge;// the number of points = 2 * the number of edges
    double * vertices_lst = new double[num_vertices * 3];
    for(int i=0; i<num_edge; i++)
    {
        // get 2 end pts of this edge
        cHalfEdge * cur_he = edge_list[i]->mHalfEdge1;
        memcpy(vertices_lst + i * 6, cur_he->mOriVertex->mPos.data(), sizeof(double) * 3);
        memcpy(vertices_lst + i * 6 + 3, cur_he->mDestVertex->mPos.data(), sizeof(double) * 3);

        // check memcpy
        // for(int j=0; j<3; j++)
        // {
        //     std::cout << (vertices_lst + i * 6 + 3)[j] <<" " << cur_he->mDestVertex->mPos.data()[j] << std::endl;
        // }
    }

    glUseProgram(mShaderProgram);
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 创建VBO并且写入数据
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  // 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
    // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * num_vertices * 3, vertices_lst, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    glLineWidth(2.0);
    Eigen::Vector4f color = mColorPool[1];
    glUniform4f(mColorLocation, color[0], color[1], color[2], color[3]);

    glDrawArrays(GL_LINES, 0, num_vertices);
}

void cEulerRender::DrawVertex()
{
    std::vector<cVertex *> vertex_list = mEulerWorld->GetVertexList();
    double * vertices = new double[vertex_list.size() * 3];
    // std::cout <<"[log] Draw vertex: " << vertex_list.size() << std::endl;
    for(int i=0; i<vertex_list.size(); i++)
    {
        memcpy(vertices + i * 3, vertex_list[i]->mPos.data(), sizeof(double) * 3);
    }

    glUseProgram(mShaderProgram);
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 创建VBO并且写入数据
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  // 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
    // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertex_list.size() * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, vertex_list.size());
}
    