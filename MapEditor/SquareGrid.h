#pragma once

#include "Ogre.h"

#include "SquareCell.h"
#include "SquareMetrics.h"
#include "Offsets.h"

class SquareGrid
{
public:
	SquareGrid(Ogre::SceneManager* mScnMgr, Ogre::MaterialPtr defaultColor);
	~SquareGrid();

	void createCell(int x, int z, int i);
	SquareCell* getCell(Ogre::String);
	SquareCell* getCell(int index);
	std::vector<SquareCell*> getNeighbors(int index, int depth);

	Ogre::SceneManager* mScnMgr;
	Ogre::MaterialPtr					mDefaultColor;
	SquareMetrics*						mSquareMetrics;

	const static int width = 30;
	const static int height = 30;

	SquareCell cells[width * height];
};

