// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// NC3DView.cpp : CNC3DView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
		// ��׼��ӡ����
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

	// CNC3DView ����/����

	CNC3DView::CNC3DView()
	{
		// TODO: �ڴ˴���ӹ������
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
		//ƽ��
		//ָ������λ��


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
		manual->end(); //�����������á������begin��end��Ӧ��d3d�ж�д���㻺��ʱ��loack �� unlock !
		//���ֶ����󴴽�Ϊmesh
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
		w=4.0;//��
		h=4.0;//��
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
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ

		return CView::PreCreateWindow(cs);
	}




	// CNC3DView ����

	void CNC3DView::OnDraw(CDC* /*pDC*/)
	{
		CNC3DDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���  
		//Ogre��Ⱦ  
		try {  
			if(m_firstDraw)  
			{  
				m_firstDraw = false;  
				CRect   rect;  
				GetClientRect(&rect);  
				mOgreApp.go(rect,m_hWnd);//����MFC�����ھ��
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


	// CNC3DView ��ӡ


	void CNC3DView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CNC3DView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// Ĭ��׼��
		return DoPreparePrinting(pInfo);
	}

	void CNC3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
	}

	void CNC3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: ��Ӵ�ӡ����е��������
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


	// CNC3DView ���

#ifdef _DEBUG
	void CNC3DView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CNC3DView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CNC3DDoc* CNC3DView::GetDocument() const // �ǵ��԰汾��������
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNC3DDoc)));
		return (CNC3DDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CNC3DView ��Ϣ�������


	void CNC3DView::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (1 == nIDEvent)  
		{  
			mOgreApp.getRoot()->renderOneFrame();  
		}  
		CView::OnTimer(nIDEvent);
	}


	BOOL CNC3DView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		return TRUE;  
		//<span style="white-space:pre">  </span>//return CView::OnEraseBkgnd(pDC);  
		//return CView::OnEraseBkgnd(pDC);
	}


	void CNC3DView::OnButtonGameMode()
	{
		// TODO: �ڴ���������������

		//m_ogreApp.getRoot()->addFrameListener(m_ogreApp.mListener); 
	}


	void CNC3DView::OnButtonBuildmode()
	{
		// TODO: �ڴ���������������
		//m_ogreApp.getRoot()->removeFrameListener(m_ogreApp.mListener);
		opMode = (COperateMode * )opMode_build;
	}


	void CNC3DView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (NULL!=opMode)
		{
			opMode->OnMouseMove(nFlags,point);
		}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL CNC3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (NULL!=opMode)
		{
			opMode->OnMouseWheel(nFlags,zDelta,pt);
		}
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void CNC3DView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (NULL!=opMode)
		{
			opMode->OnLButtonDown(nFlags,point);
		}
		CView::OnLButtonDown(nFlags, point);
	}


	void CNC3DView::OnMButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		// TODO: �ڴ˴������Ϣ����������
	}


	void CNC3DView::OnRButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (NULL!=opMode)
		{
			opMode->OnRButtonDown(nFlags,point);
		}
		CView::OnRButtonDown(nFlags, point);
	}


	void CNC3DView::OnMButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (NULL!=opMode)
		{
			opMode->OnMButtonUp(nFlags,point);
		}
		CView::OnMButtonUp(nFlags, point);
	}
