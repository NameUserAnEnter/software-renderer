#include "Scene.h"

void Scene::Begin() {
	meshList.clear();
}

void Scene::End() {
	meshList.clear();
}

void Scene::AddMesh(Mesh mesh) {
	meshList.push_back(mesh);
}

