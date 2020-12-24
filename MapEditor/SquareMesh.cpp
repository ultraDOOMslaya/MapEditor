#include "SquareMesh.h"

namespace Offsets {
	extern const int innerOffset = 5;
	extern const int offset = 60;
	extern const int adjustedOffset = 55;
}

SquareMesh::SquareMesh(int x, int z, int id, Ogre::SceneManager* sceneManager, SquareMetrics* squareMetrics)
{
	mScnMgr = sceneManager;
	mSquareMetrics = squareMetrics;
	std::stringstream tileName;
	tileName << id;
	std::string tileNameStr = tileName.str();

	int cm = x * Offsets::offset;
	int cp = z * Offsets::offset;
	base_x = cm;
	base_y = 0;
	base_z = cp;

	Ogre::ManualObject* man = sceneManager->createManualObject();
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	man->position(cm + Offsets::innerOffset, 0, cp + Offsets::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	man->position(cm + Offsets::innerOffset, 0, cp + Offsets::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	man->position(cm + Offsets::adjustedOffset, 0, cp + Offsets::adjustedOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	man->position(cm + Offsets::adjustedOffset, 0, cp + Offsets::innerOffset);
	man->normal(0, 0, 1);
	man->textureCoord(1, 0);
	man->quad(0, 1, 2, 3);
	man->end();
	Ogre::MeshPtr tileMesh = man->convertToMesh(tileNameStr);

	mPlaneEntity = sceneManager->createEntity(tileNameStr, tileMesh);
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(mPlaneEntity);
	mPlaneEntity->setCastShadows(false);

	addEdges(cm, cp, tileNameStr, sceneManager);
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
		/*blendedColor.r -= 0.3f;
		blendedColor.g -= 0.3f;
		blendedColor.b -= 0.3f;*/
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
	vertices[0] = cm;
	vertices[1] = cp;

	vertices[2] = cm;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm + Offsets::innerOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::innerOffset;
	vertices[7] = cp + Offsets::innerOffset;

	addQuad(vertices, parentMeshName, "W", sceneManager);

	/* West Face */
	vertices[0] = cm;
	vertices[1] = cp;

	vertices[2] = cm;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm;
	vertices[5] = cp + Offsets::offset;

	vertices[6] = cm;
	vertices[7] = cp;

	addFace(vertices, parentMeshName, "W", sceneManager);

	/* North Quad */
	vertices[0] = cm + Offsets::innerOffset;
	vertices[1] = cp + Offsets::innerOffset;

	vertices[2] = cm + Offsets::adjustedOffset;
	vertices[3] = cp + Offsets::innerOffset;

	vertices[4] = cm + Offsets::offset;
	vertices[5] = cp;

	vertices[6] = cm;
	vertices[7] = cp;

	addQuad(vertices, parentMeshName, "N", sceneManager);

	/* North Face */
	vertices[0] = cm + Offsets::offset;
	vertices[1] = cp;

	vertices[2] = cm;
	vertices[3] = cp;

	vertices[4] = cm;
	vertices[5] = cp;

	vertices[6] = cm + Offsets::offset;
	vertices[7] = cp;

	addFace(vertices, parentMeshName, "N", sceneManager);

	/* East Quad */
	vertices[0] = cm + Offsets::offset;
	vertices[1] = cp;

	vertices[2] = cm + Offsets::adjustedOffset;
	vertices[3] = cp + Offsets::innerOffset;

	vertices[4] = cm + Offsets::adjustedOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::offset;
	vertices[7] = cp + Offsets::offset;

	addQuad(vertices, parentMeshName, "E", sceneManager);

	/* East Face */
	vertices[0] = cm + Offsets::offset;
	vertices[1] = cp;

	vertices[2] = cm + Offsets::offset;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm + Offsets::offset;
	vertices[5] = cp + Offsets::offset;

	vertices[6] = cm + Offsets::offset;
	vertices[7] = cp;

	addFace(vertices, parentMeshName, "E", sceneManager);

	/* South Quad */
	vertices[0] = cm;
	vertices[1] = cp + Offsets::offset;

	vertices[2] = cm + Offsets::offset;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm + Offsets::adjustedOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::innerOffset;
	vertices[7] = cp + Offsets::adjustedOffset;

	addQuad(vertices, parentMeshName, "S", sceneManager);
	
	/* South Face */
	vertices[0] = cm + Offsets::offset;
	vertices[1] = cp + Offsets::offset;

	vertices[2] = cm;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm;
	vertices[5] = cp + Offsets::offset;

	vertices[6] = cm + Offsets::offset;
	vertices[7] = cp + Offsets::offset;

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

	Ogre::Vector3 v1 = mSquareMetrics->Perturb(vertices[0], 0, vertices[1]);
	man->position(v1);
	man->normal(0, 0, 1);
	man->textureCoord(0, 0);

	Ogre::Vector3 v2 = mSquareMetrics->Perturb(vertices[2], 0, vertices[3]);
	man->position(v2);
	man->normal(0, 0, 1);
	man->textureCoord(0, 1);

	Ogre::Vector3 v3 = mSquareMetrics->Perturb(vertices[4], 0, vertices[5]);
	man->position(v3);
	man->normal(0, 0, 1);
	man->textureCoord(1, 1);

	Ogre::Vector3 v4 = mSquareMetrics->Perturb(vertices[6], 0, vertices[7]);
	man->position(v4);
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


void SquareMesh::addFace(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << parentMeshName << "_" << direction << "_Face";
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
	vertices[0] = base_x;
	vertices[1] = height;
	vertices[2] = base_z;

	vertices[3] = base_x;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::offset;

	vertices[6] = base_x + Offsets::innerOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = base_x + Offsets::innerOffset;
	vertices[10] = height;
	vertices[11] = base_z + Offsets::innerOffset;

	adjustQuad(vertices, mWestObj);

	/* West Face */
	vertices[0] = base_x;
	vertices[1] = height;
	vertices[2] = base_z;

	vertices[3] = base_x;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::offset;

	vertices[6] = base_x;
	vertices[7] = 0;
	vertices[8] = base_z + Offsets::offset;

	vertices[9] = base_x;
	vertices[10] = 0;
	vertices[11] = base_z;

	adjustQuad(vertices, mWestFace);

	/* North Quad */
	vertices[0] = base_x + Offsets::innerOffset;
	vertices[1] = height;
	vertices[2] = base_z + Offsets::innerOffset;

	vertices[3] = base_x + Offsets::adjustedOffset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::innerOffset;

	vertices[6] = base_x + Offsets::offset;
	vertices[7] = height;
	vertices[8] = base_z;

	vertices[9] = base_x;
	vertices[10] = height;
	vertices[11] = base_z;

	adjustQuad(vertices, mNorthObj);

	/* North Face */
	vertices[0] = base_x + Offsets::offset;
	vertices[1] = height;
	vertices[2] = base_z;

	vertices[3] = base_x;
	vertices[4] = height;
	vertices[5] = base_z;

	vertices[6] = base_x;
	vertices[7] = 0;
	vertices[8] = base_z;

	vertices[9] = base_x + Offsets::offset;
	vertices[10] = 0;
	vertices[11] = base_z;

	adjustQuad(vertices, mNorthFace);

	/* East Quad */
	vertices[0] = base_x + Offsets::offset;
	vertices[1] = height;
	vertices[2] = base_z;

	vertices[3] = base_x + Offsets::adjustedOffset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::innerOffset;

	vertices[6] = base_x + Offsets::adjustedOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = base_x + Offsets::offset;
	vertices[10] = height;
	vertices[11] = base_z + Offsets::offset;

	adjustQuad(vertices, mEastObj);

	/* East Face */
	vertices[0] = base_x + Offsets::offset;
	vertices[1] = height;
	vertices[2] = base_z;

	vertices[3] = base_x + Offsets::offset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::offset;

	vertices[6] = base_x + Offsets::offset;
	vertices[7] = 0;
	vertices[8] = base_z + Offsets::offset;

	vertices[9] = base_x + Offsets::offset;
	vertices[10] = 0;
	vertices[11] = base_z;

	adjustQuad(vertices, mEastFace);

	/* South Quad */
	vertices[0] = base_x;
	vertices[1] = height;
	vertices[2] = base_z + Offsets::offset;

	vertices[3] = base_x + Offsets::offset;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::offset;

	vertices[6] = base_x + Offsets::adjustedOffset;
	vertices[7] = height;
	vertices[8] = base_z + Offsets::adjustedOffset;

	vertices[9] = base_x + Offsets::innerOffset;
	vertices[10] = height;
	vertices[11] = base_z + Offsets::adjustedOffset;

	adjustQuad(vertices, mSouthObj);

	/* South Face */
	vertices[0] = base_x + Offsets::offset;
	vertices[1] = height;
	vertices[2] = base_z + Offsets::offset;

	vertices[3] = base_x;
	vertices[4] = height;
	vertices[5] = base_z + Offsets::offset;

	vertices[6] = base_x;
	vertices[7] = 0;
	vertices[8] = base_z + Offsets::offset;

	vertices[9] = base_x + Offsets::offset;
	vertices[10] = 0;
	vertices[11] = base_z + Offsets::offset;

	adjustQuad(vertices, mSouthFace);
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


void SquareMesh::alignEdges(SquareDirection direction, int height, int elevationNeighbors[6])
{
	int vertices[12];

	//vertices[0] = base_x + Offsets::offset;
	//vertices[1] = 0;
	//vertices[2] = base_z;

	//vertices[3] = base_x + Offsets::adjustedOffset;
	//vertices[4] = height;
	//vertices[5] = base_z + Offsets::innerOffset;

	//vertices[6] = base_x + Offsets::adjustedOffset;
	//vertices[7] = height;
	//vertices[8] = base_z + Offsets::adjustedOffset;

	//vertices[9] = base_x + Offsets::offset;
	//vertices[10] = 0;
	//vertices[11] = base_z + Offsets::offset;

	//1 equals the same elevation

	if (direction == SquareDirection::E) {
		/* East Edge (bridge) */
		vertices[0] = base_x + Offsets::offset;
		vertices[1] = height;
		vertices[2] = elevationNeighbors[SquareDirection::N] == 1 ? base_z : base_z + Offsets::innerOffset;

		vertices[3] = base_x + Offsets::adjustedOffset;
		vertices[4] = height;
		vertices[5] = base_z + Offsets::innerOffset;

		vertices[6] = base_x + Offsets::adjustedOffset;
		vertices[7] = height;
		vertices[8] = base_z + Offsets::adjustedOffset;

		vertices[9] = base_x + Offsets::offset;
		vertices[10] = height;
		vertices[11] = elevationNeighbors[SquareDirection::S] == 1 ? base_z + Offsets::adjustedOffset : base_z + Offsets::offset;
		//vertices[11] = base_z + Offsets::adjustedOffset;

		adjustQuad(vertices, mEastObj);

		/* South Edge */
		vertices[0] = base_x;
		vertices[1] = 0;
		vertices[2] = base_z + Offsets::offset;

		vertices[3] = base_x + Offsets::offset;
		vertices[4] = 0;
		vertices[5] = base_z + Offsets::offset;

		vertices[6] = base_x + Offsets::offset;
		vertices[7] = height;
		vertices[8] = base_z + Offsets::adjustedOffset;

		vertices[9] = base_x + Offsets::innerOffset;
		vertices[10] = height;
		vertices[11] = base_z + Offsets::adjustedOffset;

		adjustQuad(vertices, mSouthObj);

		/* North Edge */
		vertices[0] = base_x + Offsets::innerOffset;
		vertices[1] = height;
		vertices[2] = base_z + Offsets::innerOffset;

		vertices[3] = base_x + Offsets::offset;
		vertices[4] = height;
		vertices[5] = base_z + Offsets::innerOffset;

		vertices[6] = base_x + Offsets::offset;
		vertices[7] = 0;
		vertices[8] = base_z;

		vertices[9] = base_x;
		vertices[10] = 0;
		vertices[11] = base_z;

		adjustQuad(vertices, mNorthObj);

	}

	if (direction == SquareDirection::W) {
		/* West Edge (bridge) */
		vertices[0] = base_x;
		vertices[1] = height;
		vertices[2] = base_z + Offsets::innerOffset;

		vertices[3] = base_x;
		vertices[4] = height;
		vertices[5] = base_z + Offsets::adjustedOffset;

		vertices[6] = base_x + Offsets::innerOffset;
		vertices[7] = height;
		vertices[8] = base_z + Offsets::adjustedOffset;

		vertices[9] = base_x + Offsets::innerOffset;
		vertices[10] = height;
		vertices[11] = base_z + Offsets::innerOffset;

		adjustQuad(vertices, mWestObj);

		/* South Edge */
		vertices[0] = base_x;
		vertices[1] = 0;
		vertices[2] = base_z + Offsets::offset;

		vertices[3] = base_x + Offsets::offset;
		vertices[4] = 0;
		vertices[5] = base_z + Offsets::offset;

		vertices[6] = base_x + Offsets::adjustedOffset;
		vertices[7] = height;
		vertices[8] = base_z + Offsets::adjustedOffset;

		vertices[9] = base_x;
		vertices[10] = height;
		vertices[11] = base_z + Offsets::adjustedOffset;

		adjustQuad(vertices, mSouthObj);

		/* North Edge */
		vertices[0] = base_x + Offsets::offset;
		vertices[1] = height;
		vertices[2] = base_z + Offsets::innerOffset;

		vertices[3] = base_x + Offsets::adjustedOffset;
		vertices[4] = height;
		vertices[5] = base_z + Offsets::innerOffset;

		vertices[6] = base_x + Offsets::offset;
		vertices[7] = 0;
		vertices[8] = base_z;

		vertices[9] = base_x;
		vertices[10] = 0;
		vertices[11] = base_z;

		adjustQuad(vertices, mNorthObj);
	}
	
}