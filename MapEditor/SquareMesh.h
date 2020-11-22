#pragma once

#include "Ogre.h"

enum SquareDirection {
	NW, N, NE, E, SE, S, SW, W
};

class SquareMesh
{
public:
	SquareMesh(int x, int z, int i, Ogre::SceneManager* sceneManager);
	~SquareMesh();

	void addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager);

	void addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);
	void changeColor(Ogre::String materialName);

	Ogre::Entity* mPlaneEntity;
	Ogre::Entity* mNorthEdge;
	Ogre::Entity* mSouthEdge;
	Ogre::Entity* mEastEdge;
	Ogre::Entity* mWestEdge;
};

