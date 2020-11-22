#include "SquareCell.h"




SquareCell::SquareCell(int x, int z, int i, Ogre::SceneManager* sceneManager)
{
	tileId = i;
	mMesh = new SquareMesh(x, z, tileId, sceneManager);
}


SquareCell::SquareCell()
{
}

SquareCell::~SquareCell()
{
}


int getOpposite(SquareDirection direction) {
	int oppositeDirection = 0;
	if ((int)direction > 3) {
		oppositeDirection - 4;
	}
	else {
		oppositeDirection + 4;
	}
	return oppositeDirection;
}


void SquareCell::touchCell(Ogre::MaterialPtr color)
{
	mMesh->changeColor(color->getName());
}


SquareCell SquareCell::getNeighbor(SquareDirection direction) {
	return *neighbors[(int)direction];
}


void SquareCell::setNeighbor(SquareDirection direction, SquareCell cell) {
	neighbors[(int)direction] = &cell;
	cell.neighbors[getOpposite(direction)] = this;
}


