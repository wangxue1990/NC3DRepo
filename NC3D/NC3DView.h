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

// NC3DView.h : CNC3DView ��Ľӿ�
//

#pragma once
#include "OgreBaseApp.h"
#include "OperateMode.h"
#include "BuildMode.h"
class CNC3DView : public CView
{
protected: // �������л�����
	CNC3DView();
	DECLARE_DYNCREATE(CNC3DView)

// ����
public:
	CNC3DDoc* GetDocument() const;
	COgreBaseApp mOgreApp;//ogre ����  
	bool m_firstDraw;//������Ⱦ  
	COperateMode * opMode;
	CBuildMode *opMode_build;
// ����
public:
	
	
	bool bIni;
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	void CreatEditingPlane();
	void CreatLane(Vector3 p1,Vector3 p2,Ogre::String name);
	void CreatNode(Vector3 p);
// ʵ��
public:
	virtual ~CNC3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonGameMode();
	afx_msg void OnButtonBuildmode();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // NC3DView.cpp �еĵ��԰汾
inline CNC3DDoc* CNC3DView::GetDocument() const
   { return reinterpret_cast<CNC3DDoc*>(m_pDocument); }
#endif

