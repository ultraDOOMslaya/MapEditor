#include "SquareGrid.h"



SquareGrid::SquareGrid(Ogre::SceneManager* SceneManager, Ogre::MaterialPtr defaultColor)
{
	mScnMgr = SceneManager;
	mDefaultColor = defaultColor;

	for (int z = 0, i = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			createCell(x, z, i++);
		}
	}
}


SquareGrid::~SquareGrid()
{
}


void SquareGrid::createCell(int x, int z, int i) {

	SquareCell* cell = new SquareCell(x, z, i, mScnMgr);
	//cell->touchCell(mDefaultColor);
	cells[i] = *cell;

	if (x > 0) {
		cell->setNeighbor(SquareDirection::W, cells[i - 1]);
	}
	//if (x < width) {
	//	cell->setNeighbor(SquareDirection::E, cells[i + i]);
	//}
	if (z > 0) {
		if ((z & 1) == 0) {
			cell->setNeighbor(SquareDirection::S, cells[i - width]);
		}
	}
}


SquareCell* SquareGrid::getCell(Ogre::String objName) {

	for (int i = 0; i < (width * height); i++) {
		if (objName == cells[i].mMesh->mPlaneEntity->getName()) {
			return &cells[i];
		}
	}
}