#pragma once

#include "Ogre.h"

enum SquareDirection {
	N, W, S, E
};

class SquareMesh
{
public:
	SquareMesh(int x, int z, int i, Ogre::SceneManager* sceneManager);
	~SquareMesh();

	void addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager);

	void addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);
	void changeColor(Ogre::String materialName);
	void changeEdgeColor(SquareDirection direction, Ogre::ColourValue newColor, Ogre::ColourValue neighborColor);

	Ogre::Entity* mPlaneEntity;

	Ogre::Entity* mNorthEdge;
	Ogre::MaterialPtr mNorthMat;

	Ogre::Entity* mSouthEdge;
	Ogre::MaterialPtr mSouthMat;

	Ogre::Entity* mEastEdge;
	Ogre::MaterialPtr mEastMat;

	Ogre::Entity* mWestEdge;
	Ogre::MaterialPtr mWestMat;
};

