#ifndef OBJECTS_MESH
#define OBJECTS_MESH

#include "Structs.hpp"
#include <vector>
#include <cmath>

template <typename T> class Mesh {
private:
	std::vector<T> _vertices;
	std::vector<unsigned int> _indices;

	Mesh();
public:
	~Mesh();

	static Mesh* SquareTriangleMesh(float width, float height, int xpartitions, int ypartitions) {
		Mesh* mesh = new Mesh();

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

        return mesh;
	}

	static Mesh* EquilateralTriangleMesh(float width, float height, float triangle_side_length) {
		Mesh* mesh = new Mesh();

        int xpart = (int)(width / triangle_side_length);
        int ypart = (int)(height / triangle_side_length);

        float halfwidth = width * 0.5f;
        float currentx = -halfwidth;
        float currenty = -height * 0.5f;
        float stepsizex = triangle_side_length;
        float stepsizey = std::sqrt(triangle_side_length * triangle_side_length - (triangle_side_length * 0.5) * (triangle_side_length * 0.5));

        for (int i = 0; i <= ypart; i++) //vertex generation
        {
            currentx = -halfwidth;
            float offset = i % 2 == 0 ? triangle_side_length * 0.5 : 0;
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

	static Mesh* EquilateralNPolygon(float n, float radius) {
		Mesh* mesh = new Mesh();

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

        mesh->_vertices[i++] = -halfwidth; mesh->_vertices[i++] =  halfheight; mesh->_vertices[i++] = halfdepth; //0  
        mesh->_vertices[i++] =  halfwidth; mesh->_vertices[i++] =  halfheight; mesh->_vertices[i++] = halfdepth; //1
        mesh->_vertices[i++] = -halfwidth; mesh->_vertices[i++] = -halfheight; mesh->_vertices[i++] = halfdepth; //2
        mesh->_vertices[i++] =  halfwidth; mesh->_vertices[i++] = -halfheight; mesh->_vertices[i++] = halfdepth; //3

        //back  
        mesh->_vertices[i++] = -halfwidth; mesh->_vertices[i++] =  halfheight; mesh->_vertices[i++] = -halfdepth; //4
        mesh->_vertices[i++] =  halfwidth; mesh->_vertices[i++] =  halfheight; mesh->_vertices[i++] = -halfdepth; //5
        mesh->_vertices[i++] = -halfwidth; mesh->_vertices[i++] = -halfheight; mesh->_vertices[i++] = -halfdepth; //6
        mesh->_vertices[i++] =  halfwidth; mesh->_vertices[i++] = -halfheight; mesh->_vertices[i++] = -halfdepth; //7

        //front
        mesh->_indices[j++] = 0; mesh->_indices[j++] = 1; mesh->_indices[j++] = 2; //front left
        mesh->_indices[j++] = 1; mesh->_indices[j++] = 3; mesh->_indices[j++] = 2; //front right

        //right
        mesh->_indices[j++] = 1; mesh->_indices[j++] = 5; mesh->_indices[j++] = 3; //right left
        mesh->_indices[j++] = 5; mesh->_indices[j++] = 7; mesh->_indices[j++] = 3; //right right

        //back
        mesh->_indices[j++] = 5; mesh->_indices[j++] = 4; mesh->_indices[j++] = 7; //front left
        mesh->_indices[j++] = 4; mesh->_indices[j++] = 6; mesh->_indices[j++] = 7; //front right

        //left
        mesh->_indices[j++] = 4; mesh->_indices[j++] = 0; mesh->_indices[j++] = 6; //right left
        mesh->_indices[j++] = 0; mesh->_indices[j++] = 2; mesh->_indices[j++] = 6; //right right

        //top
        mesh->_indices[j++] = 4; mesh->_indices[j++] = 5; mesh->_indices[j++] = 0; //front left
        mesh->_indices[j++] = 5; mesh->_indices[j++] = 1; mesh->_indices[j++] = 0; //front right

        //bottom
        mesh->_indices[j++] = 2; mesh->_indices[j++] = 3; mesh->_indices[j++] = 6; //right left
        mesh->_indices[j++] = 3; mesh->_indices[j++] = 7; mesh->_indices[j++] = 6; //right right

        return mesh;
	}

	static Mesh* MeshedCube(float sidelength, int partitions) {
		Mesh* mesh = new Mesh();

        float half_side = sidelength / 2.0;
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
                    //(offset_side + offset_row + col) * 3 + 0
                    mesh->_vertices.push_back(x);
                    //(offset_side + offset_row + col) * 3 + 1
                    mesh->_vertices.push_back(y);
                    //(offset_side + offset_row + col) * 3 + 2
                    mesh->_vertices.push_back(z);


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
};
#endif