#pragma once

#include "MyBoundingObjectClass.h"
#include "MyOctree.h"

class BoundingObjectManagerSingleton
{
	// The single instance of the BoundingObjectManagerSingleton
	static BoundingObjectManagerSingleton* instance;

private:
	// Basic constructor
	BoundingObjectManagerSingleton(void) {
		objectList = std::vector<MyBoundingObjectClass>();
	};

	// Copy constructor
	BoundingObjectManagerSingleton(BoundingObjectManagerSingleton const& other) {
		instance = other.GetInstance();
	};

	// Copy assignment
	BoundingObjectManagerSingleton& operator=(BoundingObjectManagerSingleton const& other) {
		instance = other.GetInstance();
	};

	// Basic destructor
	~BoundingObjectManagerSingleton() {};

	bool m_bIsVisible = false;

public:
	// List of objects the singleton creates and has control over
	std::vector<MyBoundingObjectClass> objectList;
	MyOctree octree = MyOctree(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f, 7);

	// Returns the single instance of BoundingObjectManagerSingleton
	// If one doesn't exist, create an instance
	static BoundingObjectManagerSingleton* GetInstance() {
		if (instance == nullptr) {
			instance = new BoundingObjectManagerSingleton();
		}
		return instance;
	};

	// Deletes the instance of BoundingObjectManagerSingleton if not already destroyed
	static void ReleaseInstance(void) {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	};

	// Used to create new Bounding Objects from a list of vertices
	void CreateBoundingObject(std::vector<vector3> vertexList) {
		objectList.push_back(MyBoundingObjectClass(vertexList));
	};

	//render any specific BO or all of them
	void RenderBoundingObject(){
		for (int i = 0; i < objectList.size(); i++) {
			//objectList[i].RenderSphere();
			objectList[i].RenderBox();
		}
	}

	//render the octree
	void RenderOctree() {
		if (octree.GetOctreeVis()) {
			octree.Render();
		}
	}

	//change visibilty of all the objetcs in list
	void SetGeneralVisibility(bool value) {
		for (int i = 0; i < objectList.size(); i++) {
			objectList[i].SetGeneralVisibility(value);
		}
	};

	void SetModelMatrix(matrix4 model, int value) {
		objectList[value].SetModelMatrix(model);
		octree.AddObject(objectList.back());
	};

	//change visibilty of AABB all the objetcs in list
	void SetAABBVisibility(bool value) {
		for (int i = 0; i < objectList.size(); i++) {
			objectList[i].SetAABBVisibility(value);
		}
		m_bIsVisible = value;
	};

	void SetColor(vector3 color) {
		for (int i = 0; i < objectList.size(); i++) {
			objectList[i].SetColor(color);
		}
	};

	void SetSphereColor(vector3 color) {
		for (int i = 0; i < objectList.size(); i++) {
			objectList[i].SetSphereColor(color);
		}
	};

	void UpdateOctree() {
		octree = MyOctree(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f, 7);

		for (int i = 0; i < objectList.size(); i++) {
			octree.AddObject(objectList[i]);
		}
	}

	// Loops through the objectList and checks for collisions
	//switches between SO and Brute Force
	void CheckCollisions() {

		if (octree.GetSOCheck()) {
			//Special Optimization
			octree.CheckCollisions();
		}
		else {
			//Brute Source
			for (int i = 0; i < objectList.size(); i++) {
				objectList[i].SetColliding(false);
			}
			for (int i = 0; i < objectList.size(); i++) {
				for (int j = i; j < objectList.size(); j++) {
					if (i != j) {
						if (objectList[i].IsColliding(&objectList[j])) {

							objectList[i].SetColliding(true);
							objectList[j].SetColliding(true);
						}
					}
				}
			}
		}

	};

	//returns the amount of objects in list
	int GetObjectCount() {
		return objectList.size();
	}

	bool GetAABBVisibility() {
		return m_bIsVisible;
	}

	void UpdatePositions(matrix4 mat, int i) {
		objectList[i].SetModelMatrix(mat);
	};

};
