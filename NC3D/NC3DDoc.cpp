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

// NC3DDoc.cpp : CNC3DDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NC3D.h"
#endif

#include "NC3DDoc.h"

#include <propkey.h>

#include "tinyxml/tinyxml.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// CNC3DDoc

IMPLEMENT_DYNCREATE(CNC3DDoc, CDocument)

BEGIN_MESSAGE_MAP(CNC3DDoc, CDocument)
	//ON_COMMAND(ID_FILE_OPEN, &CNC3DDoc::OnFileOpen)
	ON_COMMAND(ID_BUTTON_OpenXML, &CNC3DDoc::OnButtonOpenxml)
END_MESSAGE_MAP()


// CNC3DDoc 构造/析构

CNC3DDoc::CNC3DDoc()
{
	// TODO: 在此添加一次性构造代码

}

CNC3DDoc::~CNC3DDoc()
{
}

BOOL CNC3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CNC3DDoc 序列化

void CNC3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		ar<<9<<8<<7;
	}
	else
	{
		// TODO: 在此添加加载代码
		
		ar>>a>>b>>c;
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CNC3DDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CNC3DDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CNC3DDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNC3DDoc 诊断

#ifdef _DEBUG
void CNC3DDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNC3DDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNC3DDoc 命令


//void CNC3DDoc::OnFileOpen()
//{
//	// TODO: 在此添加命令处理程序代码
//	AfxMessageBox("niha");
//	
//}


void CNC3DDoc::OnButtonOpenxml()
{
	// TODO: 在此添加命令处理程序代码
	//获取打开文件路径
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = "";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "文件 (*.xml;)|*.doc;*.*||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);  
	openFileDlg.GetOFN().lpstrInitialDir = "";  
	INT_PTR result = openFileDlg.DoModal();  
	CString filePath = defaultDir + "";  
	if(result == IDOK) {  
		filePath = openFileDlg.GetPathName();  
	}  
	//string file = ;

	const char * xmlFile = filePath.GetBuffer();
	TiXmlDocument doc(xmlFile);
	if (!doc.LoadFile(TIXML_DEFAULT_ENCODING))
	{
		AfxMessageBox("打开文件失败。");
		return;
	}

	int mMaxBranchIndex = 0;
	FLRVentTopoNode node;

	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* BranchInfoElement = rootElement->FirstChildElement(Vent_Para_XML_Tag::BranchInfo);
	if (BranchInfoElement)
	{
		TiXmlElement * LaneElement = BranchInfoElement->FirstChildElement(Vent_Para_XML_Tag::Lane);
		for (; LaneElement != NULL; LaneElement = LaneElement->NextSiblingElement())
		{
			FLRVentTopoLaneway *lane = new FLRVentTopoLaneway;

			TiXmlElement * element = LaneElement->FirstChildElement(Vent_Para_XML_Tag::LaneType);
			if ( element )
				lane->type = EVentTopoLanewayType(atoi(element->GetText()));

			TiXmlElement* LaneGUIDElement = LaneElement->FirstChildElement(Vent_Para_XML_Tag::LaneGuid);
			if (LaneGUIDElement){
				//读取ABCD
				element = LaneGUIDElement->FirstChildElement(Vent_Para_XML_Tag::GuidA);
				lane->guid.A = strtoul(element->GetText(), NULL, 10);
				element = LaneGUIDElement->FirstChildElement(Vent_Para_XML_Tag::GuidB);
				lane->guid.B = strtoul(element->GetText(), NULL, 10);
				element = LaneGUIDElement->FirstChildElement(Vent_Para_XML_Tag::GuidC);
				lane->guid.C = strtoul(element->GetText(), NULL, 10);
				element = LaneGUIDElement->FirstChildElement(Vent_Para_XML_Tag::GuidD);
				lane->guid.D = strtoul(element->GetText(), NULL, 10);
			}

			TiXmlElement* LaneSegments = LaneElement->FirstChildElement(Vent_Para_XML_Tag::Segments);
			TiXmlElement* SegmentElement = LaneSegments->FirstChildElement(Vent_Para_XML_Tag::Segment);
			for (; SegmentElement != NULL; SegmentElement = SegmentElement->NextSiblingElement())
			{
				FLRVentTopoSegment *cross = new FLRVentTopoSegment;

				TiXmlElement * branches = SegmentElement->FirstChildElement(Vent_Para_XML_Tag::Branches);
				TiXmlElement * branch = branches->FirstChildElement(Vent_Para_XML_Tag::Branch);
				for (; branch != NULL; branch = branch->NextSiblingElement())
				{
					FLRVentBranch * tmp = new FLRVentBranch;
					tmp->info.Guid = lane->guid;
					tmp->info.id = ++mMaxBranchIndex;
					TiXmlElement * para = branch->FirstChildElement(Vent_Para_XML_Tag::Para);
					element = para->FirstChildElement(Vent_Para_XML_Tag::Place);
					tmp->info.Place = atoi(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::BranchType);
					tmp->info.Type = atoi(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::R);
					tmp->info.R = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::rR);
					tmp->info.rR = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::Alpha);
					tmp->info.Alpha = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::Length);
					tmp->info.Length = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::Area);
					tmp->info.Area = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::U);
					tmp->info.U = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::rho);
					tmp->info.rho = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::Q);
					tmp->info.Q = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::H);
					tmp->info.H = atof(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::StructType);
					tmp->info.StructType = atoi(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::WindType);
					tmp->info.WindType = atoi(element->GetText());
					element = para->FirstChildElement(Vent_Para_XML_Tag::Direction);
					tmp->info.Direction = atoi(element->GetText());

					TiXmlElement * points = branch->FirstChildElement(Vent_Para_XML_Tag::Points);
					TiXmlElement * point = points->FirstChildElement(Vent_Para_XML_Tag::Point);
					for (; point != NULL; point = point->NextSiblingElement())
					{
						element = point->FirstChildElement(Vent_Para_XML_Tag::X);
						node.info.Position.x = atof(element->GetText());
						element = point->FirstChildElement(Vent_Para_XML_Tag::Y);
						node.info.Position.y = atof(element->GetText());
						element = point->FirstChildElement(Vent_Para_XML_Tag::Z);
						node.info.Position.z = atof(element->GetText());
						element = point->FirstChildElement(Vent_Para_XML_Tag::PointType);
						node.info.type = EVentNodeType(atoi(element->GetText()));

						int index = AddAndGetPoint(node.info.Position);
						if (index != -1)
						{
							node.neighbors = mVentTopoNodeMap[index]->neighbors;//wsy
							*(mVentTopoNodeMap[index]) = node;//wrong:do this delete node's neighbors
							mVentTopoNodeMap[index]->branches.push_back(tmp);
							tmp->points.push_back(index);
						}
					}
					//neighbors
					for (int32 index = 0; index < tmp->points.size() - 1; ++index)
					{
						mVentTopoNodeMap[tmp->points[index]]->neighbors.push_back(tmp->points[index+1]);
						mVentTopoNodeMap[tmp->points[index+1]]->neighbors.push_back(tmp->points[index]);
					}

					tmp->info.PointNum = tmp->points.size();
					tmp->info.SegmentNum = lane->segments.size();
					tmp->info.BranchNum = cross->branches.size();
					cross->branches.push_back(tmp);

					mBranchInfoMap[tmp->info.id] = tmp;
				}
				lane->segments.push_back(cross);
			}
			mVentTopoLanewayMap[lane->guid] = lane;
		}
	}

	//check branch info place
	for (auto & lane : mVentTopoLanewayMap)
	{
		for (auto & segment : lane.second->segments)
		{
			for (auto & branch : segment->branches)
			{
				if (mVentTopoNodeMap[branch->points[0]]->neighbors.size() == 1 ||
					mVentTopoNodeMap[branch->points[branch->points.size() - 1]]->neighbors.size() == 1)
				{//branch->info.Place = EPositiondll::Eother;
					if (branch->info.Place == EPositiondll::Einteral)
					{
						branch->info.Place = EPositiondll::Eother;
					}
				}
				else{
					if (branch->info.Place == EPositiondll::Eother)
					{
						branch->info.Place = EPositiondll::Einteral;
					}
				}
			}
		}
	}

	TiXmlElement* NodeInfoElement = rootElement->FirstChildElement(Vent_Para_XML_Tag::NodeInfo);
	if (NodeInfoElement)
	{
		TiXmlElement * pnode = NodeInfoElement->FirstChildElement(Vent_Para_XML_Tag::Node);
		for (; pnode != NULL; pnode = pnode->NextSiblingElement())
		{
			NodeInfo tmp;
			TiXmlElement * point = pnode->FirstChildElement(Vent_Para_XML_Tag::Point);
			if (point)
			{
				TiXmlElement * element = point->FirstChildElement(Vent_Para_XML_Tag::X);
				tmp.Position.x = atof(element->GetText());
				element = point->FirstChildElement(Vent_Para_XML_Tag::Y);
				tmp.Position.y = atof(element->GetText());
				element = point->FirstChildElement(Vent_Para_XML_Tag::Z);
				tmp.Position.z = atof(element->GetText());				
			}
			TiXmlElement *element = pnode->FirstChildElement(Vent_Para_XML_Tag::T);
			tmp.T = atof(element->GetText());
			element = pnode->FirstChildElement(Vent_Para_XML_Tag::fi);
			tmp.fi = atof(element->GetText());
			element = pnode->FirstChildElement(Vent_Para_XML_Tag::pa);
			tmp.pa = atof(element->GetText());
			element = pnode->FirstChildElement(Vent_Para_XML_Tag::rho);
			tmp.rho = atof(element->GetText());

			uint32 index = AddAndGetPoint(tmp.Position);
			tmp.type = mVentTopoNodeMap[index]->info.type;//do not override
			mVentTopoNodeMap[index]->info = tmp;
		}
	}

	TiXmlElement * FanInfoElement = rootElement->FirstChildElement(Vent_Para_XML_Tag::FanInfo);
	if (FanInfoElement)
	{
		TiXmlElement *pFan = FanInfoElement->FirstChildElement(Vent_Para_XML_Tag::Fan);
		for (; pFan != NULL; pFan = pFan->NextSiblingElement())
		{
			FanInfo tmp;
			TiXmlElement * point = pFan->FirstChildElement(Vent_Para_XML_Tag::Point);
			if (point)
			{
				TiXmlElement * element = point->FirstChildElement(Vent_Para_XML_Tag::X);
				tmp.pos.x = atof(element->GetText());
				element = point->FirstChildElement(Vent_Para_XML_Tag::Y);
				tmp.pos.y = atof(element->GetText());
				element = point->FirstChildElement(Vent_Para_XML_Tag::Z);
				tmp.pos.z = atof(element->GetText());
			}
			TiXmlElement * element = pFan->FirstChildElement(Vent_Para_XML_Tag::xh);
			wcscpy(tmp.xh, StringUtil::MultiByteToWid(element->GetText()).c_str());
			element = pFan->FirstChildElement(Vent_Para_XML_Tag::YType);
			wcscpy(tmp.YType, StringUtil::MultiByteToWid(element->GetText()).c_str());
			element = pFan->FirstChildElement(Vent_Para_XML_Tag::FType);
			wcscpy(tmp.FType, StringUtil::MultiByteToWid(element->GetText()).c_str());
			element = pFan->FirstChildElement(Vent_Para_XML_Tag::jd);
			tmp.jd = atof(element->GetText());
			element = pFan->FirstChildElement(Vent_Para_XML_Tag::JH);
			tmp.JH = atof(element->GetText());
			element = pFan->FirstChildElement(Vent_Para_XML_Tag::ZS);
			tmp.ZS = atof(element->GetText());

			TiXmlElement *DsesElement = pFan->FirstChildElement(Vent_Para_XML_Tag::Dses);
			TiXmlElement * ds = DsesElement->FirstChildElement(Vent_Para_XML_Tag::Ds);
			for (int i = 0; ds != NULL; ds = ds->NextSiblingElement(), ++i)
			{
				element = ds->FirstChildElement(Vent_Para_XML_Tag::qf);
				tmp.qf[i] = atof(element->GetText());
				element = ds->FirstChildElement(Vent_Para_XML_Tag::pf);
				tmp.pf[i] = atof(element->GetText());
				tmp.NumDs = i + 1;
			}

			int index = AddAndGetPoint(tmp.pos);
			if (index != -1)
			{
				tmp.nof = mVentTopoNodeMap[index]->branches[0]->info.id;
				mVentTopoNodeMap[index]->info.type = eFan;
				mFanInfoMap[index] = tmp;
			}
		}
	}

	TiXmlElement * GlobalElement = rootElement->FirstChildElement(Vent_Para_XML_Tag::GlobalPara);
	if (GlobalElement)
	{
		TiXmlElement * element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::MaxIterate);
		mGlobalPara.m_MaxIterate = atoi(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Werr);
		mGlobalPara.m_Werr = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Presidual);
		mGlobalPara.m_Presidual = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Xsj);
		mGlobalPara.m_Xsj = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Patm);
		mGlobalPara.m_Patm = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Tr);
		mGlobalPara.m_Tr = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Dfai);
		mGlobalPara.m_Dfai = atof(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::ReferenceNode);
		mGlobalPara.m_ReferenceNode = atol(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Iatm);
		mGlobalPara.m_Iatm = atol(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::Nvpn);
		mGlobalPara.m_Nvpn = atoi(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::IscalNodePressure);
		mGlobalPara.m_IscalNodePressure = atoi(element->GetText());
		element = GlobalElement->FirstChildElement(Vent_Para_XML_Tag::AutoIniQ);
		mGlobalPara.m_AutoIniQ = atoi(element->GetText());
	}

	if (mVentTopoLanewayMap.size())
		bLoadFile = true;
}
