#include "MeshCollection.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

MeshCollection::~MeshCollection() {
	for (int i = 0; i < this->size(); i++)
		delete this->at(i);
	this->clear();
	this->shrink_to_fit();
	//TODO: test to see if the destructor for Material is called when cleared from map
	this->_materials.clear();
}

void MeshCollection::Add(Mesh* mesh) {
	this->push_back(mesh);
}

void MeshCollection::Remove(int i) {
	if (i < this->size())
		this->erase(this->begin() + i);
	else
		Controller::Instance()->ThrowException("Mesh index out of bounds");
}

const Mesh* MeshCollection::Get(int i) {
	if (i < this->size())
		return this->at(i);
	Controller::Instance()->ThrowException("Mesh index out of bounds");
}

void MeshCollection::AddMaterial(std::string name, Material* material) {
	this->_materials[name] = material;
	Controller::Instance()->ThrowException("Not implemented yet");
}
void MeshCollection::RemoveMaterial(std::string name) {
	Controller::Instance()->ThrowException("Not implemented yet");
}
Material* MeshCollection::GetMaterial(std::string name) {
	Controller::Instance()->ThrowException("Not implemented yet");
	return NULL;
}

int MeshCollection::MergeCompatibleMeshes() {
	Controller::Instance()->ThrowException("Not implemented yet");
	return 0;
}

void MeshCollection::Draw(int i) {
	if (i < 0) {
		int size = this->size(); //Only call size once, if in for loop it calls at the end of each iteration
		for (int i = 0; i < size; i++)
			this->at(i)->Draw();
	}
	else if (i < this->size())
		this->at(i)->Draw();
	else
		Controller::Instance()->ThrowException("Mesh index out of bounds");
}