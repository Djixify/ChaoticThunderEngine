#include "Mesh.hpp"

Mesh::Mesh() {
	_arraybuffer = std::make_shared<ArrayBuffer>();
	_vertexdatabuffer = std::shared_ptr<VertexDataBuffer>(_arraybuffer->CreateVertexBuffer());
	_vertexindexbuffer = std::shared_ptr<VertexIndexBuffer>(_vertexdatabuffer->CreateIndexBuffer());
}

Mesh:: ~Mesh() {
	_vertices.clear();
	_vertices.shrink_to_fit();
	_indices.clear();
	_indices.shrink_to_fit();
}

void Mesh::Draw() {
	_vertexindexbuffer->Draw();
}