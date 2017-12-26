// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// NC3DView.cpp : CNC3DView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NC3D.h"
#endif

#include "NC3DDoc.h"
#include "NC3DView.h"
#include "ogre.h"  
#include "OgreConfigFile.h"  
#include "OgreFrameListener.h"  
#include "OgreStringConverter.h"  
#include "OIS.h"  
#include "CMyFrameListener.h"  
#include <SdkTrays.h>  
#include <SdkCameraMan.h>

#include "BuildMode.h"

using namespace Ogre;
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
#define OGRE_DEBUG_MEMORY_MANAGER 1

// CNC3DView

IMPLEMENT_DYNCREATE(CNC3DView, CView)

	BEGIN_MESSAGE_MAP(CNC3DView, CView)
		// 标准打印命令
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CNC3DView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_TIMER()
		ON_WM_ERASEBKGND()
		ON_COMMAND(ID_BUTTON_GameMode, &CNC3DView::OnButtonGameMode)
		ON_COMMAND(ID_BUTTON_BuildMode, &CNC3DView::OnButtonBuildmode)
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_WM_LBUTTONDOWN()
		ON_WM_MBUTTONDOWN()
		ON_WM_SIZE()
		ON_WM_RBUTTONDOWN()
		ON_WM_MBUTTONUP()
	END_MESSAGE_MAP()

	// CNC3DView 构造/析构

	CNC3DView::CNC3DView()
	{
		// TODO: 在此处添加构造代码
		//_CrtSetBreakAlloc(558888);
		bIni=false;
		opMode=NULL;
		opMode_build=NULL;
	}

	CNC3DView::~CNC3DView()
	{
	}
	void CNC3DView::CreatEditingPlane()
	{
		ManualObject* manual = mOgreApp.getSceneManager()->createManualObject("plane");
		manual->begin("BaseWhiteNoLighting",  RenderOperation::OT_LINE_LIST);
		bool test = false;
		//平面
		//指定定点位置


		Ogre::Real z=0.0;
		long cishu=100;
		Real jg=5.0;
		Real fudu=jg*(Real)cishu;
		Vector3 center(0.0,0.0,z);
		Vector3 zuoxia;
		zuoxia.x=center.x-fudu*0.5;
		zuoxia.y=center.y-fudu*0.5;
		zuoxia.z=center.z;
		for (long i=0.0;i<cishu;i++)
		{
			Real dd=i*jg;
			manual->position(zuoxia.x, zuoxia.y+dd, zuoxia.z);
			manual->colour(ColourValue(0.0,1.0,0.0));
			manual->position( zuoxia.x+fudu, zuoxia.y+dd, zuoxia.z);
			manual->colour(ColourValue(0.0,1.0,0.0));
			manual->position( zuoxia.x+dd, zuoxia.y, zuoxia.z);
			manual->colour(ColourValue(0.0,1.0,0.0));
			manual->position( zuoxia.x+dd, zuoxia.y+fudu, zuoxia.z);
			manual->colour(ColourValue(0.0,1.0,0.0));
		}
		manual->end(); //结束定点设置。这里的begin和end对应于d3d中读写定点缓存时的loack 与 unlock !
		//将手动对象创建为mesh
		manual->convertToMesh("planeMesh");
		Entity * tempEntity =  mOgreApp.getSceneManager()->createEntity("plane","planeMesh");
		SceneNode * sce = mOgreApp.getSceneManager()->getRootSceneNode()->createChildSceneNode();
		sce->attachObject(tempEntity);
		sce->setPosition(0.0,0.0,0.0);
	}
	void CNC3DView::CreatLane(Vector3 p1,Vector3 p2,Ogre::String name)
	{
		ManualObject* lManualObject = mOgreApp.getSceneManager()->createManualObject(name);
		lManualObject->begin("BaseWhiteNoLighting");
		Ogre::Real Len = p1.distance(p2);

		Ogre::Real w,h;
		w=4.0;//宽
		h=4.0;//高
		Vector3 b,e;

		b.x = 0.0; b.y = 0.0; b.z = 0.0;
		e.x = 0.0; e.y = 0.0; e.z = -Len;

		lManualObject->position(b.x, b.y+0.5*w, b.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));  
		lManualObject->position(b.x, b.y-0.5*w, b.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));  
		lManualObject->position(b.x+h, b.y-0.5*w, b.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));  
		lManualObject->position(b.x+h, b.y+0.5*w, b.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));  

		lManualObject->position(e.x, e.y+0.5*w, e.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));  
		lManualObject->position(e.x, e.y-0.5*w, e.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));  
		lManualObject->position(e.x+h, e.y-0.5*w, e.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));  
		lManualObject->position(e.x+h, e.y+0.5*w, e.z);// a vertex  
		lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));  
		// face behind / front  
		lManualObject->triangle(0,1,2);  
		lManualObject->triangle(2,3,0);  
		lManualObject->triangle(4,6,5);  
		lManualObject->triangle(6,4,7);  

		// face top / down  
		lManualObject->triangle(0,4,5);  
		lManualObject->triangle(5,1,0);  
		lManualObject->triangle(2,6,7);  
		lManualObject->triangle(7,3,2);  

		// face left / right  
		lManualObject->triangle(0,7,4);  
		lManualObject->triangle(7,0,3);  
		lManualObject->triangle(1,5,6);  
		lManualObject->triangle(6,2,1);     

		lManualObject->end();  
		//Ogre::String lNameOfTheMesh = "MeshOfLane";  
		lManualObject->convertToMesh(name);  
		Entity* entity = mOgreApp.getSceneManager()->createEntity(name);  
		SceneNode * sce = mOgreApp.getSceneManager()->getRootSceneNode()->createChildSceneNode();
		sce->setPosition(p1);
		sce->lookAt(p2,Ogre::Node::TransformSpace::TS_WORLD);
		sce->attachObject(entity);
	};


	BOOL CNC3DView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式

		return CView::PreCreateWindow(cs);
	}




	// CNC3DView 绘制

	void CNC3DView::OnDraw(CDC* /*pDC*/)
	{
		CNC3DDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		// TODO: 在此处为本机数据添加绘制代码  
		//Ogre渲染  
		try {  
			if(m_firstDraw)  
			{  
				m_firstDraw = false;  
				CRect   rect;  
				GetClientRect(&rect);  
				mOgreApp.go(rect,m_hWnd);//传入MFC主窗口句柄
				CreatEditingPlane();
				Vector3 p1(0.0,0.0,0.0);Vector3 p2(0.0,0.0,200.0);
				Vector3 p3(100.0,-100.0,100.0);Vector3 p4(100.0,100.0,-50.0);
				Vector3 p5(300.0,-100.0,-100.0);Vector3 p6(300.0,100.0,-50.0);
				Vector3 p7(400.0,0.0,0.0);Vector3 p8(400.0,0.0,200.0);
				Vector3 p9(100.0,0.0,0.0);Vector3 p10(300.0,0.0,0.0);

				Ogre::String ss;
				ss="lanemesh";

				CreatLane(p2,p1,ss.append("1"));
				CreatLane(p1,p9,ss.append("1"));
				CreatLane(p9,p10,ss.append("1"));
				CreatLane(p10,p7,ss.append("1"));
				CreatLane(p7,p8,ss.append("1"));
				CreatLane(p9,p4,ss.append("1"));
				CreatLane(p4,p6,ss.append("1"));
				CreatLane(p6,p10,ss.append("1"));
				CreatLane(p9,p3,ss.append("1"));
				CreatLane(p3,p5,ss.append("1"));
				CreatLane(p5,p10,ss.append("1"));

				opMode_build = new CBuildMode(&mOgreApp,m_hWnd);
				SetTimer(1,20,0);  
			}  

			mOgreApp.getRoot()->renderOneFrame();  
		} catch( Ogre::Exception& e ) {  
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32  
			//MessageBox(  e.getFullDescription().c_str(), _T("An exception has occured!"), MB_OK | MB_ICONERROR | MB_TASKMODAL);  
#else  
			std::cerr << "An exception has occured: " <<  
				e.getFullDescription().c_str() << std::endl;  
#endif  
		} 
	}


	// CNC3DView 打印


	void CNC3DView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CNC3DView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// 默认准备
		return DoPreparePrinting(pInfo);
	}

	void CNC3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 添加额外的打印前进行的初始化过程
	}

	void CNC3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 添加打印后进行的清理过程
	}

	void CNC3DView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CNC3DView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CNC3DView 诊断

