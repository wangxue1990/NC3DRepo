#include "StdAfx.h"
#include "OgreBaseApp.h"


COgreBaseApp::COgreBaseApp(void)
{
	mRoot=NULL;
	mCameraMan=NULL;
	mWindow=NULL;
	//mListener=NULL;
}


COgreBaseApp::~COgreBaseApp(void)
{
	if(NULL!=mRoot) 	delete mRoot;
	if (NULL!=mCameraMan) delete mCameraMan;
//	if (NULL!=mListener) delete mListener;
}
bool COgreBaseApp::go(CRect rt, HWND hWnd)  
{  
	createRoot();  
	setupResources();  
	setupRenderSystem();  
	createRenderWindow(hWnd, rt.Width(), rt.Height());  
	chooseSceneManager();  
	createCamera();  
	createViewport();  
	initializeResourceGroups();  
	createScene();  
	createFrameListener();  
	return true;  
}  


void COgreBaseApp::createRoot(void)  
{  
	mRoot = new Ogre::Root("plugins.cfg");  
}  


void COgreBaseApp::setupResources(void)  
{  
	// Load resource paths from config file  
	Ogre::ConfigFile cf;  
	cf.load("resources.cfg");  

	// Go through all sections & settings in the file  
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();  

	Ogre::String secName, typeName, archName;  
	while (seci.hasMoreElements())  
	{  
		secName = seci.peekNextKey();  
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();  
		Ogre::ConfigFile::SettingsMultiMap::iterator i;  
		for (i = settings->begin(); i != settings->end(); ++i)  
		{  
			typeName = i->first;  
			archName = i->second;  
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(  
				archName, typeName, secName);  
		}  
	}  
}  


void COgreBaseApp::setupRenderSystem(void)  
{  
	//mRoot->showConfigDialog();
	
	RenderSystemList* rl = &const_cast<RenderSystemList&>(mRoot->getAvailableRenderers());
	RenderSystem* rSys = NULL;
	RenderSystemList::iterator it = rl->begin();
	while (it!=rl->end())
	{
		//if (-1 != (*it)->getName().find("GL"))
		if (-1 != (*it)->getName().find("OpenGL"))
		{
			rSys = (RenderSystem*)(*it);
			break;
		}
		++it;
	}

	rSys->setConfigOption("Full Screen","No");
	rSys->setConfigOption("VSync","No");
	mRoot->setRenderSystem(rSys);
	//if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())  
	//  throw Ogre::Exception(1, "未选择渲染系统","setupRenderSystem()");  
}  

void COgreBaseApp::OnSize(int cx,int cy)
{
	// TODO: 在此处添加消息处理程序代码
	if (mWindow)
	{
		
		mWindow->windowMovedOrResized();

		if (cy != 0 && mCamera != NULL)
		{
			mCamera->setNearClipDistance(Real(mWindow->getWidth())/Real(mWindow->getHeight()));
		}

		mCamera->yaw(Radian(0));
	}
}


void COgreBaseApp::createRenderWindow(HWND m_hWnd, int width, int height)  
{  
	mRoot->initialise(false);//禁止ogre创建新的渲染窗口，而使用MFC的窗口  
	NameValuePairList misc;  
	misc["externalWindowHandle"] = StringConverter::toString((int)m_hWnd);   
	mWindow = mRoot->createRenderWindow("OgreRenderWindow", width, height, false, &misc);  
}  


int COgreBaseApp::initializeResourceGroups(void)  
{  
	TextureManager::getSingleton().setDefaultNumMipmaps(5);  
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();  
	return 0;  
}  


void COgreBaseApp::chooseSceneManager(void)  
{  
	mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);  
}  


void COgreBaseApp::createCamera(void)  
{  
	// Create the camera  
	mCamera = mSceneMgr->createCamera("PlayerCam");  

	// Position it at 500 in Z direction  
	mCamera->setPosition(Ogre::Vector3(0,0,80));  
	// Look back along -Z  
	mCamera->lookAt(Ogre::Vector3(0,0,-300));  
	mCamera->setNearClipDistance(5);  
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller  
}  


void COgreBaseApp::createViewport(void)  
{  
	// Create one viewport, entire window  
	Viewport* vp = mWindow->addViewport(mCamera);  
	vp->setBackgroundColour(ColourValue(0.2,0.3,0.3));  
	// Alter the camera aspect ratio to match the viewport  
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));  
}  


void COgreBaseApp::createScene(void)  
{  

	//添加背景  
	//mSceneMgr->setSkyDome(true, "Examples/CloudySky",6, 8);  

	/*Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");  

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();  
	
	headNode->attachObject(ogreHead);  */

	// Set ambient light  
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));  

	// Create a light  
	//Ogre::Light* l = mSceneMgr->createLight("MainLight");  
	//l->setPosition(20,80,50);  
}  

//创建帧监听  
void COgreBaseApp::createFrameListener(void)  
{  
//	  mListener= new CMyFrameListener(mWindow, mCamera, mSceneMgr, light);  
	  //mRoot->addFrameListener(mListener);  
	 
}  