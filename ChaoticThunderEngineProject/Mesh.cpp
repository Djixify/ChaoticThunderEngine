#include "Mesh.hpp"

Mesh::Mesh() {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
}

Mesh:: ~Mesh() {
	delete _vertexindexbuffer;
	delete _vertexdatabuffer;
	delete _arraybuffer;
}

void Mesh::Draw() {
	_vertexindexbuffer->Draw();
}