#ifdef _DEBUG
	void CNC3DView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CNC3DView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CNC3DDoc* CNC3DView::GetDocument() const // 非调试版本是内联的
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNC3DDoc)));
		return (CNC3DDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CNC3DView 消息处理程序


	void CNC3DView::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (1 == nIDEvent)  
		{  
			mOgreApp.getRoot()->renderOneFrame();  
		}  
		CView::OnTimer(nIDEvent);
	}


	BOOL CNC3DView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		return TRUE;  
		//<span style="white-space:pre">  </span>//return CView::OnEraseBkgnd(pDC);  
		//return CView::OnEraseBkgnd(pDC);
	}


	void CNC3DView::OnButtonGameMode()
	{
		// TODO: 在此添加命令处理程序代码

		//m_ogreApp.getRoot()->addFrameListener(m_ogreApp.mListener); 
	}


	void CNC3DView::OnButtonBuildmode()
	{
		// TODO: 在此添加命令处理程序代码
		//m_ogreApp.getRoot()->removeFrameListener(m_ogreApp.mListener);
		opMode = (COperateMode * )opMode_build;
	}


	void CNC3DView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnMouseMove(nFlags,point);
		}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL CNC3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnMouseWheel(nFlags,zDelta,pt);
		}
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void CNC3DView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnLButtonDown(nFlags,point);
		}
		CView::OnLButtonDown(nFlags, point);
	}


	void CNC3DView::OnMButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnMButtonDown(nFlags,point);
		}
		CView::OnMButtonDown(nFlags, point);
	}


	void CNC3DView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);
		//CRect rt;
		//::GetClientRect(m_hWnd,&rt);
		mOgreApp.OnSize(cx,cy);
		// TODO: 在此处添加消息处理程序代码
	}


	void CNC3DView::OnRButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnRButtonDown(nFlags,point);
		}
		CView::OnRButtonDown(nFlags, point);
	}


	void CNC3DView::OnMButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if (NULL!=opMode)
		{
			opMode->OnMButtonUp(nFlags,point);
		}
		CView::OnMButtonUp(nFlags, point);
	}
