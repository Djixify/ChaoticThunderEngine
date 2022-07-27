#include <iostream>
#include <fstream>
#include <numbers>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <cmath>
#include "Debug.hpp"
#include "File.hpp"
#include "Controller.hpp"
#include "imgui.h"
#include "Mesh.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Graphics.hpp"

#define PI 3.14159265358979323846

#if GL_ES
precision mediump float;
#endif

void TestFunction() {


    // call a function in another file
    std::cout << "Hello other world!" << std::endl;

    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 9001, 42, 23.2);
    Debug::Logger::Console(Debug::Level::INFO, "%10d ::: %-15d ::: %10f", 69, 360, 1000.2);

    std::string currentPath = File::CurrentDirectory();
    std::string parent = File::GetParent(currentPath);
    std::string file = "Somefile.uwu";
    std::string firstuwu = File::CombinePath(2, currentPath, file);

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

    logger.Log(Debug::Level::INFO, "Im your execution path!: %s", File::CurrentDirectory());

    printf("%d\n", logger.IsValid());

    filestream.close();

    printf("%d\n", logger.IsValid());
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, const char* argv[]) {

    TestFunction();

    glfwSetErrorCallback(glfw_error_callback);

    Controller::Init();

    Debug::SetLogLevels(511 - (int)Debug::Level::CONTEXT);

    Window mainwindow("Main window", 800, 800);
    //Window secondarywindow("Secondary window", 400, 400, &mainwindow);

    try {
        Controller::Instance()->AddWindow(&mainwindow);
        //Controller::Instance()->AddWindow(&secondarywindow);
        // get version info
        const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString(GL_VERSION); // version as a string
        const GLubyte* vendor = glGetString(GL_VENDOR); // vecndor as a string
        const GLubyte* GLSLversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version of GLSL as a string
        Debug::Logger::Console(Debug::Level::INFO, "Renderer: %s", renderer);
        Debug::Logger::Console(Debug::Level::INFO, "OpenGL version supported: %s", version);
        Debug::Logger::Console(Debug::Level::INFO, "Vendor of GPU: %s", vendor);
        Debug::Logger::Console(Debug::Level::INFO, "GLSL version: %s", GLSLversion);
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        Debug::Logger::Console(Debug::Level::INFO, "Maximum nr of vertex attributes supported: %d", nrAttributes);
    }
    catch (const char* ex) {
        std::cerr << ex << std::endl;
        glfwTerminate();
        return 0;
    }

    unsigned int indices_count = 0, vertices_count = 0;
#define SIMPLE false
#if SIMPLE
    float vertices[] = {
         0.0f,  0.7f, -0.5f,  // top center
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  // top left 
         0.0f, -0.7f, 0.0f   // bottom center
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 4,   // first triangle
        1, 2, 3,   // second triangle
        3, 4, 1,    // third triangle
        2, 3, 5
    };

    vertices_count = sizeof(vertices) / 4;
    indices_count = sizeof(indices) / 4;
#else
    float* vertices = 0;
    unsigned int* indices = 0;

    //MakeNgon(6, 0.5f, 0, 0, indices, indices_count, vertices, vertices_count);
    //SquareMesh(2.0, 2.0, 100, 100, indices, indices_count, vertices, vertices_count);
    //EquilateralMesh(0.5, 0.5, 0.02, indices, indices_count, vertices, vertices_count);
    //Cube(5.0, 2.0, 2.0, indices, indices_count, vertices, vertices_count);
    //MeshCube(2, 40, indices, indices_count, vertices, vertices_count);
    Mesh* mesh = Mesh::Sphere(50, 50, 30);

    //Debugging mesh information:
    if (vertices_count + indices_count < 100) {
        for (int i = 0; i < vertices_count / 3; i++) {
            Debug::Logger::Console(Debug::Level::INFO, "Vertex %d: %f, %f, %f", i, vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
        }
        for (int i = 0; i < indices_count / 3; i++) {
            Debug::Logger::Console(Debug::Level::INFO, "Triangle %d: %d, %d, %d", i, indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]);
        }
    }
#endif

    //Load shaders into main window
    std::string fragmentshaderfolder = "shaderprograms";
    std::string circlepatternfragment = "circlepattern.frag";
    std::string scalingvertex = "scaling.vert";
    std::string cameravertex = "camera.vert";
    std::string sinwavevertex = "sinwave.vert";
    std::string redtrianglefragment = "red.frag";
    std::string bluetrianglefragment = "oscillatingblue.frag"; 
    std::string greentrianglefragment = "green.frag";

    std::string shaderfolder = File::CombinePath(2, File::CurrentDirectory(), fragmentshaderfolder);
    //load_shader circleshaderinfo{ FRAGMENT, FileUtility::CombinePath(2, shaderfolder, circlepatternfragment) };
    //Shader circleShader(secondarywindow, 1, circleshaderinfo);
    load_shader scalingvertexinfo{ shader_type::VERTEX, File::CombinePath(2, shaderfolder, scalingvertex) };
    load_shader cameravertexinfo{ shader_type::VERTEX, File::CombinePath(2, shaderfolder, cameravertex) };
    load_shader sinwavevertexinfo{ shader_type::VERTEX, File::CombinePath(2, shaderfolder, sinwavevertex) };

    load_shader redfragmentinfo{ shader_type::FRAGMENT, File::CombinePath(2, shaderfolder, redtrianglefragment) };
    load_shader bluefragmentinfo{ shader_type::FRAGMENT, File::CombinePath(2, shaderfolder, bluetrianglefragment) };
    load_shader greenfragmentinfo{ shader_type::FRAGMENT, File::CombinePath(2, shaderfolder, greentrianglefragment) };

    mainwindow.AddShader("green", 2, cameravertexinfo, greenfragmentinfo);
    mainwindow.AddShader("red", 2, sinwavevertexinfo, redfragmentinfo);
    mainwindow.AddShader("blue", 2, scalingvertexinfo, bluefragmentinfo);

    ArrayBuffer* arraymainbuffer = mainwindow.AddArrayBuffer("positions");
    VertexDataBuffer* datamainbuffer = arraymainbuffer->CreateVertexBuffer(sizeof(float) * mesh->_vertices.size(), &mesh->_vertices[0]);
    VertexIndexBuffer* indexmainbuffer = datamainbuffer->CreateIndexBuffer(sizeof(unsigned int) * mesh->_indices.size(), &mesh->_indices[0]);
    arraymainbuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false); //Positions
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // Closest object to the camera will be drawn

    //Initialize ImGUI (for parameter testing in window)
    Graphics::InitializeImGUI(Controller::Instance()->GetContextWindow());

    bool shouldClose = false;
    while (!shouldClose)
    {
        float currentFrame = static_cast<float>(glfwGetTime());

        //Debug::Logger::console("Displaying window " + i);
        Window* window = Controller::Instance()->GetContextWindow();

        window->Clear();
        window->UpdateTime();
        window->ProcessKeyContinuous();
        Graphics::UpdateVariablesImGUI(window);

        float constant_timemultiplier = 1.0f;
        //window->GetActiveCamera()->Position.y = glm::cos(window->GetCurrentTimeSec() * constant_timemultiplier);
        //window->GetActiveCamera()->Position.z = glm::sin(window->GetCurrentTimeSec() * constant_timemultiplier);

        //window->GetActiveCamera()->Front = -window->GetActiveCamera()->Position;

        indexmainbuffer->Draw();

        Graphics::RenderImGUI(window);

        glfwSwapBuffers(window->GetGLContext());
        glfwPollEvents();

        shouldClose |= glfwWindowShouldClose(window->GetGLContext());
    }

    Graphics::TerminateImGUI();

    glfwTerminate();
}
