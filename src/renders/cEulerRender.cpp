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

// void draw_circle(int mShaderProgram)
// {
//     float circle[] = {
//         1.5, 0, 0,
//         0, 1.5, 0,
//         -1.5, 0, 0,
//         0, -1.5, 0,
//     };

//     glUseProgram(mShaderProgram);
//     // VAO
//     unsigned int VAO;
//     glGenVertexArrays(1, &VAO);

//     // 创建VBO并且写入数据
//     unsigned int VBO;
//     glBindVertexArray(VAO);
//     glGenBuffers(1, &VBO);  // 生成VBO
//     glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
//     // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
//     glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//     glEnableVertexAttribArray(0);
    
//     // render
//     glBindVertexArray(VAO);
//     glPointSize(4.0f);
//     glDrawArrays(GL_POLYGON, 0, 4);

// }

// void draw_scene(int mShaderProgram)
// {
//     float circle[] = {
//         1, 1, 0,
//         -1, 1, 0,
//         -1, -1, 0,
//         1, -1, 0,
//     };

//     glUseProgram(mShaderProgram);
//     // VAO
//     unsigned int VAO;
//     glGenVertexArrays(1, &VAO);

//     // 创建VBO并且写入数据
//     unsigned int VBO;
//     glBindVertexArray(VAO);
//     glGenBuffers(1, &VBO);  // 生成VBO
//     glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
//     // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
//     glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//     glEnableVertexAttribArray(0);
    
//     // render
//     glBindVertexArray(VAO);
//     glPointSize(4.0f);

    
//     glDrawArrays(GL_POLYGON, 0, 4);
// }

// void DrawTest(int shader)
// {
//     glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//     glDepthMask(GL_FALSE);
//     glStencilFunc(GL_NEVER, 1, 0xFF);
//     glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

//     // draw stencil pattern
//     glStencilMask(0xFF);
//     glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
  
//     draw_circle(shader);

//     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//     glDepthMask(GL_TRUE);
//     glStencilMask(0x00);
//     // draw only where stencil's value is 1
//     glStencilFunc(GL_EQUAL, 1, 0xFF);

//     draw_scene(shader);

//     // glDisable(GL_STENCIL_TEST);
    
// }

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

    // DrawTest(mShaderProgram);
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
    if(state == GLUT_DOWN)
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

void cEulerRender::DrawInnerLoop(cLoop * cur_loop)
{
    std::vector<Eigen::Vector3f> vertice_lst;
    cHalfEdge * first_he = cur_loop->mFirstHalfEdge;
    cHalfEdge * cur_he = first_he;
    do{
        vertice_lst.push_back(Eigen::Vector3f(cur_he->mOriVertex->mPos[0], cur_he->mOriVertex->mPos[1], cur_he->mOriVertex->mPos[2]));
        cur_he = cur_he->mNextHF;
    }while(first_he != cur_he);
    
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // std::cout <<"[log] VAO " << VAO <<std::endl;
    // 创建VBO并且写入数据
    unsigned int VBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  // 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
    // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertice_lst.size() * 3, vertice_lst[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilMask(0xFF);


    Eigen::Vector4f color = mColorPool[1];
    glUniform4f(mColorLocation, color[0], color[1], color[2], color[3]);

    #ifdef __APPLE__
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertice_lst.size());  
    #else
        glDrawArrays(GL_POLYGON, 0, vertice_lst.size()); 
    #endif

    glBindVertexArray(0);
}

