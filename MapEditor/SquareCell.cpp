#include "SquareCell.h"



SquareCell::SquareCell(int x, int z, int i, Ogre::Vector3 vertices[4], Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics)
{
	tileId = i;
	mColor = Ogre::ColourValue::White;
	mElevation = 0;
	mWaterLevel = 0;
	mUnderWater = false;
	mSlope = false;
	mScnMgr = sceneManager;
	mWidth = x;
	mHeight = z;

	mMesh = new SquareMesh(x, z, tileId, vertices, sceneManager, squareMetrics);
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


int* getElevationNeghbors(SquareDirection dir) {
	
}


void SquareCell::touchCell(Ogre::MaterialPtr mat, Ogre::ColourValue color, int activeElevation, bool isSubmerged, bool ramp, Ogre::String rampDirection) {

	float height = activeElevation * elevationStep;
	mElevation = activeElevation;
	mColor = color;

	//mMesh->changeColor("Ocean2_HLSL_GLSL");
	//mMesh->changeColor("Examples/Water1");
	//mMesh->changeColor("Examples/WaterStream"); //This one is really good Examples/TransparentTest
	//mMesh->changeColor("Examples/TransparentTestLight");
	
	
	mMesh->changeColor(mat->getName());
	if (isSubmerged && isUnderwater()) {
		mMesh->addWater(mWaterLevel);
	}
	else {
		mMesh->removeWater();
	}
	mMesh->adjustEdges(height);

	if (ramp) {
		Ogre::String tileName = std::to_string(tileId);
		mMesh->addRamp(rampDirection, height, (height + elevationStep), tileName, mat->getName());
	}
	else {
		mMesh->removeRamp();
	}

	//TODO Color blending is broken after 2-3 passes... just fix when we switch to materials
	/*if (neighbors[SquareDirection::N] != NULL) {

		mMesh->changeEdgeColor(SquareDirection::N, color, neighbors[SquareDirection::N]->mColor, mElevation, neighbors[SquareDirection::N]->mElevation);
		neighbors[SquareDirection::N]->mMesh->changeEdgeColor(SquareDirection::S, neighbors[SquareDirection::N]->mColor, color, neighbors[SquareDirection::N]->mElevation, mElevation);
	}
		
	if (neighbors[SquareDirection::S] != NULL) {

		mMesh->changeEdgeColor(SquareDirection::S, color, neighbors[SquareDirection::S]->mColor, mElevation, neighbors[SquareDirection::S]->mElevation);
		neighbors[SquareDirection::S]->mMesh->changeEdgeColor(SquareDirection::N, neighbors[SquareDirection::S]->mColor, color, neighbors[SquareDirection::S]->mElevation, mElevation);
	}
		
	if (neighbors[SquareDirection::E] != NULL) {

		mMesh->changeEdgeColor(SquareDirection::E, color, neighbors[SquareDirection::E]->mColor, mElevation, neighbors[SquareDirection::E]->mElevation);
		neighbors[SquareDirection::E]->mMesh->changeEdgeColor(SquareDirection::W, neighbors[SquareDirection::E]->mColor, color, neighbors[SquareDirection::E]->mElevation, mElevation);
	}

	if (neighbors[SquareDirection::W] != NULL) {

		mMesh->changeEdgeColor(SquareDirection::W, color, neighbors[SquareDirection::W]->mColor, mElevation, neighbors[SquareDirection::W]->mElevation);
		neighbors[SquareDirection::W]->mMesh->changeEdgeColor(SquareDirection::E, neighbors[SquareDirection::W]->mColor, color, neighbors[SquareDirection::W]->mElevation, mElevation);
	}*/	
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

bool SquareCell::isUnderwater() {
	if (mElevation < mWaterLevel) {
		return true;
	}
	else {
		return false;
	}
}
