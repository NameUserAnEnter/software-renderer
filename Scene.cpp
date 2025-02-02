#include "Scene.h"

Scene::Scene() {
	meshList.clear();
}

void Scene::Begin() {
	meshList.clear();
}

void Scene::End() {
	meshList.clear();
}

void Scene::AddMesh(Mesh mesh) {
	meshList.push_back(mesh);
}

