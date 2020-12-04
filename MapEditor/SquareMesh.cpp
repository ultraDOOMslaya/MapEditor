#include "SquareMesh.h"

namespace Offsets {
	extern const int innerOffset = 5;
	extern const int offset = 60;
	extern const int adjustedOffset = 55;
}

SquareMesh::SquareMesh(int x, int z, int id, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << id;
	std::string tileNameStr = tileName.str();

	int cm = x * Offsets::offset;
	int cp = z * Offsets::offset;

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
	mNorthEdge->setMaterialName(materialName);
	mSouthEdge->setMaterialName(materialName);
	mEastEdge->setMaterialName(materialName);
	mWestEdge->setMaterialName(materialName);
}


void SquareMesh::changeEdgeColor(SquareDirection direction, Ogre::ColourValue newColor, Ogre::ColourValue neighborColor)
{
	float r = (newColor.r + neighborColor.r) * 0.5f;
	float g = (newColor.g + neighborColor.g) * 0.5f;
	float b = (newColor.b + neighborColor.b) * 0.5f;
	Ogre::ColourValue blendedColor = Ogre::ColourValue(r, g, b, 1.0f);

	if (direction == SquareDirection::N) {
		mNorthMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mNorthEdge->setMaterial(mNorthMat);
	}
	else if (direction == SquareDirection::S) {
		mSouthMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mSouthEdge->setMaterial(mSouthMat);
	}
	else if (direction == SquareDirection::E) {
		mEastMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mEastEdge->setMaterial(mEastMat);
	}
	else if (direction == SquareDirection::W) {
		mWestMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, blendedColor);
		mWestEdge->setMaterial(mWestMat);
	}

	//mEastEdge->setMaterial(blendedMat);
	//mNorthEdge->setMaterial(blendedMat);
	//mSouthEdge->setMaterial(blendedMat);
	//mWestEdge->setMaterial(blendedMat);
	/*mPlaneEntity->setMaterialName(materialName);
	mEastEdge->setMaterialName(materialName);
	mNorthEdge->setMaterialName(materialName);
	mWestEdge->setMaterialName(materialName);
	mSouthEdge->setMaterialName(materialName);*/
}


void SquareMesh::addEdges(int cm, int cp, Ogre::String parentMeshName, Ogre::SceneManager* sceneManager)
{
	int vertices [8];

	/* East Quad */
	vertices[0] = cm;
	vertices[1] = cp;

	vertices[2] = cm;
	vertices[3] = cp + Offsets::offset;

	vertices[4] = cm + Offsets::innerOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::innerOffset;
	vertices[7] = cp + Offsets::innerOffset;

	addQuad(vertices, parentMeshName, "W", sceneManager);

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

	/* West Quad */
	vertices[0] = cm + Offsets::offset;
	vertices[1] = cp;

	vertices[2] = cm + Offsets::adjustedOffset;
	vertices[3] = cp + Offsets::innerOffset;

	vertices[4] = cm + Offsets::adjustedOffset;
	vertices[5] = cp + Offsets::adjustedOffset;

	vertices[6] = cm + Offsets::offset;
	vertices[7] = cp + Offsets::offset;

	addQuad(vertices, parentMeshName, "E", sceneManager);

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
	
}


void SquareMesh::addQuad(int vertices[8], Ogre::String parentMeshName, Ogre::String direction, Ogre::SceneManager* sceneManager)
{
	std::stringstream tileName;
	tileName << parentMeshName << "_" << direction;
	std::string tileNameStr = tileName.str();

	Ogre::ManualObject* man = sceneManager->createManualObject();
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
	Ogre::MeshPtr tileMesh = man->convertToMesh(tileNameStr);

	Ogre::Entity* anEntity = sceneManager->createEntity(tileNameStr, tileMesh);
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(anEntity);
	anEntity->setCastShadows(false);

	if (direction == "W") {
		mWestEdge = anEntity;
		mWestMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_W"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "N") {
		mNorthEdge = anEntity;
		mNorthMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_N"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "S") {
		mSouthEdge = anEntity;
		mSouthMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_S"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
	else if (direction == "E") {
		mEastEdge = anEntity;
		mEastMat = Ogre::MaterialManager::getSingleton().create(
			parentMeshName.append("_E"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	}
}


