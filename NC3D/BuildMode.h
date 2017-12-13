#pragma once
#include "operatemode.h"
#include "ogre.h"
#include "OgreConfigFile.h"
#include "OgreFrameListener.h"
#include "OgreStringConverter.h"
#include "OIS.h"

#include "OgreBaseApp.h"
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include "OgreBaseApp.h"
#define EPSILON 0.000001 
using namespace Ogre;
class CBuildMode :
	public COperateMode
{
public:
	CBuildMode(COgreBaseApp *ogapp,HWND hw);
	~CBuildMode(void);
	//virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnMButtonUp(UINT nFlags, CPoint point);

	void _TransPoint2V3(CPoint p2,Ogre::Vector3 &v3);
	void local2World(const Vector3 locP, Vector3 &worldP, const SceneNode* node);
	bool World2Screen(Vector3 objPos, Vector2& screenPos);
	void VectorNormalize(Vector3 &v);
	COgreBaseApp *ogreapp;
	HWND m_hwnd;
	Ogre::Camera* mCamera;//ÉãÏñ»ú
	CPoint m_mouseLast;
	SceneNode *mCurrentObject;
	RaySceneQuery* mRaySceneQuery;
	Ogre::Vector3 oriPos;
	Ogre::Vector3 startPos;
	Ogre::Vector3 endPos;
	bool mRBtdown;
};

