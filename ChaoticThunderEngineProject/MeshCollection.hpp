
#ifndef OBJECTS_MESH_COLLECTION
#define OBJECTS_MESH_COLLECTION
#include <vector>

class Mesh;

class MeshCollection : private std::vector<Mesh*> {
private:

public:
	~MeshCollection();
	
	void Add(Mesh* mesh);
	void Remove(int i);
	const Mesh* Get(int i);

	int MergeCompatibleMeshes();

	void Draw(int i = -1);
};

#endif