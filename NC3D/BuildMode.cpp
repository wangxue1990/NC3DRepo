#include "StdAfx.h"
#include "BuildMode.h"


CBuildMode::CBuildMode(COgreBaseApp *ogapp,HWND hw):ogreapp(ogapp),m_hwnd(hw)
{
	mCamera=ogapp->getCamera();
	mRaySceneQuery = ogapp->getSceneManager()->createRayQuery(Ray()); // 创建射线场景查询 
	mCurrentObject=NULL;
	
}


CBuildMode::~CBuildMode(void)
{
}
//#include <windows.h>
void CBuildMode::VectorNormalize(Vector3 &v)
{
	double vm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (vm > 0)
	{
		v.x = v.x / vm;
		v.y = v.y / vm;
		v.z = v.z / vm;
	}
}
void CBuildMode::_TransPoint2V3(CPoint p2,Ogre::Vector3 &v)
{
	CRect rt;
	GetClientRect(m_hwnd,&rt);//获取视图窗口大小
	double w = rt.Width();
	double h = rt.Height();
	Vector2 cen;
	this->World2Screen(oriPos,cen);
	if (cen.x > w *0.5) 
	{
		v.x = (p2.x - cen.x) / cen.x;
	}else
	{
		v.x = (p2.x - cen.x) / (w - cen.x);
	}
	if (cen.y > h *0.5)
	{
		v.y = (cen.y - p2.y) / cen.y;
	}else
	{
		v.y = (cen.y - p2.y) / (h - cen.y);
	}
	double d = sqrt(v.x * v.x + v.y * v.y);
	double r ;
	if (d < 1.0)
	{
		r = d;
	}else
	{
		r = 1.0;
	}
	v.z = cos(3.1415926 / 2 * r);
	VectorNormalize(v);
}

void CBuildMode::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_mouseLast = point;
	_TransPoint2V3(point,startPos);
}

void CBuildMode::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_mouseLast = point;
	if(mCurrentObject)  {
		mCurrentObject->showBoundingBox(false);  
		mCurrentObject=NULL;
	}
	CRect rt;
	GetClientRect(m_hwnd,&rt);//获取视图窗口大小
	int a=rt.Width();
	int b=rt.Height();
	//transPoint2V3(point,startPos);
	Ray mouseRay = mCamera->getCameraToViewportRay(float(point.x)/float(a),  (float(point.y))/float(b));  
	mRaySceneQuery->setRay(mouseRay);  
	mRaySceneQuery->setSortByDistance(true);  

	// Execute query  
	RaySceneQueryResult &result = mRaySceneQuery->execute();  
	RaySceneQueryResult::iterator itr;  

	// Get results, create a node/entity on the position  
	for ( itr = result.begin(); itr != result.end(); itr++ )  
	{  
		Ogre::Vector3 v3;
		if (itr->movable )//&& itr->movable->getName().substr(0, 5) != "tile[")  
		{  
			mCurrentObject = itr->movable->getParentSceneNode();  
			break;  
		}
	}

	if(mCurrentObject)  
		mCurrentObject->showBoundingBox(true); 

}


void CBuildMode::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nFlags & MK_RBUTTON)  
	{  
		CPoint mouseDiff = point - m_mouseLast;  
		m_mouseLast = point;  
		_TransPoint2V3(point,endPos);
		Ogre::Quaternion qu=startPos.getRotationTo(endPos);
		Ogre::Vector3 camPos= mCamera->getPosition();
		//oriPos
		camPos = oriPos+(qu*(camPos-oriPos));
		mCamera->setPosition(camPos);
		mCamera->lookAt(oriPos);
		startPos = endPos;
	}
	if(nFlags & MK_MBUTTON)  
	{  
		Ogre::Real factor;  
		factor = mCamera->getPosition().length()/400.0f;  
		mCamera->moveRelative(Ogre::Vector3((m_mouseLast.x - point.x)*factor,  
		(point.y -m_mouseLast.y)*factor,0));
		m_mouseLast=point;
		//mouse mid button up 的时候屏幕中间发射一条射线，求出与场景的焦点作为旋转中心点。
	}
}



