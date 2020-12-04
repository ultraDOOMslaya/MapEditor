#include "SquareCell.h"




SquareCell::SquareCell(int x, int z, int i, Ogre::SceneManager* sceneManager)
{
	tileId = i;
	mColor = Ogre::ColourValue::White;
	mMesh = new SquareMesh(x, z, tileId, sceneManager);
}


SquareCell::SquareCell()
{
}

SquareCell::~SquareCell()
{
}


SquareDirection getOpposite(SquareDirection direction) {
	if (direction == SquareDirection::W) {
		return SquareDirection::E;
	}
	else {
		return SquareDirection::S;
	}
}


void SquareCell::touchCell(Ogre::MaterialPtr mat, Ogre::ColourValue color, int activeElevation) {

	mMesh->changeColor(mat->getName());
	mColor = color;
	if (neighbors[SquareDirection::N] != NULL) {
		mMesh->changeEdgeColor(SquareDirection::N, color, neighbors[SquareDirection::N]->mColor);
		neighbors[SquareDirection::N]->mMesh->changeEdgeColor(SquareDirection::S, color, neighbors[SquareDirection::N]->mColor);
	}
		
	if (neighbors[SquareDirection::S] != NULL) {
		mMesh->changeEdgeColor(SquareDirection::S, color, neighbors[SquareDirection::S]->mColor);
		neighbors[SquareDirection::S]->mMesh->changeEdgeColor(SquareDirection::N, color, neighbors[SquareDirection::S]->mColor);

	}
		
	if (neighbors[SquareDirection::E] != NULL) {
		mMesh->changeEdgeColor(SquareDirection::E, color, neighbors[SquareDirection::E]->mColor);
		neighbors[SquareDirection::E]->mMesh->changeEdgeColor(SquareDirection::W, color, neighbors[SquareDirection::E]->mColor);
	}

	if (neighbors[SquareDirection::W] != NULL) {
		mMesh->changeEdgeColor(SquareDirection::W, color, neighbors[SquareDirection::W]->mColor);
		neighbors[SquareDirection::W]->mMesh->changeEdgeColor(SquareDirection::E, color, neighbors[SquareDirection::W]->mColor);
	}

	mMesh->mPlaneEntity->getParentSceneNode()->translate(0, (activeElevation * elevationStep), 0);
	
}


void SquareCell::touchCell(Ogre::MaterialPtr color)
{
	mMesh->changeColor(color->getName());
}


SquareCell* SquareCell::getNeighbor(SquareDirection direction) {
	return neighbors[direction];
}


void SquareCell::setNeighbor(SquareDirection direction, SquareCell* cell) {
	neighbors[direction] = cell;
	//int oppositeDirection = getOpposite(direction);
	if (direction == SquareDirection::W) 
		cell->neighbors[SquareDirection::E] = this;

	if (direction == SquareDirection::N)
		cell->neighbors[SquareDirection::S] = this;

}


