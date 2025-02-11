#include "Scene.h"

Scene::Scene() {
	meshes = (Mesh**) calloc(0, sizeof(Mesh*));
	cMeshes = 0;
}

Scene::~Scene() {
	for (int i = 0; i < cMeshes; i++) {
		delete meshes[i];
	}

	free(meshes);

	meshes = nullptr;
	cMeshes = 0;
}

void Scene::Begin() {
}

void Scene::End() {
}

bool Scene::AddMesh() {
	Mesh** new_ptr = (Mesh**) realloc(meshes, (size_t) (sizeof(Mesh*) * (cMeshes + 1)));
	if (new_ptr == nullptr) return false;

	meshes = new_ptr;
	cMeshes = cMeshes + 1;

	meshes[cMeshes - 1] = new Mesh();

	return true;	// return true on success
}

bool Scene::AddMesh(std::string filepath) {
	return AddMesh(toWide(filepath));
}

bool Scene::AddMesh(std::wstring filepath) {
	if (!AddMesh()) return false;

	meshes[cMeshes - 1]->LoadWavefrontObj(filepath);
	return true;
}

unsigned int Scene::GetMeshCount() {
	return cMeshes;
}

//Mesh& Scene::GetMesh(unsigned int index) {
//	if (index >= 0 && index < cMeshes) {
//		return meshes[index];
//	}
//}

