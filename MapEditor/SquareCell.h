#pragma once

#include "Ogre.h"

#include "SquareMesh.h"

class SquareCell
{
public:
	SquareCell(int x, int z, int i, Ogre::SceneManager* sceneManager);
	SquareCell();
	~SquareCell();

	void touchCell(Ogre::MaterialPtr color);
	SquareCell getNeighbor(SquareDirection direction);
	void setNeighbor(SquareDirection direction, SquareCell cell);

	int							tileId;
	SquareCell*					neighbors[8];
	SquareMesh*					mMesh;
};

