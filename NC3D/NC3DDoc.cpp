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

// NC3DDoc.cpp : CNC3DDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CNC3DDoc ����/����

CNC3DDoc::CNC3DDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CNC3DDoc::~CNC3DDoc()
{
}

BOOL CNC3DDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CNC3DDoc ���л�

void CNC3DDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		ar<<9<<8<<7;
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		
		ar>>a>>b>>c;
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CNC3DDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CNC3DDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CNC3DDoc ���

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


// CNC3DDoc ����


//void CNC3DDoc::OnFileOpen()
//{
//	// TODO: �ڴ���������������
//	AfxMessageBox("niha");
//	
//}


void CNC3DDoc::OnButtonOpenxml()
{
	// TODO: �ڴ���������������
	//��ȡ���ļ�·��
	BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
	CString defaultDir = "";   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = "";         //Ĭ�ϴ򿪵��ļ���  
	CString filter = "�ļ� (*.xml;)|*.doc;*.*||";   //�ļ����ǵ�����  
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
		AfxMessageBox("���ļ�ʧ�ܡ�");
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
				//��ȡABCD
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
