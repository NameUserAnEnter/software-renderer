#include "Scene.h"

void Scene::Begin() {
	models.clear();
}

void Scene::End() {
	models.clear();
}

void Scene::AddModel(Model model) {
	models.push_back(model);
}

