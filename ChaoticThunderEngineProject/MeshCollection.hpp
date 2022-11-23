
#ifndef OBJECTS_MESH_COLLECTION
#define OBJECTS_MESH_COLLECTION
#include <vector>
#include "Material.hpp"

class Mesh;

class MeshCollection : private std::vector<Mesh*> {
private:
	std::map<std::string, Material*> _materials;
public:
	~MeshCollection();
	
	void Add(Mesh* mesh);
	void Remove(int i);
	const Mesh* Get(int i);

	void AddMaterial(std::string, Material* material);
	void RemoveMaterial(std::string); 
	Material* GetMaterial(std::string name);

	int MergeCompatibleMeshes();

	void Draw(int i = -1);
};

#endif