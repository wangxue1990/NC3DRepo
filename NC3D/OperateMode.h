#pragma once
#include "OgreBaseApp.h"
class COperateMode
{
public:
	COperateMode(void);
	~COperateMode(void);
public:
	virtual void OnRButtonUp(UINT nFlags, CPoint point){};
	virtual void OnLButtonDown(UINT nFlags, CPoint point){};
	virtual void OnMouseMove(UINT nFlags, CPoint point){};
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt){return TRUE;};
	virtual void OnMButtonDown(UINT nFlags, CPoint point){};
	virtual void OnRButtonDown(UINT nFlags, CPoint point){};
	virtual void OnMButtonUp(UINT nFlags, CPoint point){};
};

