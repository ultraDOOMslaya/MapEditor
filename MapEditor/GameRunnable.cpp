#include "GameRunnable.h"



GameRunnable::GameRunnable()
{
}


GameRunnable::~GameRunnable()
{
}


bool GameRunnable::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
	Ogre::String objectName = "";
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCam);

	mRayScnQuery->setRay(mouseRay);
	Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
	Ogre::RaySceneQueryResult::iterator it = result.begin();
	int rayQuerySize = result.size();

	if (evt.button == SDL_BUTTON_LEFT) {
		if (rayQuerySize > 0) {
			for (; it != result.end(); it++) {
				Ogre::String objName = it->movable->getName();
				Ogre::String parsedObjName = Ogre::StringUtil::split(objName, "_").at(0);
				SquareCell* cell = nullptr;
				cell = mSquareGrid->getCell(parsedObjName);

				if (mEditColorSM->getSelectedItem() == "Green") {
					if (mElevationSlider->getValue() == 0) {
						cell->touchCell(darkerGreenMat, darkerGreenColor, mElevationSlider->getValue());
					}
					else {
						cell->touchCell(greenMat, greenColor, mElevationSlider->getValue());
					}
				}
				else if (mEditColorSM->getSelectedItem() == "Yellow") {
					if (mElevationSlider->getValue() == 0) {
						cell->touchCell(darkerYellowMat, darkerYellowColor, mElevationSlider->getValue());
					}
					else {
						cell->touchCell(yellowMat, yellowColor, mElevationSlider->getValue());
					}
				}
				else if (mEditColorSM->getSelectedItem() == "Blue") {
					cell->touchCell(blueMat, blueColor, mElevationSlider->getValue());
				}
			}
		}
	}
}

SquareDirection getOppositeDirection(SquareDirection direction) {
	if (direction == SquareDirection::W) {
		return SquareDirection::E;
	}
	else {
		return SquareDirection::S;
	}
}

