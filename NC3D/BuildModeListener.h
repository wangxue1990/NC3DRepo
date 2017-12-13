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

	Ogre::SceneManager *mSceneMgr;//����������
	Ogre::SceneNode* mCamNode;//������ڵ�
	Ogre::Camera* mCamera;//�����
	Ogre::RenderWindow* mWindow;//��Ⱦ����
	Ogre::Light* light;//�ƹ�

	OIS::Keyboard* mKeyboard;//����         
	OIS::Mouse* mMouse;      //���    
	OIS::InputManager* mInputManager;//��������� 

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

