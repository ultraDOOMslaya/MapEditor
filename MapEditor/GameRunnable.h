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
#include "SDL.h"

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
	Ogre::MaterialPtr yellowMat;

	Ogre::SceneManager* mScnMgr;
	Ogre::Camera* mCam;
	Ogre::RenderWindow* mWindow;
	Ogre::RaySceneQuery* mRayScnQuery;

	Ogre::SceneNode* camNode;
	Ogre::SceneNode* camAnchor;

	SquareGrid* mSquareGrid;
};

#endif __GameRunnable_h_