#pragma once

#include "Ogre.h"

#include "SquareMesh.h"
#include "SquareMetrics.h"

class SquareCell
{
public:
	SquareCell(int x, int z, int i, Ogre::Vector3 vertices[4], Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics);
	SquareCell();
	~SquareCell();

	void touchCell(Ogre::MaterialPtr mat, Ogre::ColourValue color, int activeElevation, bool isSubmerged, bool ramp, Ogre::String rampDirection);
	void touchCell(Ogre::MaterialPtr color);
	SquareCell* getNeighbor(SquareDirection direction);
	void setNeighbor(SquareDirection direction, SquareCell* cell);
	bool isUnderwater();

	Ogre::SceneManager*			mScnMgr;
	int							tileId;
	int							mElevation;
	int							mWaterLevel;
	int							mWidth;
	int							mHeight;
	bool						mUnderWater;
	bool						mSlope;
	float						elevationStep = 50.0f;
	SquareCell*					neighbors[8];
	SquareMesh*					mMesh;

	Ogre::ColourValue			mColor;
};

