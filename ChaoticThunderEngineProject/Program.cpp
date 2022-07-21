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


void SquareMesh(float width, float height, int xpart, int ypart, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {
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

void MeshCube(float sidelength, int partitions, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {
    //Remember to not redundantly define same vertices multiple times, hence reuse edge vertices
    vertices_count = (partitions + 1) * partitions * 4 * 3; //front, right, back, left
    //vertices_count += (partitions - 1) * (partitions - 1) * 2; //top, bottom
    vertices = new float[vertices_count * 3];
    
    //indices_count = partitions * partitions * 6 * 2 * 3; //n * n squares per side (* 6) each consisting of two triangles (* 2) of three indices each (* 3)
    indices_count = partitions * partitions * 4 * 2 * 3;
    indices = new unsigned int[indices_count];

    float half_side = sidelength / 2.0;
    float stepsize = sidelength / partitions;
    int current_index_iter = 0;

    for (int side = 0; side < 4; side++) {
        float current_row = -half_side;
        for (int row = 0; row <= partitions; row++) {
            float current_col = -half_side;
            for (int col = 0; col < partitions; col++) {
                int offset_side = (side * (partitions + 1) * partitions);
                int offset_row = (row * partitions);
                float x = 0, y = 0, z = 0;
                switch (side) {
                case 0: //front
                    x = current_col;
                    y = current_row;
                    z = half_side;
                    break;
                case 1: //right
                    x = half_side;
                    y = current_row;
                    z = -current_col;
                    break;
                case 2: //back
                    x = -current_col;
                    y = current_row;
                    z = -half_side;
                    break;
                case 3: //left
                    x = -half_side;
                    y = current_row;
                    z = current_col;
                    break;
                }
                vertices[(offset_side + offset_row + col) * 3 + 0] = x;
                vertices[(offset_side + offset_row + col) * 3 + 1] = y;
                vertices[(offset_side + offset_row + col) * 3 + 2] = z;

                if (row < partitions) {
                    int offset_row_next = ((row + 1) * partitions);
                    if (col == partitions - 1) {
                        int offset_side_next = ((side + 1) % 4) * (partitions + 1) * partitions;
                        indices[current_index_iter++] = offset_side      + offset_row      + col;
                        indices[current_index_iter++] = offset_side_next + offset_row      + 0;
                        indices[current_index_iter++] = offset_side      + offset_row_next + col;
                        indices[current_index_iter++] = offset_side_next + offset_row      + 0;
                        indices[current_index_iter++] = offset_side_next + offset_row_next + 0;
                        indices[current_index_iter++] = offset_side      + offset_row_next + col;
                    }
                    else {
                        indices[current_index_iter++] = offset_side + offset_row      + col;
                        indices[current_index_iter++] = offset_side + offset_row      + col + 1;
                        indices[current_index_iter++] = offset_side + offset_row_next + col;
                        indices[current_index_iter++] = offset_side + offset_row      + col + 1;
                        indices[current_index_iter++] = offset_side + offset_row_next + col + 1;
                        indices[current_index_iter++] = offset_side + offset_row_next + col;
                    }
                }
                current_col += stepsize;
            }
            current_row += stepsize;
        }
    }

    while (current_index_iter < indices_count) {
        indices[current_index_iter++] = 0;
    }

    //Map points on cube mesh to sphere:
    //x' = x * sqrt(1.0f - y*y / 2.0f - z*z / 2.0f + y*y*z*z / 3.0)
    //y' = y * sqrt(1.0f - z*z / 2.0f - x*x / 2.0f + z*z*x*x / 3.0)
    //z' = z * sqrt(1.0f - x*x / 2.0f - y*y / 2.0f + x*x*y*y / 3.0)
}

//TODO: mpofmz
void Cube(float width, float height, float depth, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {

    vertices_count = 8 * 3;
    indices_count = 12 * 3;

    vertices = new float[vertices_count];
    indices = new unsigned int[indices_count];
    float halfwidth = width * 0.5f;
    float halfheight = height * 0.5f;
    float halfdepth = depth * 0.5f;

    int i = 0;
    int j = 0;

    //front                   
    // x // y // z //   -  // -x = left // -y = down // -z = away from camera // 0.0 = center world     

    vertices[i++] = -halfwidth; vertices[i++] = halfheight; vertices[i++] = halfdepth; //0  
    vertices[i++] = halfwidth; vertices[i++] = halfheight; vertices[i++] = halfdepth; //1
    vertices[i++] = -halfwidth; vertices[i++] = -halfheight; vertices[i++] = halfdepth; //2
    vertices[i++] = halfwidth; vertices[i++] = -halfheight; vertices[i++] = halfdepth; //3

    //back  
    vertices[i++] = -halfwidth; vertices[i++] = halfheight; vertices[i++] = -halfdepth; //4
    vertices[i++] = halfwidth; vertices[i++] = halfheight; vertices[i++] = -halfdepth; //5
    vertices[i++] = -halfwidth; vertices[i++] = -halfheight; vertices[i++] = -halfdepth; //6
    vertices[i++] = halfwidth; vertices[i++] = -halfheight; vertices[i++] = -halfdepth; //7

    //front
    indices[j++] = 0; indices[j++] = 1; indices[j++] = 2; //front left
    indices[j++] = 1; indices[j++] = 3; indices[j++] = 2; //front right

    //right
    indices[j++] = 1; indices[j++] = 5; indices[j++] = 3; //right left
    indices[j++] = 5; indices[j++] = 7; indices[j++] = 3; //right right
  
    //back
    indices[j++] = 5; indices[j++] = 4; indices[j++] = 7; //front left
    indices[j++] = 4; indices[j++] = 6; indices[j++] = 7; //front right

    //left
    indices[j++] = 4; indices[j++] = 0; indices[j++] = 6; //right left
    indices[j++] = 0; indices[j++] = 2; indices[j++] = 6; //right right

    //top
    indices[j++] = 4; indices[j++] = 5; indices[j++] = 0; //front left
    indices[j++] = 5; indices[j++] = 1; indices[j++] = 0; //front right

    //bottom
    indices[j++] = 2; indices[j++] = 3; indices[j++] = 6; //right left
    indices[j++] = 3; indices[j++] = 7; indices[j++] = 6; //right right
}

void EquilateralMesh(float width, float height, float triangle_side, unsigned int*& indices, unsigned int& indices_count, float*& vertices, unsigned int& vertices_count) {
    
    int xpart = (int)((width) / triangle_side);
    int ypart = (int)((height) / triangle_side);
    
    vertices_count = 3 * (xpart + 1) * (ypart + 1);
    indices_count = 6 * xpart * ypart;

    vertices = new float[vertices_count]; // array coordinates times 3 so that each position stores one coordinate axis
    indices = new unsigned int[indices_count]; // indices for 2 triangles for every 4 vertices
    float halfwidth = width * 0.5f;
    float currentx = -halfwidth;
    float currenty = -height * 0.5f;
    float stepsizex = triangle_side;
    float stepsizey = std::sqrt( triangle_side * triangle_side - (triangle_side * 0.5) * (triangle_side * 0.5) );

    for (int i = 0; i <= ypart; i++) //vertex generation
    {
        currentx = -halfwidth;
        float offset = i % 2 == 0 ? triangle_side * 0.5 : 0;
        for (int j = 0; j <= xpart; j++)
        {
            vertices[3 * (i * (xpart + 1) + j)] = currentx + offset;
            vertices[3 * (i * (xpart + 1) + j) + 1] = currenty;
            vertices[3 * (i * (xpart + 1) + j) + 2] = 0.0f;
            currentx += stepsizex;
        }
        currenty += stepsizey;
    }

    for (int i = 0; i < ypart; i++)
    {
        for (int j = 0; j < xpart; j++) {
            indices[6 * (i * xpart + j)]     = i % 2 == 0 ? i       * (xpart + 1) + j     : i       * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 1] = i % 2 == 0 ? i       * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 2] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j;
            indices[6 * (i * xpart + j) + 3] = i % 2 == 0 ? i       * (xpart + 1) + j     : i       * (xpart + 1) + j;
            indices[6 * (i * xpart + j) + 4] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : i       * (xpart + 1) + j + 1;
            indices[6 * (i * xpart + j) + 5] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j     : (i + 1) * (xpart + 1) + j;
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

float lastX = .0f, lastY = .0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessMouse(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Controller::Instance()->GetContextWindow()->GetActiveCamera()->ProcessKeyboard(RIGHT, deltaTime);
    */
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
    EquilateralMesh(0.5, 0.5, 0.02, indices, indices_count, vertices, vertices_count);
    //Cube(5.0, 2.0, 2.0, indices, indices_count, vertices, vertices_count);
    //MeshCube(2, 40, indices, indices_count, vertices, vertices_count);

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
    VertexDataBuffer* datamainbuffer = arraymainbuffer->CreateVertexBuffer(sizeof(float) * vertices_count, vertices);
    VertexIndexBuffer* indexmainbuffer = datamainbuffer->CreateIndexBuffer(sizeof(unsigned int) * indices_count, indices);
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
        processInput(window->GetGLContext());

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
