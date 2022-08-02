#include "Mesh.hpp"

Mesh::Mesh() {
	_arraybuffer = new ArrayBuffer();
	_vertexdatabuffer = _arraybuffer->CreateVertexBuffer();
	_vertexindexbuffer = _vertexdatabuffer->CreateIndexBuffer();
}

Mesh:: ~Mesh() {
	delete _arraybuffer;
	delete _vertexdatabuffer;
	delete _vertexindexbuffer;
}

void Mesh::Draw() {
	_vertexindexbuffer->Draw();
}