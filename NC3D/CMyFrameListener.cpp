
#include "StdAfx.h"
#include "CMyFrameListener.h"

CMyFrameListener::CMyFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light *l): mMouse(0), 
	mKeyboard(0), mInputManager(0), mWindow(win), mCamera(cam), light(l)
{
	mRBtdown = false;
	mCamNode=cam->getParentSceneNode();
	mSceneMgr = sceneMgr;


	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;


	mCameraMan = new OgreBites::SdkCameraMan(mCamera);  


	windowHnd = (size_t )AfxGetMainWnd()->GetSafeHwnd(); // ����������ھ�����Ǵ����MFC������
	windowHndStr << windowHnd;
	// OIS�Ĵ��ڱ���Ҫ���㴰��,����ֻ�д�MFC�������ڸ���,��view�Ͳ���
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


	// ���������ʾ�ͷ���Ϸ��ռ,������������ʾ����Ļ�ϲ������ƶ���������
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	// ���̷���Ϸ��ռ
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	mInputManager = OIS::InputManager::createInputSystem(pl);


	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));


	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
}




CMyFrameListener::~CMyFrameListener(void)
{
	mInputManager->destroyInputObject(mMouse);
	mInputManager->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = 0;
}




bool CMyFrameListener::frameRenderingQueued(const Ogre::FrameEvent& e){


	mMouse->capture();
	mKeyboard->capture();
	mCameraMan->frameRenderingQueued(e); 
	return true;
}


bool CMyFrameListener::mouseMoved(const OIS::MouseEvent &e)
{
	if (mRBtdown)
	{
		mCameraMan->injectMouseMove(e);
	}

	return true;
}


bool CMyFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	//	mCameraMan->injectMouseDown(e, id);
	if (id == OIS::MB_Right)
	{
		mRBtdown = true;
	}

	return true;
}


bool CMyFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{


	//	mCameraMan->injectMouseUp(e, id);
	if (id == OIS::MB_Right)
	{
		mRBtdown = false;
	}
	return true;
}
//������Ӧ
bool CMyFrameListener::keyPressed(const OIS::KeyEvent &e)
{

	mCameraMan->injectKeyDown(e);


	return true;
}


bool CMyFrameListener::keyReleased(const OIS::KeyEvent &e)
{
	mCameraMan->injectKeyUp(e);
	return true;
}