void GameRunnable::setup(void)
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();
	addInputListener(this);
	SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	Ogre::Root* root = getRoot();
	Ogre::ConfigDialog* dialog = OgreBites::getNativeConfigDialog();


	OgreBites::TrayListener* mTrayListener = new OgreBites::TrayListener();
	mScnMgr = root->createSceneManager();
	mScnMgr->addRenderQueueListener(getOverlaySystem());
	mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), mTrayListener);
	mTrayMgr->showFrameStats(OgreBites::TrayLocation::TL_BOTTOMLEFT);
	//mCordPanel = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "MouseCords", "Mouse Cordinates", 175.0f, 125.0f);
	//mModeWidget = mTrayMgr->createParamsPanel(OgreBites::TL_TOPRIGHT, "ModeSet",  Ogre::Real(175), 3);
	/*mModeCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "WallGen", "Editor Mode", 150.0f);
	mUnitCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "UnitGen", "New Units", 150.0f);
	mShowFlowPathCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "ShowPath", "Show Paths", 150.0f);
	mNonCombat = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "NonCombat", "Non Combat", 150.0f);*/
	mMoveableCamera = mTrayMgr->createCheckBox(OgreBites::TL_TOPRIGHT, "MoveableCamera", "Moveable Camera", 150.0f);


	mEditColorSM = mTrayMgr->createThickSelectMenu(OgreBites::TL_TOPLEFT, "EditColor", "Colors:", 180.0f, 3, { "Green", "Yellow", "Blue" });
	/*mGroundTypeSM = mTrayMgr->createThickSelectMenu(OgreBites::TL_TOPLEFT, "GroundType", "Textures:", 180.0f, 2, { "Grass", "Dirt" });*/
	mElevationSlider = mTrayMgr->createThickSlider(OgreBites::TL_TOPLEFT, "Elevation", "#:", 180.0f, 150.0f, -1, 5, 7);
	mElevationSlider->setValue(0);

	addInputListener(mTrayMgr);

	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mScnMgr);

	camAnchor = mScnMgr->getRootSceneNode()->createChildSceneNode("camAnchor");
	camNode = camAnchor->createChildSceneNode("camNode");
	mCam = mScnMgr->createCamera("myCam");
	camAnchor->setPosition(500, 750, 1000);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TransformSpace::TS_WORLD);

	mCam->setNearClipDistance(5);
	camNode->attachObject(mCam);
	Ogre::Viewport* vp = getRenderWindow()->addViewport(mCam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	root->addFrameListener(this);

	mScnMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	//mScnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
	Ogre::Light * spotLight = mScnMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	Ogre::SceneNode * spotLightNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
	spotLightNode->attachObject(spotLight);
	spotLightNode->setDirection(-1, -1, 0);
	spotLightNode->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	mRayScnQuery = mScnMgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setSortByDistance(true);

	greenMat =
		Ogre::MaterialManager::getSingleton().create(
			"GreenMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	greenMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Green);

	yellowMat =
		Ogre::MaterialManager::getSingleton().create(
			"YellowMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	yellowMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::ColourValue(1.0f, 0.8f, 0.0f, 1.0f));

	blueMat =
		Ogre::MaterialManager::getSingleton().create(
			"BlueMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	blueMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Blue);

	grayMat =
		Ogre::MaterialManager::getSingleton().create(
			"GrayMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	grayMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::ColourValue(0.75f, 0.78f, 0.74f));

	darkerGrayMat =
		Ogre::MaterialManager::getSingleton().create(
			"DarkerGrayMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	darkerGreenMat =
		Ogre::MaterialManager::getSingleton().create(
			"DarkerGreenMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	darkerGreenMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, darkerGreenColor);

	darkerGrayMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::ColourValue(0.67f, 0.70f, 0.66f));

	darkerYellowMat =
		Ogre::MaterialManager::getSingleton().create(
			"DarkerYellowMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	darkerYellowMat->getTechnique(0)->getPass(0)->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, darkerYellowColor);

	mSquareGrid = new SquareGrid(mScnMgr, greenMat);

	for (int z = 0, i = 0; z < mSquareGrid->height; z++) {
		for (int x = 0; x < mSquareGrid->width; x++) {
			
			SquareCell* cell = &mSquareGrid->cells[i];

			if (x > 0) {
				SquareCell* westNeighbor = &mSquareGrid->cells[i - 1];
				//cell->setNeighbor(SquareDirection::W, westNeighbor);

				cell->neighbors[SquareDirection::W] = westNeighbor;
				//int oppositeDirection = SquareDirection::E;
				westNeighbor->neighbors[SquareDirection::E] = cell;
			}

			if (z > 0) {
				SquareCell* northNeighbor = &mSquareGrid->cells[i - mSquareGrid->width];
				//cell->setNeighbor(SquareDirection::N, northNeighbor);

				cell->neighbors[SquareDirection::N] = northNeighbor;
				int oppositeDirection = getOppositeDirection(SquareDirection::N);
				northNeighbor->neighbors[oppositeDirection] = cell;
			}

			//if (x > 0 && z > 0) {
			//	SquareCell* northWestNeighbor = &mSquareGrid->cells[i - mSquareGrid->width - 1];

			//	cell->neighbors[SquareDirection::NW] = northWestNeighbor;
			//	northWestNeighbor->neighbors[SquareDirection::SE] = cell;

			//	SquareCell* northEastNeighbor = &mSquareGrid->cells[i - mSquareGrid->width + 1];

			//	cell->neighbors[SquareDirection::NE] = northEastNeighbor;
			//	northEastNeighbor->neighbors[SquareDirection::SW] = cell;
			//}

			i++;
		}
	}

	//Ogre::Image::Filter::FILTER_BILINEAR;
	//Ogre::Image noiseSource;
	//noiseSource.load("PerlinNoise.PNG", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//noiseSource.getColourAt(0, 0, 0);
}
//----------------------------------------------------------------


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char* argv[])
#endif
	{
		// Create application object
		GameRunnable app;
		try {
			app.initApp();
			app.getRoot()->startRendering();
			app.closeApp();
			return 0;
		}
		catch (Ogre::Exception & e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}
		return 0;
	}
#ifdef __cplusplus
}
#endif