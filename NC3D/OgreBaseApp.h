#pragma once
#include "ogre.h"  
#include "OgreConfigFile.h"  
#include "OgreFrameListener.h"  
#include "OgreStringConverter.h"  
#include "OIS.h"  
//#include "CMyFrameListener.h"  
#include <SdkTrays.h>  
#include <SdkCameraMan.h>  
using namespace Ogre;  
#define OGRE_DEBUG_MEMORY_MANAGER 1
class COgreBaseApp
{
public:
	COgreBaseApp(void);
	~COgreBaseApp(void);
	Ogre::Root* getRoot(void) const {return mRoot;}  
	Ogre::Camera* getCamera(void) const {return mCamera;}  
	Ogre::SceneNode* getCamNode(void) const {return mCamNode;}  
	Ogre::SceneManager* getSceneManager(void) const {return mSceneMgr;}  
	Ogre::RenderWindow* getRenderWindow(void) const {return mWindow;}  

	void OnSize(int cx,int cy);
	bool go(CRect rt, HWND hWnd);  
	void createRoot(void);  
	void setupResources(void);  
	void setupRenderSystem(void);  
	void createRenderWindow(HWND m_hWnd, int width, int height);  
	int initializeResourceGroups(void);  
	void chooseSceneManager(void);  
	void createCamera(void);  
	void createViewport(void);  
	void createScene(void);  
	void createFrameListener(void);  
	// class CMyFrameListener* mListener;  
private:  
	Ogre::Root* mRoot;  
	Ogre::RenderWindow* mWindow;              
	Ogre::SceneManager* mSceneMgr;  
	Ogre::Camera* mCamera;  
	Ogre::SceneNode* mCamNode;  
	OgreBites::SdkCameraMan* mCameraMan;     // basic camera controller  
	
	Ogre::Light* light;  
};

