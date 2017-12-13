#pragma once

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
using namespace Ogre;
class CBuildModeListener: public FrameListener, public OIS::MouseListener,public Ogre::WindowEventListener, public OIS::KeyListener
{
public:
	CBuildModeListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light* light);
	~CBuildModeListener(void);

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	void transPoint2V3(CPoint p2,Ogre::Vector3 &v3);
protected:

	Ogre::SceneManager *mSceneMgr;//场景管理器
	Ogre::SceneNode* mCamNode;//摄像机节点
	Ogre::Camera* mCamera;//摄像机
	Ogre::RenderWindow* mWindow;//渲染窗口
	Ogre::Light* light;//灯光

	OIS::Keyboard* mKeyboard;//键盘         
	OIS::Mouse* mMouse;      //鼠标    
	OIS::InputManager* mInputManager;//输入管理器 

	//	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;     // basic camera controller
	bool mRBtdown;


	CPoint m_mouseLast;
	RaySceneQuery* mRaySceneQuery;
	SceneNode *mCurrentObject;
	Ogre::Vector3 oriPos;
	Ogre::Vector3 startPos;
	Ogre::Vector3 endPos;

};

