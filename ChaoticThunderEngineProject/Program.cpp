#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <numbers>
#include <filesystem>
//#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "Debug.hpp"
#include "FileUtility.hpp"
#include "Controller.hpp"

#define PI 3.14159265358979323846

#if GL_ES
precision mediump float;
#endif

void TestFunction() {


    // call a function in another file
    std::cout << "Hello other world!" << std::endl;


    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 9001, 42, 23.2);
    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 69, 360, 1000.2);

    std::string currentPath = FileUtility::CurrentDirectory();
    std::string parent = FileUtility::GetParent(currentPath);
    std::string file = "Somefile.uwu";
    std::string firstuwu = FileUtility::CombinePath(2, currentPath, file);

    std::cout << currentPath << std::endl;
    std::cout << parent << std::endl;
    std::cout << firstuwu << std::endl;

    Debug::Logger* tmp = new Debug::Logger(std::cout);

    //Below is equivalent, *pointer is dereference, &value is reference, type* referes to a pointer of the type 'type'
    //Dereference will take the value that the pointer points to
    //Reference will provide the address for the value 
    int a = 20;
    int* a_ptr = &a;
    int b = *a_ptr;
    printf("a=%d, addres of a: %p, addres of a_ptr: %p, b=%d, addres of b: %p\n", a, &a, &a_ptr, b, &b);

    tmp->Log(Debug::Level::INFO, "Test");
    (*tmp).Log(Debug::Level::INFO, "Test");

    std::ofstream filestream("Test.log"); //huh, no path required...
    Debug::Logger logger(filestream);

    logger.Log(Debug::Level::INFO, "Hello from the log! 2");

    logger.Log(Debug::Level::INFO, "Im a title", "Hello from the log! 3");

    logger.Log(Debug::Level::INFO, "Im your execution path!: %s", FileUtility::CurrentDirectory());

    printf("%d\n", logger.IsValid());

    filestream.close();

    printf("%d\n", logger.IsValid());
}


void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WindowSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, const char* argv[]) {

    TestFunction();

    if (!glfwInit()) {
        return -1;
    }


    std::string fragmentshaderfolder = "fragmentshaders";
    std::string circlepatternfragment = "circlepattern.frag";
    std::string trianglevertex = "triangle.vert";
    std::string trianglefragment = "triangle.frag";

    Window mainwindow("Main window", 400, 400);
    Window secondarywindow("Secondary window", 400, 400);

    try {
        Controller::Instance()->AddWindow(&mainwindow);
        //Controller::Instance()->AddWindow(&secondarywindow);
        // get version info
        const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString(GL_VERSION); // version as a string
        printf("Renderer: %s\n", renderer);
        printf("OpenGL version supported %s\n", version);
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    }
    catch (const char* ex) {
        std::cerr << ex << std::endl;
        glfwTerminate();
        return 0;
    }

#define SIMPLE false
#if SIMPLE
    float vertices[] = {
        -1.0f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f
    };
    unsigned int vertex_indices1[] = {
        0, 1, 2,
        0, 1, 4
    };
    unsigned int vertex_indices2[] = {
        0, 1, 2,
        0, 1, 3
    };
#else
    float vertices[3 * 25];
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            vertices[(y * 5 + x) * 3 + 0] = ((float)x - 2.f) / 2.f;
            vertices[(y * 5 + x) * 3 + 1] = ((float)y - 2.f) / 2.f;
            vertices[(y * 5 + x) * 3 + 2] = 0.f;
            Debug::Logger::Console(Debug::Level::INFO, "%d: (%f, %f, %f)", (y * 5 + x), vertices[(y * 5 + x) * 3 + 0], vertices[(y * 5 + x) * 3 + 1], vertices[(y * 5 + x) * 3 + 2]);
        }
    }
    unsigned int vertex_indices1[] = {
        6, 16, 12,
        12, 8, 18
    };
    unsigned int vertex_indices2[] = {
        6, 8, 12,
        12, 16, 18
    };
#endif

    std::string shaderfolder = FileUtility::CombinePath(2, FileUtility::CurrentDirectory(), fragmentshaderfolder);
    load_shader circleshaderinfo{ FRAGMENT, FileUtility::CombinePath(2, shaderfolder, circlepatternfragment) };
    Shader circleShader(secondarywindow, 1, circleshaderinfo);
    load_shader trianglevertexinfo{ VERTEX, FileUtility::CombinePath(2, shaderfolder, trianglevertex) };
    load_shader trianglefragmentinfo{ FRAGMENT, FileUtility::CombinePath(2, shaderfolder, trianglefragment) };
    Shader triangleshader(mainwindow, 2, trianglevertexinfo, trianglefragmentinfo);

    Controller::Instance()->FirstWindow()->AddShader(triangleshader);
    ArrayBuffer* arraymainbuffer = triangleshader.AddArrayBuffer("positions");
    arraymainbuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
    VertexDataBuffer* datamainbuffer = arraymainbuffer->CreateVertexBuffer(sizeof(vertices) * 4, vertices);
    VertexIndexBuffer* indexmainbuffer = datamainbuffer->CreateIndexBuffer(sizeof(vertex_indices1), vertex_indices1);


    int counter = 0;
    bool shouldClose = false;
    while (!shouldClose)
    {
        for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
            //Debug::Logger::console("Displaying window " + i);
            ProcessInput(Controller::Instance()->GetContextWindow()->GetGLContext());

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            triangleshader.Use();
            indexmainbuffer->SetActive();
            indexmainbuffer->Draw();

            glfwSwapBuffers(Controller::Instance()->GetContextWindow()->GetGLContext());
            counter++;
            glfwPollEvents();
        }

        //Debug::Logger::console("%d", Controller::Instance()->GetWindowCount());

        for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++)
            shouldClose |= glfwWindowShouldClose(Controller::Instance()->GetWindow(i)->GetGLContext());
        if (shouldClose)
            break;
    }

    glfwTerminate();
}