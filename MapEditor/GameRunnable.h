#pragma once
#ifndef __GameRunnable_h_
#define __GameRunnable_h_

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElementFactory.h>

#include <OgreTrays.h>
#include "SDL2-2.0.8/include/SDL.h"

#include "SquareGrid.h"
#include "SquareCell.h"

class GameRunnable : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	GameRunnable();
	~GameRunnable();

	void setup(void);
	bool mouseReleased(const OgreBites::MouseButtonEvent& evt);

	OgreBites::TrayManager* mTrayMgr;
	OgreBites::TextBox* mCordPanel;			// Coordinates displayer
	OgreBites::CheckBox* mModeCB;
	OgreBites::CheckBox* mUnitCB;
	OgreBites::CheckBox* mShowFlowPathCB;
	OgreBites::CheckBox* mNonCombat;
	OgreBites::CheckBox* mMoveableCamera;

	//Top left panel
	OgreBites::SelectMenu* mEditColorSM;
	OgreBites::SelectMenu* mGroundTypeSM;
	OgreBites::Slider* mElevationSlider;

	Ogre::MaterialPtr greenMat;
	Ogre::ColourValue greenColor = Ogre::ColourValue::Green;
	Ogre::MaterialPtr yellowMat;
	Ogre::ColourValue yellowColor = Ogre::ColourValue::ColourValue(1.0f, 0.8f, 0.0f, 1.0f);
	Ogre::MaterialPtr blueMat;
	Ogre::ColourValue blueColor = Ogre::ColourValue::Blue;
	Ogre::MaterialPtr grayMat;
	Ogre::ColourValue grayColor = Ogre::ColourValue::ColourValue(0.75f, 0.78f, 0.74f);
	
	/* elevations */
	Ogre::MaterialPtr darkerGreenMat;
	Ogre::ColourValue darkerGreenColor = Ogre::ColourValue::ColourValue(0.0f, 0.92f, 0.0f, 1.0f);
	Ogre::MaterialPtr darkerYellowMat;
	Ogre::ColourValue darkerYellowColor = Ogre::ColourValue::ColourValue(0.92f, 0.72f, 0.0f, 1.0f);
	Ogre::MaterialPtr darkerGrayMat;
	Ogre::ColourValue darkerGrayColor = Ogre::ColourValue::ColourValue(0.68f, 0.68f, 0.68f);


	Ogre::SceneManager* mScnMgr;
	Ogre::Camera* mCam;
	Ogre::RenderWindow* mWindow;
	Ogre::RaySceneQuery* mRayScnQuery;

	Ogre::SceneNode* camNode;
	Ogre::SceneNode* camAnchor;

	SquareGrid* mSquareGrid;
};

#endif __GameRunnable_h_