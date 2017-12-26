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

// NC3DView.h : CNC3DView 类的接口
//

#pragma once
#include "OgreBaseApp.h"
#include "OperateMode.h"
#include "BuildMode.h"
class CNC3DView : public CView
{
protected: // 仅从序列化创建
	CNC3DView();
	DECLARE_DYNCREATE(CNC3DView)

// 特性
public:
	CNC3DDoc* GetDocument() const;
	COgreBaseApp mOgreApp;//ogre 主类  
	bool m_firstDraw;//初次渲染  
	COperateMode * opMode;
	CBuildMode *opMode_build;
// 操作
public:
	
	
	bool bIni;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	void CreatEditingPlane();
	void CreatLane(Vector3 p1,Vector3 p2,Ogre::String name);
	void CreatNode(Vector3 p);
// 实现
public:
	virtual ~CNC3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // NC3DView.cpp 中的调试版本
inline CNC3DDoc* CNC3DView::GetDocument() const
   { return reinterpret_cast<CNC3DDoc*>(m_pDocument); }
#endif

