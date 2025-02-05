#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include <vector>

class Scene {
public:
	void Begin();
	void End();

	void AddModel(Model);
public:
	std::vector<Model> models;
};

#endif