void cEulerRender::DrawOuterLoop(cLoop * cur_loop)
{
    // std::cout <<"[log] draw outer loop" <<std::endl;
    std::vector<Eigen::Vector3f> vertice_lst;
    cHalfEdge * first_he = cur_loop->mFirstHalfEdge;
    cHalfEdge * cur_he = first_he;
    do{
        vertice_lst.push_back(Eigen::Vector3f(cur_he->mOriVertex->mPos[0], cur_he->mOriVertex->mPos[1], cur_he->mOriVertex->mPos[2]));
        cur_he = cur_he->mNextHF;
    }while(first_he != cur_he);
    
    // std::cout <<"vertices num = " << vertice_lst.size() << std::endl;
    
    if(cur_loop->VAO == -1)
    {
            // VAO
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        // std::cout <<"[log] VAO " << VAO <<std::endl;
        // 创建VBO并且写入数据
        unsigned int VBO;
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);  // 生成VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把VBO绑定为vertex buffer的类型，也就是GL_ARRAY_BUFFER。如果想要绑定到别的着色器，那就不选这个目标。
        // std::cout <<"[log] vbo size = " << sizeof(v) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertice_lst.size() * 3, vertice_lst[0].data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        
        // render
        cur_loop->VAO = VAO;
    }
    
    glBindVertexArray(cur_loop->VAO);
    

    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glDepthMask(GL_TRUE);
    
    // glDepthFunc(GL_NEVER);

    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    Eigen::Vector4f color = mColorPool[0];
    glUniform4f(mColorLocation, color[0], color[1], color[2], color[3]);

    #ifdef __APPLE__
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertice_lst.size());  
    #else
        glDrawArrays(GL_POLYGON, 0, vertice_lst.size()); 
    #endif

    glBindVertexArray(0);
    glStencilMask(0xFF);
}

void cEulerRender::DrawFace()
{
    
    std::vector<cFace *> face_list = mEulerWorld->GetFaceList();
    glStencilFunc(GL_NEVER, 1, 0x00);

    for(int i=0; i< face_list.size(); i++)
    {
        cFace * cur_face = face_list[i];
        // std::cout <<"[log] draw face " << i << std::endl;
        // 绘制内环 
        cLoop * outer_loop = cur_face->mFirstLoop;
        cLoop * inner_loop = nullptr;
        if(outer_loop->mNextLoop != nullptr) inner_loop = outer_loop->mNextLoop;
        // else continue;

        // 绘制内环
        while (inner_loop) {
            DrawInnerLoop(inner_loop);
            inner_loop = inner_loop->mNextLoop;
        }

        // 绘制外环
        DrawOuterLoop(outer_loop);
    }
    
    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_STENCIL_TEST);
}

void cEulerRender::DrawEdge()
{
    // set up vertices array for edges
    std::vector<cEdge *> edge_list = mEulerWorld->GetEdgeList();
    int num_edge = edge_list.size(), num_vertices = 2 * num_edge;// the number of points = 2 * the number of edges
    float * vertices_lst = new float[num_vertices * 3];
    for(int i=0; i<num_edge; i++)
    {
        // get 2 end pts of this edge
        cHalfEdge * cur_he = edge_list[i]->mHalfEdge1;
        vertices_lst[6 * i + 0 ] = cur_he->mOriVertex->mPos[0];
        vertices_lst[6 * i + 1 ] = cur_he->mOriVertex->mPos[1];
        vertices_lst[6 * i + 2 ] = cur_he->mOriVertex->mPos[2];
        
        vertices_lst[6 * i + 3 ] = cur_he->mDestVertex->mPos[0];
        vertices_lst[6 * i + 4 ] = cur_he->mDestVertex->mPos[1];
        vertices_lst[6 * i + 5 ] = cur_he->mDestVertex->mPos[2];
        
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices_lst, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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
    float * vertices = new float[vertex_list.size() * 3];
    // std::cout <<"[log] Draw vertex: " << vertex_list.size() << std::endl;
    for(int i=0; i<vertex_list.size(); i++)
    {
        // memcpy(vertices + i * 3, vertex_list[i]->mPos.data(), sizeof(float) * 3);
        vertices[3 * i +0 ] = vertex_list[i]->mPos[0];
        vertices[3 * i +1 ] = vertex_list[i]->mPos[1];
        vertices[3 * i +2 ] = vertex_list[i]->mPos[2];
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_list.size() * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    // render
    glBindVertexArray(VAO);
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, vertex_list.size());
}
    