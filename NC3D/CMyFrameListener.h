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
class CMyFrameListener: public FrameListener, public OIS::MouseListener,public Ogre::WindowEventListener, public OIS::KeyListener
{
public:
	CMyFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light* light);
	~CMyFrameListener(void);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

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
};
