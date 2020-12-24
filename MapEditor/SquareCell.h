#pragma once

#include "Ogre.h"

#include "SquareMesh.h"
#include "SquareMetrics.h"

class SquareCell
{
public:
	SquareCell(int x, int z, int i, Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics);
	SquareCell();
	~SquareCell();

	void touchCell(Ogre::MaterialPtr mat, Ogre::ColourValue color, int activeElevation);
	void touchCell(Ogre::MaterialPtr color);
	SquareCell* getNeighbor(SquareDirection direction);
	void setNeighbor(SquareDirection direction, SquareCell* cell);

	int							tileId;
	int							mElevation;
	float						elevationStep = 25.0f;
	SquareCell*					neighbors[8];
	SquareMesh*					mMesh;

	Ogre::ColourValue			mColor;
};

