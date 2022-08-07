#include "Mesh.hpp"

Mesh::Mesh() {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
}

Mesh::Mesh(std::vector <float>& vertices, std::vector<unsigned int>& vertindices) {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
	_vertexdatabuffer->Write(vertices.size() * sizeof(float), &vertices[0]);
	_arraybuffer->AddAttribute(0, 3, attribute_type::FLOAT32, false);
	_arraybuffer->AddAttribute(1, 3, attribute_type::FLOAT32, false);
	_arraybuffer->AddAttribute(2, 2, attribute_type::FLOAT32, false);
	_vertexindexbuffer->Write(vertindices.size() * sizeof(unsigned int), &vertindices[0]);
}
Mesh:: ~Mesh() {
	delete _arraybuffer;
	delete _vertexdatabuffer;
	delete _vertexindexbuffer;
}

void Mesh::Draw() {
	_vertexindexbuffer->Draw();
}