#pragma once

#include "Ogre.h"

#include "SquareMetrics.h"
#include "Offsets.h"

enum SquareDirection {
	N, W, S, E
};

enum SquareCorners {
	NW, NE, SW, SE
};

class SquareMesh
{
public:
	SquareMesh(int x, int z, int i, Ogre::Vector3 vertices[4], Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics);
	~SquareMesh();

	void addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager);
	void adjustEdges(float height);	
	void addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);
	void addQuad(Ogre::Vector3 vertices[4], Ogre::String parentMeshName, Ogre::String objName, Ogre::SceneManager* sceneManager);
	void adjustQuad(int vertices[12], Ogre::ManualObject* edgeObj);
	void addFace(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager);

	void changeColor(Ogre::String materialName);
	void changeEdgeColor(SquareDirection direction, Ogre::ColourValue newColor, Ogre::ColourValue neighborColor, int targetElevation, int neighborElevation);
	void addRamp(Ogre::String direction, int baseHeight, int elevatedHeight, Ogre::String id, Ogre::String color);
	void removeRamp();
	void addWater(int height);
	void removeWater();

	Ogre::SceneManager* mScnMgr;
	SquareMetrics* mSquareMetrics;
	int base_x, base_y, base_z;
	Ogre::Vector3 squareVertices[4];

	Ogre::Entity* mPlaneEntity;

	Ogre::ManualObject* mNorthObj;
	Ogre::ManualObject* mNorthFace;
	Ogre::ManualObject* mNorthRamp = nullptr;
	//Ogre::Entity* mNorthEdge;
	Ogre::MaterialPtr mNorthMat;

	Ogre::ManualObject* mSouthObj;
	Ogre::ManualObject* mSouthFace;
	Ogre::ManualObject* mSouthRamp = nullptr;
	//Ogre::Entity* mSouthEdge;
	Ogre::MaterialPtr mSouthMat;

	Ogre::ManualObject* mEastObj;
	Ogre::ManualObject* mEastFace;
	Ogre::ManualObject* mEastRamp = nullptr;
	//Ogre::Entity* mEastEdge;
	Ogre::MaterialPtr mEastMat;

	Ogre::ManualObject* mWestObj;
	Ogre::ManualObject* mWestFace;
	Ogre::ManualObject* mWestRamp = nullptr;
	//Ogre::Entity* mWestEdge;
	Ogre::MaterialPtr mWestMat;

	Ogre::ManualObject* mWater;

	Ogre::ManualObject* mNWCornerRampObj = nullptr;
	Ogre::ManualObject* mNECornerRampObj;
	Ogre::ManualObject* mSWCornerRampObj;
	Ogre::ManualObject* mSECornerRampObj;
};

