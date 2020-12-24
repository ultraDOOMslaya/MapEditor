#pragma once

#include "Ogre.h"

#include "SquareMetrics.h"

enum SquareDirection {
	N, W, S, E, NW, NE, SW, SE
};

class SquareMesh
{
public:
	SquareMesh(int x, int z, int i, Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics);
	~SquareMesh();

	void addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager);
	void adjustEdges(float height);	
	void addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);
	void adjustQuad(int vertices[12], Ogre::ManualObject* edgeObj);
	void addFace(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);

	void changeColor(Ogre::String materialName);
	void changeEdgeColor(SquareDirection direction, Ogre::ColourValue newColor, Ogre::ColourValue neighborColor, int targetElevation, int neighborElevation);
	void alignEdges(SquareDirection direction, int height, int elevationNeighbors[6]);

	Ogre::SceneManager* mScnMgr;
	SquareMetrics* mSquareMetrics;
	int base_x, base_y, base_z;

	Ogre::Entity* mPlaneEntity;

	Ogre::ManualObject* mNorthObj;
	Ogre::ManualObject* mNorthFace;
	//Ogre::Entity* mNorthEdge;
	Ogre::MaterialPtr mNorthMat;

	Ogre::ManualObject* mSouthObj;
	Ogre::ManualObject* mSouthFace;
	//Ogre::Entity* mSouthEdge;
	Ogre::MaterialPtr mSouthMat;

	Ogre::ManualObject* mEastObj;
	Ogre::ManualObject* mEastFace;
	//Ogre::Entity* mEastEdge;
	Ogre::MaterialPtr mEastMat;

	Ogre::ManualObject* mWestObj;
	Ogre::ManualObject* mWestFace;
	//Ogre::Entity* mWestEdge;
	Ogre::MaterialPtr mWestMat;
};

