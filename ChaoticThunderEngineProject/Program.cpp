#include <iostream>
#include <fstream>
#include <numbers>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "Debug.hpp"
#include "File.hpp"
#include "Controller.hpp"
#include "imgui.h"
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


void Mesh(float width, float height, int xpart, int ypart, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {
    vertices_count = 3 * (xpart + 1) * (ypart + 1);
    indices_count  = 6 * xpart * ypart;
    
    vertices = new float[vertices_count]; // array coordinates times 3 so that each position stores one coordinate axis
    indices  = new unsigned int[indices_count]; // indices for 2 triangles for every 4 vertices
    float halfwidth = width * 0.5f;
    float currentx  = -halfwidth;
    float currenty  = -height * 0.5f;
    float stepsizex = width / xpart;
    float stepsizey = height / ypart;

    for (int i = 0; i <= ypart; i++) //vertex generation
    {
        currentx = -halfwidth;
        for (int j = 0; j <= xpart; j++)
        {
            vertices[3 * (i * (xpart + 1) + j)]     = currentx;
            vertices[3 * (i * (xpart + 1) + j) + 1] = currenty;
            vertices[3 * (i * (xpart + 1) + j) + 2] = 0.0f;
            currentx += stepsizex;
        }
        currenty += stepsizey;
    }   

    for (int i = 0; i < ypart; i++) 
    {
        for (int j = 0; j < xpart; j++) {
            indices[6 * (i * xpart + j)]     = i       * (xpart + 1) + j;
            indices[6 * (i * xpart + j) + 1] = i       * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 2] = (i + 1) * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 3] = i       * (xpart + 1) + j;
            indices[6 * (i * xpart + j) + 4] = (i + 1) * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 5] = (i + 1) * (xpart + 1) + j;
        }
    }
}

void MakeNgon(int input, float radius, float x, float y, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {
    vertices_count = (input + 1) * 3; // n + 1 vertices (introducing center vertex)
    indices_count = input * 3; // n triangles
    
    vertices = new float[vertices_count];
    indices = new unsigned int[indices_count];
    float current = 0; 
    float stepsize = (2 * PI) / input;
    vertices[0] = x + 0.0f;
    vertices[1] = y + 0.0f;
    vertices[2] = 0.0f;
    for (int i = 1; i <= input; i++)
    {
        //Ignore warning, false positive
        vertices[3 * i]     = x + radius * sin(current);
        vertices[3 * i + 1] = y + radius * cos(current);
        vertices[3 * i + 2] = 0.0f;

        indices[3 * (i - 1)]     = 0;
        indices[3 * (i - 1) + 1] = i;
        indices[3 * (i - 1) + 2] = i + 1;

        current += stepsize;
    }
    indices[3 * (input - 1) + 2] = 1;
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

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, const char* argv[]) {

    TestFunction();

    glfwSetErrorCallback(glfw_error_callback);

    Controller::Init();

    std::ofstream filestream("Test2.log"); //huh, no path required...
    Debug::defaultoutstream = &filestream;
    Debug::Logger::Console(Debug::Level::INFO, "Test from static override");

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
        printf("Renderer: %s\n", renderer);
        printf("OpenGL version supported: %s\n", version);
        printf("Vendor of GPU: %s\n", vendor);
        printf("GLSL version: %s\n", GLSLversion);
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
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
     0.0f,  0.7f, 0.0f,  // top center
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
    int n = sizeof(indices)/3;

    vertices_count = sizeof(vertices);
    indices_count = sizeof(indices);
#else
    float* vertices = 0;
    unsigned int* indices = 0;

    //MakeNgon(n, 0.5f, 0, 0, indices, indices_count, vertices, vertices_count);
    Mesh(0.5, 0.5, 5, 3, indices, indices_count, vertices, vertices_count);
    int n = indices_count / 3;

    //Debugging mesh information:
    for (int i = 0; i < vertices_count / 3; i++) {
        Debug::Logger::Console(Debug::Level::INFO, "Vertex %d: %f, %f, %f", i, vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
    }
    for (int i = 0; i < indices_count / 3; i++) {
        Debug::Logger::Console(Debug::Level::INFO, "Triangle %d: %d, %d, %d", i, indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]);
    }
#endif

    //Load shaders into main window
    std::string fragmentshaderfolder = "fragmentshaders";
    std::string circlepatternfragment = "circlepattern.frag";
    std::string trianglevertex = "triangle.vert";
    std::string trianglefragment = "triangle.frag";

    std::string shaderfolder = File::CombinePath(2, File::CurrentDirectory(), fragmentshaderfolder);
    //load_shader circleshaderinfo{ FRAGMENT, FileUtility::CombinePath(2, shaderfolder, circlepatternfragment) };
    //Shader circleShader(secondarywindow, 1, circleshaderinfo);
    load_shader trianglevertexinfo{ VERTEX, File::CombinePath(2, shaderfolder, trianglevertex) };
    load_shader trianglefragmentinfo{ FRAGMENT, File::CombinePath(2, shaderfolder, trianglefragment) };
    Shader triangleshader(mainwindow, 2, trianglevertexinfo, trianglefragmentinfo);

    mainwindow.AddShader(triangleshader);
    ArrayBuffer* arraymainbuffer = triangleshader.AddArrayBuffer("positions");
    VertexDataBuffer* datamainbuffer = arraymainbuffer->CreateVertexBuffer(sizeof(float) * vertices_count, vertices);
    VertexIndexBuffer* indexmainbuffer = datamainbuffer->CreateIndexBuffer(sizeof(unsigned int) * indices_count, indices);
    arraymainbuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);


    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    
    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*(n+1) , vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*3*n, indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    GLuint shader_programme = triangleshader.GetID();
    //TEST

    bool shouldClose = false;
    glViewport(0, 0, 400, 400);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // Closest object to the camera will be dra


    //Initialize ImGUI (for parameter testing in window)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(mainwindow.GetGLContext(), true);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!shouldClose)
    {
        for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++) {
            //Debug::Logger::console("Displaying window " + i);
            GLFWwindow* context = Controller::Instance()->GetContextWindow()->GetGLContext();
            ProcessInput(context);

            Graphics::ClearWindow(context);

            //triangleshader.Use();
            //indexmainbuffer->SetActive();
            //indexmainbuffer->Draw();

            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /*
            //arraymainbuffer->SetActive();
            //datamainbuffer->SetActive();
            //arraymainbuffer->SetAttribute(0, 0, attribute_type::INT8, false);
            //datamainbuffer->Draw();
            glBindVertexArray(arraymainbuffer->GetID());
            //indexmainbuffer->SetActive();
            // draw points 0-3 from the currently bound VAO with current in-use shader
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            */


            glUseProgram(shader_programme);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, n*3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            Graphics::RenderImGUI(context);

            glfwSwapBuffers(context);
            glfwPollEvents();
        }


        //Debug::Logger::console("%d", Controller::Instance()->GetWindowCount());

        for (int i = 0; i < Controller::Instance()->GetWindowCount(); i++)
            shouldClose |= glfwWindowShouldClose(Controller::Instance()->GetWindow(i)->GetGLContext());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
