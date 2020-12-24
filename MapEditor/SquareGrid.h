#pragma once

#include "Ogre.h"

#include "SquareCell.h"
#include "SquareMetrics.h"

class SquareGrid
{
public:
	SquareGrid(Ogre::SceneManager* mScnMgr, Ogre::MaterialPtr defaultColor);
	~SquareGrid();

	void createCell(int x, int z, int i);
	SquareCell* getCell(Ogre::String);

	Ogre::SceneManager* mScnMgr;
	Ogre::MaterialPtr					mDefaultColor;
	SquareMetrics*						mSquareMetrics;

	const static int width = 6;
	const static int height = 6;

	SquareCell cells[width * height];
};

