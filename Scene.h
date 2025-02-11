#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include <vector>

class Scene {
public:
	Scene();
	~Scene();

	void Begin();
	void End();

	bool AddMesh();
	bool AddMesh(std::string);
	bool AddMesh(std::wstring);

	unsigned int GetMeshCount();

	//Mesh& GetMesh(unsigned int);

public:
	Mesh** meshes;

private:
	unsigned int cMeshes;
};

#endif

