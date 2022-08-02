#ifndef OBJECTS_MESH
#define OBJECTS_MESH

#include <vector>
#include <cmath>
#include "glm.hpp"
#include "buffer.hpp"
#define PI 3.14159265358979323846f

class Mesh {
private:
    ArrayBuffer* _arraybuffer;
    VertexDataBuffer* _vertexdatabuffer;
    VertexIndexBuffer* _vertexindexbuffer;
    
    Mesh();
public:
	~Mesh();

    void Draw();

	static Mesh* SquareTriangleMesh(float width, float height, int xpartitions, int ypartitions) {
		Mesh* mesh = new Mesh();

        int vertices_count = 3 * (xpartitions + 1) * (ypartitions + 1);
        int indices_count = 6 * xpartitions * ypartitions;

        float* vertices = new float[vertices_count];
        unsigned int* indices = new unsigned int[indices_count];

        float halfwidth = width * 0.5f;
        float currentx = -halfwidth;
        float currenty = -height * 0.5f;
        float stepsizex = width / xpartitions;
        float stepsizey = height / ypartitions;

        for (int i = 0; i <= ypartitions; i++) //vertex generation
        {
            currentx = -halfwidth;
            for (int j = 0; j <= xpartitions; j++)
            {
                vertices[3 * (i * (xpartitions + 1) + j)]     = currentx;
                vertices[3 * (i * (xpartitions + 1) + j) + 1] = currenty;
                vertices[3 * (i * (xpartitions + 1) + j) + 2] = 0.0f;
                currentx += stepsizex;
            }
            currenty += stepsizey;
        }

        for (int i = 0; i < ypartitions; i++)
        {
            for (int j = 0; j < xpartitions; j++) {
                indices[6 * (i * xpartitions + j)]     = i       * (xpartitions + 1) + j;
                indices[6 * (i * xpartitions + j) + 1] = i       * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 2] = (i + 1) * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 3] = i       * (xpartitions + 1) + j;
                indices[6 * (i * xpartitions + j) + 4] = (i + 1) * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 5] = (i + 1) * (xpartitions + 1) + j;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices_count * sizeof(float), &vertices[0]);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices_count * sizeof(unsigned int), &indices[0]);

        delete[] vertices;
        delete[] indices;

        return mesh;
	}

    static Mesh* SquareTriangleMeshWithNormals(float width, float height, int xpartitions, int ypartitions) {
        Mesh* mesh = new Mesh();

        int vertices_count = 6 * (xpartitions + 1) * (ypartitions + 1);
        int indices_count = 6 * xpartitions * ypartitions;

        float* vertices = new float[vertices_count];
        unsigned int* indices = new unsigned int[indices_count];

        float halfwidth = width * 0.5f;
        float currentx = -halfwidth;
        float currenty = -height * 0.5f;
        float stepsizex = width / xpartitions;
        float stepsizey = height / ypartitions;

        for (int i = 0; i <= ypartitions; i++) //vertex generation
        {
            currentx = -halfwidth;
            for (int j = 0; j <= xpartitions; j++)
            {
                vertices[3 * (i * (xpartitions + 1) + j)] = currentx;
                vertices[3 * (i * (xpartitions + 1) + j) + 1] = currenty;
                vertices[3 * (i * (xpartitions + 1) + j) + 2] = 0.0f;
                vertices[3 * (i * (xpartitions + 1) + j) + 3] = 0;
                vertices[3 * (i * (xpartitions + 1) + j) + 4] = 0;
                vertices[3 * (i * (xpartitions + 1) + j) + 5] = -1;
                currentx += stepsizex;
            }
            currenty += stepsizey;
        }

        for (int i = 0; i < ypartitions; i++)
        {
            for (int j = 0; j < xpartitions; j++) 
            {
                indices[6 * (i * xpartitions + j)] = i * (xpartitions + 1) + j;
                indices[6 * (i * xpartitions + j) + 1] = i * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 2] = (i + 1) * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 3] = i * (xpartitions + 1) + j;
                indices[6 * (i * xpartitions + j) + 4] = (i + 1) * (xpartitions + 1) + j + 1;
                indices[6 * (i * xpartitions + j) + 5] = (i + 1) * (xpartitions + 1) + j;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices_count * sizeof(float), vertices);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_arraybuffer->AddAttribute(1, 3, attribute_type::FLOAT32, true);
        mesh->_vertexindexbuffer->Write(indices_count * sizeof(unsigned int), indices);

        delete[] vertices;
        delete[] indices;

        return mesh;
    }

	static Mesh* EquilateralTriangleMesh(float width, float height, float triangle_side_length) {
		Mesh* mesh = new Mesh();

        int xpart = (int)(width / triangle_side_length);
        int ypart = (int)(height / triangle_side_length);

        int vertices_count = 3 * (xpart + 1) * (ypart + 1);
        int indices_count  = 6 * xpart * ypart;

        float* vertices = new float[vertices_count];
        unsigned int* indices = new unsigned int[indices_count];

        float halfwidth = width * 0.5f;
        float currentx = -halfwidth;
        float currenty = -height * 0.5f;
        float stepsizex = triangle_side_length;
        float stepsizey = std::sqrt(triangle_side_length * triangle_side_length - (triangle_side_length * 0.5f) * (triangle_side_length * 0.5f));

        for (int i = 0; i <= ypart; i++) //vertex generation
        {
            currentx = -halfwidth;
            float offset = i % 2 == 0.f ? triangle_side_length * 0.5f : 0.f;
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
                indices[6 * (i * xpart + j)] = i % 2 == 0 ? i * (xpart + 1) + j : i * (xpart + 1) + j + 1;
                indices[6 * (i * xpart + j) + 1] = i % 2 == 0 ? i * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j + 1;
                indices[6 * (i * xpart + j) + 2] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j;
                indices[6 * (i * xpart + j) + 3] = i % 2 == 0 ? i * (xpart + 1) + j : i * (xpart + 1) + j;
                indices[6 * (i * xpart + j) + 4] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : i * (xpart + 1) + j + 1;
                indices[6 * (i * xpart + j) + 5] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j : (i + 1) * (xpart + 1) + j;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices_count * sizeof(float), vertices);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices_count * sizeof(unsigned int), indices);

        delete[] vertices;
        delete[] indices;

        return mesh;
	}

	static Mesh* EquilateralNPolygon(int n, float radius) {
		Mesh* mesh = new Mesh();

        int vertices_count = (n + 1) * 3; // n + 1 vertices (introducing center vertex)
        int indices_count = n * 3; // n triangles

        float* vertices = new float[vertices_count];
        unsigned int* indices = new unsigned int[indices_count];

        float current = 0;
        float stepsize = (2 * PI) / n;
        vertices[0] = 0.0f;
        vertices[1] = 0.0f;
        vertices[2] = 0.0f;
        for (int i = 1; i <= n; i++)
        {
            //Ignore warning, false positive
            vertices[3 * i] = radius * sin(current);
            vertices[3 * i + 1] = radius * cos(current);
            vertices[3 * i + 2] = 0.0f;

            indices[3 * (i - 1)] = 0;
            indices[3 * (i - 1) + 1] = i;
            indices[3 * (i - 1) + 2] = i + 1;

            current += stepsize;
        }
        indices[3 * (n - 1) + 2] = 1;

        mesh->_vertexdatabuffer->Write(vertices_count * sizeof(float), vertices);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices_count * sizeof(unsigned int), indices);

        delete[] vertices;
        delete[] indices;

        return mesh;
	}

	static Mesh* BasicRectangle(float width, float height, float depth) {
		Mesh* mesh = new Mesh();

        float halfwidth = width * 0.5f;
        float halfheight = height * 0.5f;
        float halfdepth = depth * 0.5f;

        int i = 0;
        int j = 0;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        //front                   
        // x // y // z //   -  // -x = left // -y = down // -z = away from camera // 0.0 = center world     

        vertices.push_back(-halfwidth); vertices.push_back(halfheight);  vertices.push_back(halfdepth); //0  
        vertices.push_back(halfwidth);  vertices.push_back(halfheight);  vertices.push_back(halfdepth); //1
        vertices.push_back(-halfwidth); vertices.push_back(-halfheight); vertices.push_back(halfdepth); //2
        vertices.push_back(halfwidth);  vertices.push_back(-halfheight); vertices.push_back(halfdepth); //3

        //back  
        vertices.push_back(-halfwidth); vertices.push_back(halfheight);  vertices.push_back(-halfdepth); //4
        vertices.push_back(halfwidth);  vertices.push_back(halfheight);  vertices.push_back(-halfdepth); //5
        vertices.push_back(-halfwidth); vertices.push_back(-halfheight); vertices.push_back(-halfdepth); //6
        vertices.push_back(halfwidth);  vertices.push_back(-halfheight); vertices.push_back(-halfdepth); //7

        //front
        indices.push_back(0); indices.push_back(1); indices.push_back(2); //front left
        indices.push_back(1); indices.push_back(3); indices.push_back(2); //front right

        //right
        indices.push_back(1); indices.push_back(5); indices.push_back(3); //right left
        indices.push_back(5); indices.push_back(7); indices.push_back(3); //right right

        //back
        indices.push_back(5); indices.push_back(4); indices.push_back(7); //front left
        indices.push_back(4); indices.push_back(6); indices.push_back(7); //front right

        //left
        indices.push_back(4); indices.push_back(0); indices.push_back(6); //right left
        indices.push_back(0); indices.push_back(2); indices.push_back(6); //right right

        //top
        indices.push_back(4); indices.push_back(5); indices.push_back(0); //front left
        indices.push_back(5); indices.push_back(1); indices.push_back(0); //front right

        //bottom
        indices.push_back(2); indices.push_back(3); indices.push_back(6); //right left
        indices.push_back(3); indices.push_back(7); indices.push_back(6); //right right

        mesh->_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices.size() * sizeof(unsigned int), &indices[0]);

        return mesh;
	}

	static Mesh* MeshedCube(float sidelength, int partitions) {
		Mesh* mesh = new Mesh();

        //Remember to not redundantly define same vertices multiple times, hence reuse edge vertices
        int vertices_count = (partitions + 1) * partitions * 4 * 3; //front, right, back, left
        vertices_count += (partitions - 1) * (partitions - 1) * 2 * 3;

        //indices_count = partitions * partitions * 6 * 2 * 3; //n * n squares per side (* 6) each consisting of two triangles (* 2) of three indices each (* 3)
        int indices_count = partitions * partitions * 6 * 2 * 3;

        float* vertices = new float[vertices_count];
        unsigned int* indices = new unsigned int[indices_count];

        float half_side = sidelength / 2.0f;
        float stepsize = sidelength / partitions;
        int current_index_iter = 0;

        float current_row, current_col;
        for (int side = 0; side < 4; side++) {
            current_row = -half_side;
            for (int row = 0; row <= partitions; row++) {
                current_col = -half_side;
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
                            indices[current_index_iter++] = offset_side + offset_row + col;
                            indices[current_index_iter++] = offset_side_next + offset_row + 0;
                            indices[current_index_iter++] = offset_side + offset_row_next + col;
                            indices[current_index_iter++] = offset_side_next + offset_row + 0;
                            indices[current_index_iter++] = offset_side_next + offset_row_next + 0;
                            indices[current_index_iter++] = offset_side + offset_row_next + col;
                        }
                        else {
                            indices[current_index_iter++] = offset_side + offset_row + col;
                            indices[current_index_iter++] = offset_side + offset_row + col + 1;
                            indices[current_index_iter++] = offset_side + offset_row_next + col;
                            indices[current_index_iter++] = offset_side + offset_row + col + 1;
                            indices[current_index_iter++] = offset_side + offset_row_next + col + 1;
                            indices[current_index_iter++] = offset_side + offset_row_next + col;
                        }
                    }
                    current_col += stepsize;
                }
                current_row += stepsize;
            }
        }

        //bottom
        int vertex_count = (4 * (partitions + 1) * partitions);
        for (int side = 4; side < 6; side++) {
            int vertex_offset = vertex_count;
            current_row = -half_side;
            for (int row = 0; row < partitions; row++) {
                current_col = -half_side;
                for (int col = 0; col < partitions; col++) {
                    int offset_row = (row * partitions);
                    float x = 0, y = 0, z = 0;
                    x = current_col;
                    y = side == 4 ? -half_side : half_side;
                    z = current_row;

                    int offset_front = 0 * (partitions + 1) * partitions + (side == 4 ? 0 : partitions * partitions);
                    int offset_right = 1 * (partitions + 1) * partitions + (side == 4 ? 0 : partitions * partitions);
                    int offset_back  = 2 * (partitions + 1) * partitions + (side == 4 ? 0 : partitions * partitions);
                    int offset_left  = 3 * (partitions + 1) * partitions + (side == 4 ? 0 : partitions * partitions);
                
                    int topleft =
                        col == 0 ? offset_left + row
                        : row == 0 ? offset_back + partitions - col
                        : vertex_offset + (partitions - 1) * (row - 1) + (col - 1);
                    int topright =
                        row == 0 ? offset_back + partitions - col - 1
                        : col == partitions - 1 ? offset_right + partitions - row
                        : vertex_offset + (partitions - 1) * (row - 1) + col;
                    int bottomleft =
                        row == partitions - 1 ? offset_front + col
                        : col == 0 ? offset_left + row + 1
                        : vertex_offset + (partitions - 1) * row + (col - 1);
                    int bottomright =
                        col == partitions - 1 ? offset_right + partitions - row - 1
                        : row == partitions - 1 ? offset_front + col + 1
                        : vertex_offset + (partitions - 1) * row + col;

                    if (row > 0 && col > 0 && row < partitions && col < partitions) {
                        vertices[vertex_count * 3 + 0] = x;
                        vertices[vertex_count * 3 + 1] = y;
                        vertices[vertex_count * 3 + 2] = z;
                        vertex_count++;
                    }

                    indices[current_index_iter++] = topleft;
                    indices[current_index_iter++] = topright;
                    indices[current_index_iter++] = bottomleft;

                    indices[current_index_iter++] = topright;
                    indices[current_index_iter++] = bottomright;
                    indices[current_index_iter++] = bottomleft;

                    current_col += stepsize;
                }
                current_row += stepsize;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices_count * sizeof(float), vertices);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices_count * sizeof(unsigned int), indices);

        delete[] vertices;
        delete[] indices;

        return mesh;
	}

    static Mesh* Sphere(int stacks, int sectors, float radius) {
        Mesh* mesh = new Mesh();

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        int i1, i2;
        float x, y, z;
        float x1 = 0;
        float phi = 0;
        float theta = 0;
        float sectorstep = (2 * PI) / sectors;
        float stackstep = PI / stacks;
        for (int i = 0; i <= stacks; i++)
        {
            float phi = PI / 2 - i * stackstep;
            x1 = radius * cos(phi);
            y = radius * sin(phi);
            for (int j = 0; j <= sectors; j++)
            {
                theta = j * sectorstep;
                //vertex calculation
                x = x1 * cos(theta);
                z = x1 * sin(theta);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        //indexing
        for (int i = 0; i < stacks; i++)
        {
            i1 = i * (sectors + 1);//current stack
            i2 = i1 + sectors + 1;//next stack

            for (int j = 0; j < sectors; j++)
            {
                if (i != 0)
                {
                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i1 + 1);
                }
                if (i != (stacks - 1))
                {
                    indices.push_back(i1 + 1);
                    indices.push_back(i2);
                    indices.push_back(i2 + 1);
                }
                i1++; i2++;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_vertexindexbuffer->Write(indices.size() * sizeof(unsigned int), &indices[0]);

        return mesh;
    }

    static Mesh* SphereWithNormals(int stacks, int sectors, float radius) {
        Mesh* mesh = new Mesh();

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        int i1, i2;
        float x, y, z;
        float x1 = 0;
        float phi = 0;
        float theta = 0;
        float sectorstep = (2 * PI) / sectors;
        float stackstep = PI / stacks;
        for (int i = 0; i <= stacks; i++)
        {
            float phi = PI / 2 - i * stackstep;
            x1 = radius * cos(phi);
            y = radius * sin(phi);
            for (int j = 0; j <= sectors; j++)
            {
                theta = j * sectorstep;
                //vertex calculation
                x = x1 * cos(theta);
                z = x1 * sin(theta);
                x += 1.5f;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                float length = glm::length(glm::vec3{ x,y,z });
                vertices.push_back(x / length);
                vertices.push_back(y / length);
                vertices.push_back(z / length);
            }
        }
        //indexing
        for (int i = 0; i < stacks; i++)
        {
            i1 = i * (sectors + 1);//current stack
            i2 = i1 + sectors + 1;//next stack

            for (int j = 0; j < sectors; j++)
            {
                if (i != 0)
                {
                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i1 + 1);
                }
                if (i != (stacks - 1))
                {
                    indices.push_back(i1 + 1);
                    indices.push_back(i2);
                    indices.push_back(i2 + 1);
                }
                i1++; i2++;
            }
        }

        mesh->_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
        mesh->_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
        mesh->_arraybuffer->AddAttribute(1, 3, attribute_type::FLOAT32, true);
        mesh->_vertexindexbuffer->Write(indices.size() * sizeof(unsigned int), &indices[0]);

        return mesh;
    }
};
#endif