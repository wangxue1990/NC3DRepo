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

// NC3DDoc.h : CNC3DDoc ��Ľӿ�
//


#pragma once
#include "GraphStruct.h"


namespace Vent_Para_XML_Tag
{
	static const char VentPara[] = "VentPara";

	//////////////////////////////////////////////////////////////////////////
	static const char BranchInfo[] = "BranchInfo";
	static const char Lane[] = "Lane";
	static const char LaneType[] = "LaneType";
	static const char LaneGuid[] = "LaneGuid";
	static const char GuidA[] = "GuidA";
	static const char GuidB[] = "GuidB";
	static const char GuidC[] = "GuidC";
	static const char GuidD[] = "GuidD";
	static const char Segments[] = "Segments";

	static const char Segment[] = "Segment";
	static const char Branches[] = "Branches";
	static const char Branch[] = "Branch";

	static const char Para[] = "Para";
	static const char ID[] = "ID";
	static const char Place[] = "Place";
	static const char BranchType[] = "BranchType";
	static const char R[] = "R";
	static const char rR[] = "rR";
	static const char Alpha[] = "Alpha";
	static const char Length[] = "Length";
	static const char Area[] = "Area";
	static const char U[] = "U";
	static const char rho[] = "rho";
	static const char Q[] = "Q";
	static const char H[] = "H";
	static const char StructType[] = "StructType";
	static const char WindType[] = "WindType";
	static const char Direction[] = "Direction";

	static const char Points[] = "Points";
	static const char Point[] = "Point";
	static const char X[] = "X";
	static const char Y[] = "Y";
	static const char Z[] = "Z";
	static const char PointType[] = "PointType";

	//////////////////////////////////////////////////////////////////////////
	static const char NodeInfo[] = "NodeInfo";
	static const char Node[] = "Node";
	static const char T[] = "T";
	static const char fi[] = "fi";
	static const char pa[] = "pa";

	//////////////////////////////////////////////////////////////////////////
	static const char FanInfo[] = "FanInfo";
	static const char Fan[] = "Fan";
	static const char xh[] = "xh";
	static const char YType[] = "YType";
	static const char FType[] = "FType";
	static const char jd[] = "jd";
	static const char JH[] = "JH";
	static const char ZS[] = "ZS";
	static const char Dses[] = "Dses";

	static const char Ds[] = "Ds";
	static const char qf[] = "qf";
	static const char pf[] = "pf";

	//////////////////////////////////////////////////////////////////////////
	static const char GlobalPara[] = "GlobalPara";
	static const char MaxIterate[] = "MaxIterate";
	static const char Werr[] = "Werr";
	static const char Presidual[] = "Presidual";
	static const char Xsj[] = "Xsj";
	static const char Patm[] = "Patm";
	static const char Tr[] = "Tr";
	static const char Dfai[] = "Dfai";
	static const char ReferenceNode[] = "ReferenceNode";
	static const char Iatm[] = "Iatm";
	static const char Nvpn[] = "Nvpn";
	static const char IscalNodePressure[] = "IscalNodePressure";
	static const char AutoIniQ[] = "AutoIniQ";
}


class CNC3DDoc : public CDocument
{
protected: // �������л�����
	CNC3DDoc();
	DECLARE_DYNCREATE(CNC3DDoc)

// ����
public:
	int a,b,c;
	Graph m_g;
// ����
public:
	void AddNode();
	void AddBranch();
	void GetGraph();
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CNC3DDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//afx_msg void OnFileOpen();
	afx_msg void OnButtonOpenxml();
};
