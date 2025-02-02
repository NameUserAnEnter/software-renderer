#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include <vector>

class Scene {
public:
	void Begin();
	void End();

	void AddMesh(Mesh);
public:
	std::vector<Mesh> meshList;
};

#endif

