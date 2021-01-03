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


void SquareGrid::createCell(int x, int z, int i) 
{
	Ogre::Vector3 vertices[4] = { Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO };
	if (x > 0) {
		SquareCell* westNeighbor = &cells[i - 1];
		vertices[SquareCorners::NW] = westNeighbor->mMesh->squareVertices[SquareCorners::NE];
		vertices[SquareCorners::SW] = westNeighbor->mMesh->squareVertices[SquareCorners::SE];
	}

	if (z > 0) {
		SquareCell* northNeighbor = &cells[i - width];
		vertices[SquareCorners::NE] = northNeighbor->mMesh->squareVertices[SquareCorners::SE];
		vertices[SquareCorners::SE] = Ogre::Vector3::ZERO;
	}

	int cm = x * Offsets::offset;
	int cp = z * Offsets::offset;
	int indices[12] = { 0 };

	indices[0] = cm;
	indices[1] = 0;
	indices[2] = cp;
	if (vertices[SquareCorners::NW] == Ogre::Vector3::ZERO) {
		vertices[SquareCorners::NW] = mSquareMetrics->Perturb(indices[0], indices[1], indices[2]);
	}

	indices[3] = cm;
	indices[4] = 0;
	indices[5] = cp + Offsets::offset;
	if (vertices[SquareCorners::SW] == Ogre::Vector3::ZERO) {
		vertices[SquareCorners::SW] = mSquareMetrics->Perturb(indices[3], indices[4], indices[5]);
	}

	indices[6] = cm + Offsets::offset;
	indices[7] = 0;
	indices[8] = cp;
	if (vertices[SquareCorners::NE] == Ogre::Vector3::ZERO) {
		vertices[SquareCorners::NE] = mSquareMetrics->Perturb(indices[6], indices[7], indices[8]);
	}

	indices[9] = cm + Offsets::offset;
	indices[10] = 0;
	indices[11] = cp + Offsets::offset;;
	if (vertices[SquareCorners::SE] == Ogre::Vector3::ZERO) {
		vertices[SquareCorners::SE] = mSquareMetrics->Perturb(indices[9], indices[10], indices[11]);
	}

	SquareCell* cell = new SquareCell(x, z, i, vertices, mScnMgr, mSquareMetrics);
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



SquareCell* SquareGrid::getCell(Ogre::String objName) 
{
	for (int i = 0; i < (width * height); i++) {
		if (objName == cells[i].mMesh->mPlaneEntity->getName()) {
			return &cells[i];
		}
	}

	return NULL;
}


SquareCell* SquareGrid::getCell(int index) 
{
	return &cells[index];
}


std::vector<SquareCell*> SquareGrid::getNeighbors(int index, int depth)
{
	SquareCell* cell = &cells[index];
	int currentWidth = cell->mWidth;
	int currentHeight = cell->mHeight;

	std::vector<SquareCell*>* paletteCells = new std::vector<SquareCell*>();
	paletteCells->push_back(cell);
	for (int i = 0; i < depth+1; ++i) {
		bool zeroHeight = (currentHeight - i) >= 0;
		bool maxHeight = (currentHeight + i) <= height;
		bool maxWidth = (currentWidth + i) <= width;
		bool zeroWidth = (currentWidth - i) >= 0;

		if (maxHeight) {
			paletteCells->push_back(&cells[index + (width * i)]);
		}

		if (zeroHeight) {
			paletteCells->push_back(&cells[index - (width * i)]);
		}

		if (maxWidth) {
			paletteCells->push_back(&cells[index + i]);
		}

		if (zeroWidth) {
			paletteCells->push_back(&cells[index - i]);
		}

		if (zeroWidth && zeroHeight && i > 0) {
			paletteCells->push_back(&cells[index - (width * i) - 1]);
		}

		if (zeroHeight && maxWidth && i > 0) {
			paletteCells->push_back(&cells[index - (width * i) + 1]);
		}

		if (maxWidth && maxHeight && i > 0) {
			paletteCells->push_back(&cells[index + (width * i) + 1]);
		}

		if (maxHeight && zeroWidth && i > 0) {
			paletteCells->push_back(&cells[index + (width * i) - 1]);
		}
	}

	return *paletteCells;
}