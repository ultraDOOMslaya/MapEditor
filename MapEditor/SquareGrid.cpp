#include "SquareGrid.h"



SquareGrid::SquareGrid(Ogre::SceneManager* SceneManager, Ogre::MaterialPtr defaultColor)
{
	mScnMgr = SceneManager;
	mDefaultColor = defaultColor;
	mSquareMetrics = new SquareMetrics();

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

	SquareCell* cell = new SquareCell(x, z, i, mScnMgr, mSquareMetrics);
	//cell->touchCell(mDefaultColor);
	cells[i] = *cell;

	//if (x > 0) {
	//	SquareCell* westNeighbor = &cells[i - 1];
	//	//cell->setNeighbor(SquareDirection::W, westNeighbor);

	//	cell->neighbors[SquareDirection::W] = westNeighbor;
	//	westNeighbor->neighbors[SquareDirection::E] = cell;
	//}

	//if (z > 0) {
	//	SquareCell* northNeighbor = &cells[i - width];
	//	//cell->setNeighbor(SquareDirection::N, northNeighbor);

	//	cell->neighbors[SquareDirection::N] = northNeighbor;
	//	northNeighbor->neighbors[SquareDirection::S] = cell;
	//}
}



SquareCell* SquareGrid::getCell(Ogre::String objName) {

	for (int i = 0; i < (width * height); i++) {
		if (objName == cells[i].mMesh->mPlaneEntity->getName()) {
			return &cells[i];
		}
	}
}