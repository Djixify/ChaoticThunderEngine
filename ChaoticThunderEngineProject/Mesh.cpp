#include "Mesh.hpp"

Mesh::Mesh() {

}

Mesh:: ~Mesh() {
	_vertices.clear();
	_vertices.shrink_to_fit();
	_indices.clear();
	_indices.shrink_to_fit();
}