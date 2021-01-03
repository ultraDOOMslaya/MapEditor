#include "SquareMesh.h"

namespace Offsetz {
	extern const int innerOffset = 5;
	extern const int offset = 60;
	extern const int adjustedOffset = 55;
}

SquareMesh::SquareMesh(int x, int z, int id, Ogre::Vector3 vertices[4], Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics)
{
	mScnMgr = sceneManager;
	mSquareMetrics = squareMetrics;
	std::stringstream tileName;
	tileName << id;
	std::string tileNameStr = tileName.str();

	int cm = x * Offsetz::offset;
	int cp = z * Offsetz::offset;
	base_x = cm;
	base_y = 0;
	base_z = cp;
	/*base_x = x;
	base_y = 0;
	base_z = z;*/

	for (int index = 0; index < 4; index++) {
		squareVertices[index] = vertices[index];
	}
	/*int indices[12];

	indices[0] = base_x;
	indices[1] = base_y;
	indices[2] = base_z;
	squareVertices[SquareCorners::NW] = mSquareMetrics->Perturb(indices[0], indices[1], indices[2]);

	indices[3] = base_x;
	indices[4] = base_y;
	indices[5] = base_z + Offsets::offset;
	squareVertices[SquareCorners::SW] = mSquareMetrics->Perturb(indices[3], indices[4], indices[5]);

	indices[6] = base_x + Offsets::offset;
	indices[7] = base_y;
	indices[8] = base_z;
	squareVertices[SquareCorners::NE] = mSquareMetrics->Perturb(indices[6], indices[7], indices[8]);

	indices[9] = base_x + Offsets::offset;
	indices[10] = base_y;
	indices[11] = base_z + Offsets::offset;;
	squareVertices[SquareCorners::SE] = mSquareMetrics->Perturb(indices[9], indices[10], indices[11]);*/

	Ogre::ManualObject* man = sceneManager->createManualObject();
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	man->position(cm + Offsetz::innerOffset, 0, cp + Offsetz::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(cm + Offsetz::innerOffset, 0, cp + Offsetz::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(cm + Offsetz::adjustedOffset, 0, cp + Offsetz::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(cm + Offsetz::adjustedOffset, 0, cp + Offsetz::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();
	Ogre::MeshPtr tileMesh = man->convertToMesh(tileNameStr);

	/*Ogre::ManualObject* man = sceneManager->createManualObject();
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	man->position(base_x + Offsets::innerOffset, 0, base_z + Offsets::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(base_x + Offsets::innerOffset, 0, base_z + Offsets::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(base_x + Offsets::adjustedOffset, 0, base_z + Offsets::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(base_x + Offsets::adjustedOffset, 0, base_z + Offsets::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();
	Ogre::MeshPtr tileMesh = man->convertToMesh(tileNameStr);*/

	mPlaneEntity = sceneManager->createEntity(tileNameStr, tileMesh);
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(mPlaneEntity);
	mPlaneEntity->setCastShadows(false);

	addEdges(base_x, base_z, tileNameStr, sceneManager);
	addQuad(vertices, tileNameStr, "Water", sceneManager);
}


SquareMesh::~SquareMesh()
{
}


void SquareMesh::changeColor(Ogre::String materialName)
{
	mPlaneEntity->setMaterialName(materialName);
	mNorthObj->setMaterialName(0, materialName);
	mSouthObj->setMaterialName(0, materialName);
	mEastObj->setMaterialName(0, materialName);
	mWestObj->setMaterialName(0, materialName);
}


void SquareMesh::changeEdgeColor(SquareDirection direction, Ogre::ColourValue newColor, Ogre::ColourValue neighborColor, int targetElevation, int neighborElevation)
{
	float r = (newColor.r + neighborColor.r) * 0.5f;
	float g = (newColor.g + neighborColor.g) * 0.5f;
	float b = (newColor.b + neighborColor.b) * 0.5f;
	Ogre::ColourValue blendedColor = Ogre::ColourValue(r, g, b, 1.0f);

	if (direction == SquareDirection::N) {
		mNorthMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mNorthObj->setMaterialName(0, mNorthMat->getName());
	}
	else if (direction == SquareDirection::S) {
		mSouthMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mSouthObj->setMaterialName(0, mSouthMat->getName());
	}
	else if (direction == SquareDirection::E) {
		mEastMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mEastObj->setMaterialName(0, mEastMat->getName());
	}
	else if (direction == SquareDirection::W) {
		mWestMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mWestObj->setMaterialName(0, mWestMat->getName());
	}
}


void SquareMesh::addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager)
{
	int vertices [8];

	/* West Quad */
	vertices[0] = squareVertices[SquareCorners::NW].x;
	vertices[1] = squareVertices[SquareCorners::NW].z;

	vertices[2] = squareVertices[SquareCorners::SW].x;
	vertices[3] = squareVertices[SquareCorners::SW].z;

	vertices[4] = cm + Offsets::innerOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::innerOffset;
	vertices[7] = cp + Offsets::innerOffset;

	addQuad(vertices, parentMeshName, "W", sceneManager);

	/* West Face */
	vertices[2] = squareVertices[SquareCorners::NW].x;
	vertices[3] = squareVertices[SquareCorners::NW].z;

	vertices[0] = squareVertices[SquareCorners::SW].x;
	vertices[1] = squareVertices[SquareCorners::SW].z;

	vertices[6] = squareVertices[SquareCorners::SW].x;
	vertices[7] = squareVertices[SquareCorners::SW].z;

	vertices[4] = squareVertices[SquareCorners::NW].x;
	vertices[5] = squareVertices[SquareCorners::NW].z;

	addFace(vertices, parentMeshName, "W", sceneManager);

	/* North Quad */
	vertices[0] = cm + Offsets::innerOffset;
	vertices[1] = cp + Offsets::innerOffset;

	vertices[2] = cm + Offsets::adjustedOffset;
	vertices[3] = cp + Offsets::innerOffset;

	vertices[4] = squareVertices[SquareCorners::NE].x;
	vertices[5] = squareVertices[SquareCorners::NE].z;

	vertices[6] = squareVertices[SquareCorners::NW].x;
	vertices[7] = squareVertices[SquareCorners::NW].z;

	addQuad(vertices, parentMeshName, "N", sceneManager);

	/* North Face */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = squareVertices[SquareCorners::NE].z;

	vertices[2] = squareVertices[SquareCorners::NW].x;
	vertices[3] = squareVertices[SquareCorners::NW].z;

	vertices[4] = squareVertices[SquareCorners::NW].x;
	vertices[5] = squareVertices[SquareCorners::NW].z;

	vertices[6] = squareVertices[SquareCorners::NE].x;
	vertices[7] = squareVertices[SquareCorners::NE].z;

	addFace(vertices, parentMeshName, "N", sceneManager);

	/* East Quad */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = squareVertices[SquareCorners::NE].z;;

	vertices[2] = cm + Offsets::adjustedOffset;
	vertices[3] = cp + Offsets::innerOffset;

	vertices[4] = cm + Offsets::adjustedOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = squareVertices[SquareCorners::SE].x;
	vertices[7] = squareVertices[SquareCorners::SE].z;

	addQuad(vertices, parentMeshName, "E", sceneManager);

	/* East Face */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = squareVertices[SquareCorners::NE].z;

	vertices[2] = squareVertices[SquareCorners::SE].x;
	vertices[3] = squareVertices[SquareCorners::SE].z;

	vertices[4] = squareVertices[SquareCorners::SE].x;
	vertices[5] = squareVertices[SquareCorners::SE].z;

	vertices[6] = squareVertices[SquareCorners::NE].x;
	vertices[7] = squareVertices[SquareCorners::NE].z;

	addFace(vertices, parentMeshName, "E", sceneManager);

	/* South Quad */
	vertices[0] = squareVertices[SquareCorners::SW].x;
	vertices[1] = squareVertices[SquareCorners::SW].z;

	vertices[2] = squareVertices[SquareCorners::SE].x;
	vertices[3] = squareVertices[SquareCorners::SE].z;

	vertices[4] = cm + Offsets::adjustedOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::innerOffset;
	vertices[7] = cp + Offsets::adjustedOffset;

	addQuad(vertices, parentMeshName, "S", sceneManager);
	
	/* South Face */
	vertices[0] = squareVertices[SquareCorners::SE].x;
	vertices[1] = squareVertices[SquareCorners::SE].z;

	vertices[2] = squareVertices[SquareCorners::SW].x;
	vertices[3] = squareVertices[SquareCorners::SW].z;

	vertices[4] = squareVertices[SquareCorners::SW].x;
	vertices[5] = squareVertices[SquareCorners::SW].z;

	vertices[6] = squareVertices[SquareCorners::SE].x;
	vertices[7] = squareVertices[SquareCorners::SE].z;

	addFace(vertices, parentMeshName, "S", sceneManager);
}


void SquareMesh::addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << parentMeshName << "_" << direction;
	std::string tileNameStr = tileName.str();

	Ogre::ManualObject* man = sceneManager->createManualObject(tileNameStr);
	man->setDynamic(true);
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	man->position(vertices[0], 0, vertices[1]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(vertices[2], 0, vertices[3]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(vertices[4], 0, vertices[5]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(vertices[6], 0, vertices[7]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();

	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(man);

	if (direction == "W") {
		mWestObj = man;
		//mWestEdge = anEntity;
		mWestMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_W"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "N") {
		mNorthObj = man;
		//mNorthEdge = anEntity;
		mNorthMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_N"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "S") {
		mSouthObj = man;
		//mSouthEdge = anEntity;
		mSouthMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_S"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "E") {
		mEastObj = man;
		//mEastEdge = anEntity;
		mEastMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_E"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
}


void SquareMesh::addQuad(Ogre::Vector3 vertices[4], Ogre::String parentMeshName, Ogre::String objName, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << parentMeshName << "_" << objName;
	std::string tileNameStr = tileName.str();

	Ogre::ManualObject* man = sceneManager->createManualObject(tileNameStr);
	man->setDynamic(true);
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	man->position(vertices[SquareCorners::NW]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(vertices[SquareCorners::SW]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(vertices[SquareCorners::SE]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(vertices[SquareCorners::NE]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();

	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(man);
	if (objName == "Water") {
		mWater = man;
		mWater->setMaterialName(0, "Examples/TransparentTestLight");
		mWater->getParentSceneNode()->translate(0, mSquareMetrics->waterElevationOffset, 0);
	}
	else if (objName == "RampNorth") {
		mNorthRamp = man;
		mNorthRamp->setMaterialName(0, mNorthMat->getName());
	}
	else if (objName == "RampSouth") {
		mSouthRamp = man;
		mSouthRamp->setMaterialName(0, mSouthMat->getName());
	}
	else if (objName == "RampEast") {
		mEastRamp = man;
		mEastRamp->setMaterialName(0, mEastMat->getName());
	}
	else if (objName == "RampWest") {
		mWestRamp = man;
		mWestRamp->setMaterialName(0, mWestMat->getName());
	}
}


void SquareMesh::addFace(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << "Face_" << parentMeshName << "_" << direction;
	std::string tileNameStr = tileName.str();

	Ogre::ManualObject* man = sceneManager->createManualObject(tileNameStr);
	man->setDynamic(true);
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	man->position(vertices[0], 0, vertices[1]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(vertices[2], 0, vertices[3]);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(vertices[4], -50, vertices[5]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(vertices[6], -50, vertices[7]);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();

	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(man);
	Ogre::ColourValue gainsborough = Ogre::ColourValue::ColourValue(0.75f, 0.78f, 0.74f);

	if (direction == "W") {
		mWestFace = man;
		mWestFace->setMaterialName(0, "DarkerGrayMat");
	}
	else if (direction == "N") {
		mNorthFace = man;
		mNorthFace->setMaterialName(0, "GrayMat");
	}
	else if (direction == "S") {
		mSouthFace = man;
		mSouthFace->setMaterialName(0, "GrayMat");
	}
	else if (direction == "E") {
		mEastFace = man;
		mEastFace->setMaterialName(0, "DarkerGrayMat");
	}
}


void SquareMesh::adjustEdges(float height)
{
	if (height != base_y) {
		float heightDifferential = height - base_y;
		mPlaneEntity->getParentSceneNode()->translate(0, heightDifferential, 0);
		base_y = height;
	}

	int vertices[12];

	/* West Quad */
	vertices[0] = squareVertices[SquareCorners::NW].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::NW].z;

	vertices[3] = squareVertices[SquareCorners::SW].x;
	vertices[4] = height;
	vertices[5] = squareVertices[SquareCorners::SW].z;

	vertices[6] = base_x + Offsets::innerOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = base_x + Offsets::innerOffset;
	vertices[10] = height;
	vertices[11] = base_z + Offsets::innerOffset;

	adjustQuad(vertices, mWestObj);

	/* West Face */
	vertices[0] = squareVertices[SquareCorners::NW].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::NW].z;

	vertices[3] = squareVertices[SquareCorners::NW].x;
	vertices[4] = -50;
	vertices[5] = squareVertices[SquareCorners::NW].z;

	vertices[6] = squareVertices[SquareCorners::SW].x;
	vertices[7] = -50;
	vertices[8] = squareVertices[SquareCorners::SW].z;

	vertices[9] = squareVertices[SquareCorners::SW].x;
	vertices[10] = height;
	vertices[11] = squareVertices[SquareCorners::SW].z;

	adjustQuad(vertices, mWestFace);

	/* North Quad */
	vertices[0] = base_x + Offsets::innerOffset;
	vertices[1] = height;
	vertices[2] = base_z + Offsets::innerOffset;

	vertices[3] = base_x + Offsets::adjustedOffset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::innerOffset;

	vertices[6] = squareVertices[SquareCorners::NE].x;
	vertices[7] = height;
	vertices[8] = squareVertices[SquareCorners::NE].z;

	vertices[9] = squareVertices[SquareCorners::NW].x;
	vertices[10] = height;
	vertices[11] = squareVertices[SquareCorners::NW].z;

	adjustQuad(vertices, mNorthObj);

	/* North Face */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::NE].z;

	vertices[3] = squareVertices[SquareCorners::NW].x;
	vertices[4] = height;
	vertices[5] = squareVertices[SquareCorners::NW].z;

	vertices[6] = squareVertices[SquareCorners::NW].x;
	vertices[7] = -50;
	vertices[8] = squareVertices[SquareCorners::NW].z;

	vertices[9] = squareVertices[SquareCorners::NE].x;
	vertices[10] = -50;
	vertices[11] = squareVertices[SquareCorners::NE].z;

	adjustQuad(vertices, mNorthFace);

	/* East Quad */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::NE].z;;

	vertices[3] = base_x + Offsets::adjustedOffset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::innerOffset;

	vertices[6] = base_x + Offsets::adjustedOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = squareVertices[SquareCorners::SE].x;
	vertices[10] = height;
	vertices[11] = squareVertices[SquareCorners::SE].z;

	adjustQuad(vertices, mEastObj);

	/* East Face */
	vertices[0] = squareVertices[SquareCorners::NE].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::NE].z;

	vertices[3] = squareVertices[SquareCorners::SE].x;
	vertices[4] = height;
	vertices[5] = squareVertices[SquareCorners::SE].z;

	vertices[6] = squareVertices[SquareCorners::SE].x;
	vertices[7] = -50;
	vertices[8] = squareVertices[SquareCorners::SE].z;

	vertices[9] = squareVertices[SquareCorners::NE].x;
	vertices[10] = -50;
	vertices[11] = squareVertices[SquareCorners::NE].z;

	adjustQuad(vertices, mEastFace);

	/* South Quad */
	vertices[0] = squareVertices[SquareCorners::SW].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::SW].z;

	vertices[3] = squareVertices[SquareCorners::SE].x;
	vertices[4] = height;
	vertices[5] = squareVertices[SquareCorners::SE].z;

	vertices[6] = base_x + Offsets::adjustedOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = base_x + Offsets::innerOffset;
	vertices[10] = height;
	vertices[11] = base_z + Offsets::adjustedOffset;

	adjustQuad(vertices, mSouthObj);

	/* South Face */
	vertices[0] = squareVertices[SquareCorners::SE].x;
	vertices[1] = height;
	vertices[2] = squareVertices[SquareCorners::SE].z;

	vertices[3] = squareVertices[SquareCorners::SW].x;
	vertices[4] = height;
	vertices[5] = squareVertices[SquareCorners::SW].z;

	vertices[6] = squareVertices[SquareCorners::SW].x;
	vertices[7] = -50;
	vertices[8] = squareVertices[SquareCorners::SW].z;

	vertices[9] = squareVertices[SquareCorners::SE].x;
	vertices[10] = -50;
	vertices[11] = squareVertices[SquareCorners::SE].z;

	adjustQuad(vertices, mSouthFace);
}


void SquareMesh::addRamp(Ogre::String direction, int baseHeight, int elevatedHeight, Ogre::String id, Ogre::String color)
{
	Ogre::Vector3 rampVertices[4];
	for (int i = 0; i < 4; i++) {
		rampVertices[i] = squareVertices[i];
	}

	if (direction == "North") {
		rampVertices[SquareCorners::NW].y = baseHeight;
		rampVertices[SquareCorners::NE].y = baseHeight;
		rampVertices[SquareCorners::SW].y = elevatedHeight;
		rampVertices[SquareCorners::SE].y = elevatedHeight;

		if (mNorthRamp == nullptr) {
			addQuad(rampVertices, id, "RampNorth", mScnMgr);
			mNorthRamp->setMaterialName(0, color);
		}
	}
	if (direction == "South") {
		rampVertices[SquareCorners::NW].y = elevatedHeight;
		rampVertices[SquareCorners::NE].y = elevatedHeight;
		rampVertices[SquareCorners::SW].y = baseHeight;
		rampVertices[SquareCorners::SE].y = baseHeight;

		if (mSouthRamp == nullptr) {
			addQuad(rampVertices, id, "RampSouth", mScnMgr);
			mSouthRamp->setMaterialName(0, color);
		}
	}
	if (direction == "East") {
		rampVertices[SquareCorners::NW].y = elevatedHeight;
		rampVertices[SquareCorners::NE].y = baseHeight;
		rampVertices[SquareCorners::SW].y = elevatedHeight;
		rampVertices[SquareCorners::SE].y = baseHeight;

		if (mEastRamp == nullptr) {
			addQuad(rampVertices, id, "RampEast", mScnMgr);
			mEastRamp->setMaterialName(0, color);
		}
	}
	if (direction == "West") {
		rampVertices[SquareCorners::NW].y = baseHeight;
		rampVertices[SquareCorners::NE].y = elevatedHeight;
		rampVertices[SquareCorners::SW].y = baseHeight;
		rampVertices[SquareCorners::SE].y = elevatedHeight;

		if (mWestRamp == nullptr) {
			addQuad(rampVertices, id, "RampWest", mScnMgr);
			mWestRamp->setMaterialName(0, color);
		}
	}
}


void SquareMesh::removeRamp()
{
	if (mNorthRamp != nullptr) {
		mNorthRamp->detachFromParent();
		mScnMgr->destroyManualObject(mNorthRamp);
		mNorthRamp = nullptr;
	}

	if (mSouthRamp != nullptr) {
		mSouthRamp->detachFromParent();
		mScnMgr->destroyManualObject(mSouthRamp);
		mSouthRamp = nullptr;
	}

	if (mEastRamp != nullptr) {
		mEastRamp->detachFromParent();
		mScnMgr->destroyManualObject(mEastRamp);
		mEastRamp = nullptr;
	}

	if (mWestRamp != nullptr) {
		mWestRamp->detachFromParent();
		mScnMgr->destroyManualObject(mWestRamp);
		mWestRamp = nullptr;
	}
}


void SquareMesh::adjustQuad(int vertices[12], Ogre::ManualObject* edgeObj)
{

	edgeObj->beginUpdate(0);
	edgeObj->position(vertices[0], vertices[1], vertices[2]);
	edgeObj->normal(0, 0, 1);
	edgeObj->textureCoord(0, 0);

	edgeObj->position(vertices[3], vertices[4], vertices[5]);
	edgeObj->normal(0, 0, 1);
	edgeObj->textureCoord(0, 1);

	edgeObj->position(vertices[6], vertices[7], vertices[8]);
	edgeObj->normal(0, 0, 1);
	edgeObj->textureCoord(1, 1);

	edgeObj->position(vertices[9], vertices[10], vertices[11]);
	edgeObj->normal(0, 0, 1);
	edgeObj->textureCoord(1, 0);
	edgeObj->quad(0, 1, 2, 3);
	edgeObj->end();
	//edgeObj->setMaterialName(0, "GrayMat");
}


void SquareMesh::addWater(int waterHeight) {
	/*mWater->setVisible(true);
	float heightDifferential = waterHeight + mSquareMetrics->waterElevationOffset;
	mWater->getParentSceneNode()->translate(0, heightDifferential, 0);*/
}


void SquareMesh::removeWater() {
	/*mWater->setVisible(false);*/
}