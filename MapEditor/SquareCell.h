#pragma once

#include "Ogre.h"

#include "SquareMesh.h"

class SquareCell
{
public:
	SquareCell(int x, int z, int i, Ogre::SceneManager* sceneManager);
	SquareCell();
	~SquareCell();

	void touchCell(Ogre::MaterialPtr mat, Ogre::ColourValue color, int activeElevation);
	void touchCell(Ogre::MaterialPtr color);
	SquareCell* getNeighbor(SquareDirection direction);
	void setNeighbor(SquareDirection direction, SquareCell* cell);

	int							tileId;
	int							mElevation;
	float						elevationStep = 5.0f;
	SquareCell*					neighbors[4];
	SquareMesh*					mMesh;

	Ogre::ColourValue			mColor;
};