BOOL CBuildMode::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	Ogre::Vector3 v3=mCamera->getPosition()-oriPos;
	v3=v3/v3.length();
	mCamera->move(v3*float(-zDelta*0.5));
	return TRUE;
}

void CBuildMode::OnMButtonUp(UINT nFlags, CPoint point)
{
	//if (nFlags&MK_MBUTTON)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		m_mouseLast = point;
		CRect rt;
		GetClientRect(m_hwnd,&rt);//获取视图窗口大小
		int a=rt.Width();
		int b=rt.Height();

		Ray mouseRay = mCamera->getCameraToViewportRay(float(point.x)/float(a),  (float(point.y))/float(b));  
		mRaySceneQuery->setRay(mouseRay);  
		mRaySceneQuery->setSortByDistance(true);  

		// Execute query  
		RaySceneQueryResult &result = mRaySceneQuery->execute();  
		RaySceneQueryResult::iterator itr;  

		// Get results, create a node/entity on the position  
		for ( itr = result.begin(); itr != result.end(); itr++ )  
		{
			Ogre::Vector3 v3;
			if (itr->movable )
			{
				mCamera->lookAt(mouseRay.getPoint(itr->distance));
				oriPos = mouseRay.getPoint(itr->distance);
				break;  
			}
		}
	}
}

void CBuildMode::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (nFlags&MK_MBUTTON)
	//{
	//	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//	m_mouseLast = point;
	//	CRect rt;
	//	GetClientRect(m_hwnd,&rt);//获取视图窗口大小
	//	int a=rt.Width();
	//	int b=rt.Height();
	//	
	//	Ray mouseRay = mCamera->getCameraToViewportRay(float(point.x)/float(a),  (float(point.y))/float(b));  
	//	mRaySceneQuery->setRay(mouseRay);  
	//	mRaySceneQuery->setSortByDistance(true);  

	//	// Execute query  
	//	RaySceneQueryResult &result = mRaySceneQuery->execute();  
	//	RaySceneQueryResult::iterator itr;  

	//	// Get results, create a node/entity on the position  
	//	for ( itr = result.begin(); itr != result.end(); itr++ )  
	//	{
	//		Ogre::Vector3 v3;
	//		if (itr->movable )
	//		{
	//			mCamera->lookAt(mouseRay.getPoint(itr->distance));
	//			oriPos = mouseRay.getPoint(itr->distance);
	//			break;  
	//		}
	//	}
	//}
}

//void  CBuildMode:: local2World(const Vector3 locP, Vector3 &worldP, const SceneNode* node)  
//{  
//	Matrix4 worldMat;  
//	node->getWorldTransforms(&worldMat); //获取该物体的世界坐标矩阵  
//	worldP = worldMat * locP;  
//}   


	bool  CBuildMode::World2Screen(Vector3 objPos, Vector2& screenPos)  
{  
	Matrix4 viewMat = mCamera->getViewMatrix();  
	Matrix4 projMat = mCamera->getProjectionMatrix();  


	Vector4 inP = Vector4(objPos.x, objPos.y, objPos.z ,1.0);  
	Vector4 outP = viewMat * inP;  
	outP = projMat * outP;  

	if(outP.w <= EPSILON)  
		return false;  

	outP.x /= outP.w;  
	outP.y /= outP.w;  
	outP.z /= outP.w;  

	//[-1,1]->[0,1]  
	outP.x = outP.x*0.5 + 0.5;  
	outP.y = outP.y*0.5 + 0.5;  
	outP.z = outP.z*0.5 + 0.5;  

	CRect rt;
	GetClientRect(m_hwnd,&rt);//获取视图窗口大小
	outP.x = outP.x * rt.Width();  
	outP.y = (1-outP.y) * rt.Height();  

	screenPos.x = outP.x;  
	screenPos.y = outP.y;  
	return true;  
}  