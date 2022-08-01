#ifndef OBJECTS_MESH
#define OBJECTS_MESH

#include <vector>
#include <cmath>
#include "glm.hpp"
#include "buffer.hpp"
#define PI 3.14159265358979323846f

class Mesh {
private:
    std::shared_ptr<ArrayBuffer> _arraybuffer;
    std::shared_ptr<VertexDataBuffer> _vertexdatabuffer;
    std::shared_ptr<VertexIndexBuffer> _vertexindexbuffer;

    std::vector<float> _vertices;
    std::vector<unsigned int> _indices;
    
    Mesh();
public:
	~Mesh();

    void Draw();

	static Mesh* SquareTriangleMesh(float width, float height, int xpartitions, int ypartitions) {
		Mesh* mesh = new Mesh();

        int vertices_count = 3 * (xpartitions + 1) * (ypartitions + 1);
        int indices_count = 6 * xpartitions * ypartitions;

        mesh->_vertices.resize(vertices_count);
        mesh->_indices.resize(indices_count);

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
                mesh->_vertices[3 * (i * (xpartitions + 1) + j)]     = currentx;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 1] = currenty;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 2] = 0.0f;
                currentx += stepsizex;
            }
            currenty += stepsizey;
        }

        for (int i = 0; i < ypartitions; i++)
        {
            for (int j = 0; j < xpartitions; j++) {
                mesh->_indices[6 * (i * xpartitions + j)]     = i       * (xpartitions + 1) + j;
                mesh->_indices[6 * (i * xpartitions + j) + 1] = i       * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 2] = (i + 1) * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 3] = i       * (xpartitions + 1) + j;
                mesh->_indices[6 * (i * xpartitions + j) + 4] = (i + 1) * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 5] = (i + 1) * (xpartitions + 1) + j;
            }
        }

        mesh->_vertexdatabuffer->Write(mesh->_vertices.size() * sizeof(float), )

        return mesh;
	}

    static Mesh* SquareTriangleMeshWithNormals(float width, float height, int xpartitions, int ypartitions) {
        Mesh* mesh = new Mesh();

        int vertices_count = 6 * (xpartitions + 1) * (ypartitions + 1);
        int indices_count = 6 * xpartitions * ypartitions;

        mesh->_vertices.resize(vertices_count);
        mesh->_indices.resize(indices_count);

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
                mesh->_vertices[3 * (i * (xpartitions + 1) + j)] = currentx;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 1] = currenty;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 2] = 0.0f;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 3] = 0;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 4] = 0;
                mesh->_vertices[3 * (i * (xpartitions + 1) + j) + 5] = -1;
                currentx += stepsizex;
            }
            currenty += stepsizey;
        }

        for (int i = 0; i < ypartitions; i++)
        {
            for (int j = 0; j < xpartitions; j++) {
                mesh->_indices[6 * (i * xpartitions + j)] = i * (xpartitions + 1) + j;
                mesh->_indices[6 * (i * xpartitions + j) + 1] = i * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 2] = (i + 1) * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 3] = i * (xpartitions + 1) + j;
                mesh->_indices[6 * (i * xpartitions + j) + 4] = (i + 1) * (xpartitions + 1) + j + 1;
                mesh->_indices[6 * (i * xpartitions + j) + 5] = (i + 1) * (xpartitions + 1) + j;
            }
        }

        return mesh;
    }

	static Mesh* EquilateralTriangleMesh(float width, float height, float triangle_side_length) {
		Mesh* mesh = new Mesh();

        int xpart = (int)(width / triangle_side_length);
        int ypart = (int)(height / triangle_side_length);

        int vertices_count = 3 * (xpart + 1) * (ypart + 1);
        int indices_count  = 6 * xpart * ypart;

        mesh->_vertices.resize(vertices_count);
        mesh->_indices.resize(indices_count);

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
                mesh->_vertices[3 * (i * (xpart + 1) + j)] = currentx + offset;
                mesh->_vertices[3 * (i * (xpart + 1) + j) + 1] = currenty;
                mesh->_vertices[3 * (i * (xpart + 1) + j) + 2] = 0.0f;
                currentx += stepsizex;
            }
            currenty += stepsizey;
        }

        for (int i = 0; i < ypart; i++)
        {
            for (int j = 0; j < xpart; j++) {
                mesh->_indices[6 * (i * xpart + j)] = i % 2 == 0 ? i * (xpart + 1) + j : i * (xpart + 1) + j + 1;
                mesh->_indices[6 * (i * xpart + j) + 1] = i % 2 == 0 ? i * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j + 1;
                mesh->_indices[6 * (i * xpart + j) + 2] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : (i + 1) * (xpart + 1) + j;
                mesh->_indices[6 * (i * xpart + j) + 3] = i % 2 == 0 ? i * (xpart + 1) + j : i * (xpart + 1) + j;
                mesh->_indices[6 * (i * xpart + j) + 4] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j + 1 : i * (xpart + 1) + j + 1;
                mesh->_indices[6 * (i * xpart + j) + 5] = i % 2 == 0 ? (i + 1) * (xpart + 1) + j : (i + 1) * (xpart + 1) + j;
            }
        }

        return mesh;
	}

	static Mesh* EquilateralNPolygon(int n, float radius) {
		Mesh* mesh = new Mesh();

        int vertices_count = (n + 1) * 3; // n + 1 vertices (introducing center vertex)
        int indices_count = n * 3; // n triangles

        mesh->_vertices.resize(vertices_count);
        mesh->_indices.resize(indices_count);

        float current = 0;
        float stepsize = (2 * PI) / n;
        mesh->_vertices[0] = 0.0f;
        mesh->_vertices[1] = 0.0f;
        mesh->_vertices[2] = 0.0f;
        for (int i = 1; i <= n; i++)
        {
            //Ignore warning, false positive
            mesh->_vertices[3 * i] = radius * sin(current);
            mesh->_vertices[3 * i + 1] = radius * cos(current);
            mesh->_vertices[3 * i + 2] = 0.0f;

            mesh->_indices[3 * (i - 1)] = 0;
            mesh->_indices[3 * (i - 1) + 1] = i;
            mesh->_indices[3 * (i - 1) + 2] = i + 1;

            current += stepsize;
        }
        mesh->_indices[3 * (n - 1) + 2] = 1;

        return mesh;
	}

	static Mesh* BasicRectangle(float width, float height, float depth) {
		Mesh* mesh = new Mesh();

        float halfwidth = width * 0.5f;
        float halfheight = height * 0.5f;
        float halfdepth = depth * 0.5f;

        int i = 0;
        int j = 0;

        //front                   
        // x // y // z //   -  // -x = left // -y = down // -z = away from camera // 0.0 = center world     

        mesh->_vertices.push_back(-halfwidth); mesh->_vertices.push_back(halfheight);  mesh->_vertices.push_back(halfdepth); //0  
        mesh->_vertices.push_back(halfwidth);  mesh->_vertices.push_back(halfheight);  mesh->_vertices.push_back(halfdepth); //1
        mesh->_vertices.push_back(-halfwidth); mesh->_vertices.push_back(-halfheight); mesh->_vertices.push_back(halfdepth); //2
        mesh->_vertices.push_back(halfwidth);  mesh->_vertices.push_back(-halfheight); mesh->_vertices.push_back(halfdepth); //3

        //back  
        mesh->_vertices.push_back(-halfwidth); mesh->_vertices.push_back(halfheight);  mesh->_vertices.push_back(-halfdepth); //4
        mesh->_vertices.push_back(halfwidth);  mesh->_vertices.push_back(halfheight);  mesh->_vertices.push_back(-halfdepth); //5
        mesh->_vertices.push_back(-halfwidth); mesh->_vertices.push_back(-halfheight); mesh->_vertices.push_back(-halfdepth); //6
        mesh->_vertices.push_back(halfwidth);  mesh->_vertices.push_back(-halfheight); mesh->_vertices.push_back(-halfdepth); //7

        //front
        mesh->_indices.push_back(0); mesh->_indices.push_back(1); mesh->_indices.push_back(2); //front left
        mesh->_indices.push_back(1); mesh->_indices.push_back(3); mesh->_indices.push_back(2); //front right

        //right
        mesh->_indices.push_back(1); mesh->_indices.push_back(5); mesh->_indices.push_back(3); //right left
        mesh->_indices.push_back(5); mesh->_indices.push_back(7); mesh->_indices.push_back(3); //right right

        //back
        mesh->_indices.push_back(5); mesh->_indices.push_back(4); mesh->_indices.push_back(7); //front left
        mesh->_indices.push_back(4); mesh->_indices.push_back(6); mesh->_indices.push_back(7); //front right

        //left
        mesh->_indices.push_back(4); mesh->_indices.push_back(0); mesh->_indices.push_back(6); //right left
        mesh->_indices.push_back(0); mesh->_indices.push_back(2); mesh->_indices.push_back(6); //right right

        //top
        mesh->_indices.push_back(4); mesh->_indices.push_back(5); mesh->_indices.push_back(0); //front left
        mesh->_indices.push_back(5); mesh->_indices.push_back(1); mesh->_indices.push_back(0); //front right

        //bottom
        mesh->_indices.push_back(2); mesh->_indices.push_back(3); mesh->_indices.push_back(6); //right left
        mesh->_indices.push_back(3); mesh->_indices.push_back(7); mesh->_indices.push_back(6); //right right

        return mesh;
	}

	static Mesh* MeshedCube(float sidelength, int partitions) {
		Mesh* mesh = new Mesh();

        //Remember to not redundantly define same vertices multiple times, hence reuse edge vertices
        int vertices_count = (partitions + 1) * partitions * 4 * 3; //front, right, back, left
        vertices_count += (partitions - 1) * (partitions - 1) * 2 * 3;

        //indices_count = partitions * partitions * 6 * 2 * 3; //n * n squares per side (* 6) each consisting of two triangles (* 2) of three indices each (* 3)
        int indices_count = partitions * partitions * 6 * 2 * 3;

        mesh->_vertices.resize(vertices_count);
        mesh->_indices.resize(indices_count);

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
                    mesh->_vertices[(offset_side + offset_row + col) * 3 + 0] = x;
                    mesh->_vertices[(offset_side + offset_row + col) * 3 + 1] = y;
                    mesh->_vertices[(offset_side + offset_row + col) * 3 + 2] = z;


                    if (row < partitions) {
                        int offset_row_next = ((row + 1) * partitions);
                        if (col == partitions - 1) {
                            int offset_side_next = ((side + 1) % 4) * (partitions + 1) * partitions;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row + col;
                            mesh->_indices[current_index_iter++] = offset_side_next + offset_row + 0;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row_next + col;
                            mesh->_indices[current_index_iter++] = offset_side_next + offset_row + 0;
                            mesh->_indices[current_index_iter++] = offset_side_next + offset_row_next + 0;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row_next + col;
                        }
                        else {
                            mesh->_indices[current_index_iter++] = offset_side + offset_row + col;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row + col + 1;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row_next + col;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row + col + 1;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row_next + col + 1;
                            mesh->_indices[current_index_iter++] = offset_side + offset_row_next + col;
                        }
                    }
                    current_col += stepsize;
                }
                current_row += stepsize;
            }
        }

        current_row = -half_side + stepsize;
        for (int row = 1; row < partitions; row++) {
            current_col = -half_side;
            for (int col = 1; col < partitions; col++) {
                int offset_side = (4 * (partitions + 1) * partitions);
                int offset_row = (row * partitions);
                float x = 0, y = 0, z = 0;
            }
        }
        return mesh;
	}

    static Mesh* Sphere(int stacks, int sectors, float radius) {
        Mesh* mesh = new Mesh();

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
            z = radius * sin(phi);
            for (int j = 0; j <= sectors; j++)
            {
                theta = j * sectorstep;
                //vertex calculation
                x = x1 * cos(theta);
                y = x1 * sin(theta);
                mesh->_vertices.push_back(x);
                mesh->_vertices.push_back(y);
                mesh->_vertices.push_back(z);
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
                    mesh->_indices.push_back(i1);
                    mesh->_indices.push_back(i2);
                    mesh->_indices.push_back(i1 + 1);
                }
                if (i != (stacks - 1))
                {
                    mesh->_indices.push_back(i1 + 1);
                    mesh->_indices.push_back(i2);
                    mesh->_indices.push_back(i2 + 1);
                }
                i1++; i2++;
            }
        }

        return mesh;
    }

    static Mesh* SphereWithNormals(int stacks, int sectors, float radius) {
        Mesh* mesh = new Mesh();

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
            z = radius * sin(phi);
            for (int j = 0; j <= sectors; j++)
            {
                theta = j * sectorstep;
                //vertex calculation
                x = x1 * cos(theta);
                y = x1 * sin(theta);
                mesh->_vertices.push_back(x);
                mesh->_vertices.push_back(y);
                mesh->_vertices.push_back(z);
                float length = glm::length(glm::vec3{ x,y,z });
                mesh->_vertices.push_back(x / length);
                mesh->_vertices.push_back(y / length);
                mesh->_vertices.push_back(z / length);
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
                    mesh->_indices.push_back(i1);
                    mesh->_indices.push_back(i2);
                    mesh->_indices.push_back(i1 + 1);
                }
                if (i != (stacks - 1))
                {
                    mesh->_indices.push_back(i1 + 1);
                    mesh->_indices.push_back(i2);
                    mesh->_indices.push_back(i2 + 1);
                }
                i1++; i2++;
            }
        }

        return mesh;
    }
};
#endif