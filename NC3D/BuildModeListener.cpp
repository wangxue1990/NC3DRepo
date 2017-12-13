#include "StdAfx.h"
#include "BuildModeListener.h"


CBuildModeListener::CBuildModeListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light *l): mMouse(0), 
	mKeyboard(0), mInputManager(0), mWindow(win), mCamera(cam), light(l)
{
	mRBtdown = false;
	mCamNode=cam->getParentSceneNode();
	mSceneMgr = sceneMgr;


	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;


	mCameraMan = new OgreBites::SdkCameraMan(mCamera);  


	windowHnd = (size_t )AfxGetMainWnd()->GetSafeHwnd(); // 这里这个窗口句柄就是传入的MFC主窗口
	windowHndStr << windowHnd;
	// OIS的窗口必须要顶层窗口,所以只有传MFC的主窗口给他,传view就不行
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


	// 设置鼠标显示和非游戏独占,这样鼠标可以显示在屏幕上并可以移动到窗口外
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	// 键盘非游戏独占
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	mInputManager = OIS::InputManager::createInputSystem(pl);


	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));


	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
}


CBuildModeListener::~CBuildModeListener(void)
{
	mInputManager->destroyInputObject(mMouse);
	mInputManager->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = 0;
}




bool CBuildModeListener::frameRenderingQueued(const Ogre::FrameEvent& e){

	mMouse->capture();
	mKeyboard->capture();
	mCameraMan->frameRenderingQueued(e); 
	return true;
}


bool CBuildModeListener::mouseMoved(const OIS::MouseEvent &e)
{
	if (mRBtdown)
	{
		
		mCameraMan->injectMouseMove(e);
	}

	return true;
}


bool CBuildModeListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	//	mCameraMan->injectMouseDown(e, id);
	if (id == OIS::MB_Left)
	{
		m_mouseLast.x = e.state.X;
		OIS::Axis a;
		
		if(mCurrentObject)  {
			mCurrentObject->showBoundingBox(false);  
			mCurrentObject=NULL;
		}
		CRect rt;
		GetClientRect(&rt);//获取视图窗口大小
		//CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();  
		int a=rt.Width();
		int b=rt.Height();
		transPoint2V3(point,startPos);
		Ray mouseRay = m_pCamera->getCameraToViewportRay(float(point.x)/float(a),  (float(point.y))/float(b));  
		mRaySceneQuery->setRay(mouseRay);  
		mRaySceneQuery->setSortByDistance(true);  

		// Execute query  
		RaySceneQueryResult &result = mRaySceneQuery->execute();  
		RaySceneQueryResult::iterator itr;  

		// Get results, create a node/entity on the position  
		for ( itr = result.begin(); itr != result.end(); itr++ )  
		{  
			Ogre::Vector3 v3;
			//v3.
			if (itr->movable )//&& itr->movable->getName().substr(0, 5) != "tile[")  
			{  
				mCurrentObject = itr->movable->getParentSceneNode();  
				break;  
			} // if  
		} // for  

		if(mCurrentObject)  
			mCurrentObject->showBoundingBox(true); 

		mRBtdown = true;
	}

	return true;
}


bool CBuildModeListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{


	//	mCameraMan->injectMouseUp(e, id);
	if (id == OIS::MB_Right)
	{
		mRBtdown = false;
	}
	return true;
}
//键盘响应
bool CBuildModeListener::keyPressed(const OIS::KeyEvent &e)
{

	mCameraMan->injectKeyDown(e);


	return true;
}


bool CBuildModeListener::keyReleased(const OIS::KeyEvent &e)
{
	mCameraMan->injectKeyUp(e);
	return true;
}

void CBuildModeListener::	transPoint2V3(CPoint p2,Ogre::Vector3 &v3)
{
	float r = 1.0;
	CRect rt;
	GetClientRect(&rt);//获取视图窗口大小

	float w = (float)rt.Width();
	float h = (float)rt.Height();
	float x   = -(p2.x - w*0.5)  / (r*w*0.5);
	float y   =  (p2.y  - h*0.5) / (r*h*0.5);
	float z   = 0.0f;

	float mag = x*x + y*y;

	if (mag > 1.0f)
	{
		float scale = 1.0f/sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
		z = sqrtf(1.0f - mag);
	v3.x=x;v3.y=y;v3.z=z;

}

