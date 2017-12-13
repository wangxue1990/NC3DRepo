// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VENT_SIMULATE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VENT_SIMULATE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef VENT_SIMULATE_EXPORTS
#define VENT_SIMULATE_API __declspec(dllexport)
#else
#define VENT_SIMULATE_API //__declspec(dllimport)
#endif
#include <vector>
using namespace std;

namespace vent{
	typedef signed int int32;
#define Compare_Error  (0.02)		//�������ȶ����

	typedef struct _StGeoVecter3D{
		double x;
		double y;
		double z;

		_StGeoVecter3D(double xx = 0.0f, double yy = 0.0f, double zz = 0.0f) : x(xx), y(yy), z(zz)
		{}

		bool operator == (const _StGeoVecter3D & lp)
		{
			if (abs(x - lp.x) > Compare_Error || abs(y - lp.y) > Compare_Error || abs(z - lp.z) > Compare_Error)
				return false;
			return true;
		}
		bool operator != (const _StGeoVecter3D & lp)
		{
			if (abs(x - lp.x) > Compare_Error || abs(y - lp.y) > Compare_Error || abs(z - lp.z) > Compare_Error)
				return true;
			return false;
		}

		double Dist(_StGeoVecter3D & v){
			return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
		}

		_StGeoVecter3D operator + (const _StGeoVecter3D & lp){
			return _StGeoVecter3D(x + lp.x, y + lp.y, z + lp.z);
		}

		_StGeoVecter3D operator - (const _StGeoVecter3D & lp){
			return _StGeoVecter3D(x - lp.x, y - lp.y, z - lp.z);
		}
	}GeoVector3D;
	/*��������*/
	enum EErrorType
	{
		EROADWAYERROR = 0,
		ENETERROR = 1,
		EINPUTPARAERROR = 2,
		EROADWAYWARING = 3,
		EOTHERERROR = 4,
		EOTHERWARING = 5,
	};
	/*�����ͨ���������ķ�֧����*/
	enum ELaneType
	{
		ELOSEFLOW = -2,//©���֧
		EFIXFLOW = -1,//�̶�������֧
		EAUTOFLOW = 0,//��Ȼ�ַ��֧
		EFANFLOW = 1,//�����֧
		EVIRTUALBRANCH = 2,//�����֧��������֧
		EFIXPRESURE = 3//�̶�ѹ����֧
	};
	/*������Ϣ*/
	struct ErrorInfo
	{
		int32 ID;
		EErrorType Etype;
		int32 explain;
		ErrorInfo() :ID(0), Etype(EROADWAYERROR), explain(0){}
	};
	/*���λ��*/
	enum EPosition
	{
		EINTERAL = 0,
		ETOGROUND = 1,
		EEOTHER = 2
	};
	/*����ģ������*/
	enum ESimType
	{
		ENATURESIM = 0,
		EFIXSIM = 1,
		EFIXSIM3D = 2,
		EREVERSESIM = 3
	};
	/*����������ݸ�ʽ*/
	struct Lane {
		int32 ID;
		int32 begin_node_ID;
		int32 end_node_ID;
		ELaneType type;
		double Q;
		double Qm;
		double R;
		double rR;
		double rho;
		double H;
		double length;
		EPosition pos;
		Lane() :ID(0), begin_node_ID(0), end_node_ID(0), type(EAUTOFLOW), Q(0.0), Qm(0.0), R(0.01),
			rR(0.01), rho(1.2), H(0.0), length(100.0), pos(EINTERAL)
		{}
	};
	/*�������*/
	struct Fan
	{
		int32 ID;
		int32 laneID;
		int32 fanstatus;
		double inverfact;
		double cur_a;
		double cur_b;
		double cur_c;
		double cur_d;
		double cur_e;
		double cur_f;
		double maxQ;
		double minQ;
		double maxP;
		double minP;
		double workQ;
		double workP;
		Fan():ID(0),laneID(0),fanstatus(1),inverfact(0.7),cur_a(0.0),
		cur_b(0.0),cur_c(0.0),cur_d(0.0),cur_e(0.0),cur_f(0.0),maxQ(0.0),
		maxP(0.0),minQ(0.0),minP(0.0),workP(0.0),workQ(0.0)
		{}
	};
	
	/*����ڵ����ݸ�ʽ*/
	struct GVertex
	{
		int32 ID;
		GeoVector3D coor;
		GVertex():ID(0),coor(0.0,0.0,0.0){
		}
	};
	
	/*ģ�����*/
	struct SimParas
	{
		double m_werr;
		double Presidual;
		double m_xsj;
		int32 numMaxIterate;
		int32 referenceNode;
		bool  m_Nvpn;
		bool iscalnodepre;
		int32 m_Iatm;
		double m_Patm;
		double m_Tr;
		double m_DFai;
		bool autoiniQ;
		SimParas():m_werr(0.01),Presidual(0.01),m_xsj(1.15),numMaxIterate(400),referenceNode(0),iscalnodepre(false),
		m_Nvpn(false),m_Patm(101325.0),m_Tr(21.0),m_DFai(80),autoiniQ(true)
		{
		}
	};

	struct Node;
	/*�ڲ���֧*/
	struct Branch
	{
		Lane* p_lane;
		Node* p_begin_node;
		Node* p_end_node;
		Fan* p_fan;
		int32 istree;
		double nature_pressure;
		double dh;
		double QR;
		Branch():p_lane(nullptr),p_begin_node(nullptr),p_end_node(nullptr),p_fan(nullptr),istree(0),nature_pressure(0.0),
		dh(0.0),QR(0.0){};
	};
	class VENT_SIMULATE_API VentNet;
	/*�ڲ��ڵ�*/
	struct Node
	{
		GVertex *node;
		Branch* link_tree;
		bool b_choosed;
		bool b_del;
		VentNet*pventnet;
		vector<Branch*> p_in_branch;
		vector<Branch*> p_out_branch;
		Node():node(nullptr),link_tree(nullptr),b_choosed(false),b_del(false),
		pventnet(nullptr)
		{
		}
	};
	class VENT_SIMULATE_API VentNet
	{
	public:
		VentNet();
		~VentNet();
		/*��ȡ��֧����*/
		size_t get_net_lanes_num();
		/*���+����*/
		bool check_net();
		/*������֧�ڵ�˫������*/
		void creat_topo();
		/*�����֧�߲�*/
		void calc_dh();
		
		/*�����������ͨ�ԣ�ͬʱ�ָ�����*/
		void calc_connect(vector<VentNet>&);
		/*��������֧*/
		void add_virtual_branch();
		/*�����С��*/
		bool tree();
		/*����·*/
		bool loop();
		/*��������ֵ*/
		void iniQ(bool b_autoini = true);
		/*���踳��ֵ*/
		void iniR(SimParas);
		/*�������㣺����Ȼ��ѹ*/
		bool incompressible_NVP_iterate_C(SimParas sim_para);
		/*�������㣬��������Ȼ��ѹ*/
		bool incompressible_iterate_C(SimParas sim_para);
		/*�����·��Ȼ��ѹ*/
		void calLoopNVP();
		/*�����֧����Ȼ��ѹ*/
		void calBranchNP();
		/*��ȡ����ĵ�ǰ����ѹ��*/
		double getworkP(Fan*fan);
		/*��ȡ����ĵ�ǰ����ѹ���ĵ���*/
		double getworkdP(Fan *mfan);

		/*��ʶ�����Ƿ���ͨ*/
		bool b_connect;
		vector<Branch*> v_p_branch_;//������net������branch
		vector<Node*> v_p_node_;//������net������node
		vector<Branch*> virtual_branches;//�����ڲ�����������֧
		vector<Node*> virtual_node2es;//�����ڴ����������ڵ�
		vector<vector<int32>> m_LoopDr;//��·�б�ʶ��֧����
		vector<vector<Branch*>> m_Loop;//��·����
		vector<double> m_LoopPresure;//��·��ѹ
	};
	
	/*�����û��Ľӿ���*/
	class VENT_SIMULATE_API VentSimulate
	{
	public:
		VentSimulate();
		~VentSimulate();
		//�������
		void set_lanes(Lane* plane, size_t num);
		//�ϲ��ӿ�
		//���ýڵ�
		void set_node(GVertex *pnode, size_t num);
		//���÷��
		void set_fan(Fan *pfan, size_t num);

		//���ý������
		void set_simparas(SimParas simpara);
		//ģ����㶯��
		bool simulate(ESimType);
		//��ȡ����������
		size_t get_vent_netnum();

		//��ȡָ��������ķ�֧��
		size_t get_vent_net_lane_num(size_t index);
		
		//��ȡָ��������ķ�֧
		void get_vent_net_lanes(size_t index, Lane**pplane, size_t num);
		
		//��ȡָ�����������С����ֵ����
		size_t get_vent_net_tree_lane_num();
		//��ȡָ�����������С��
		void get_vent_net_tree(size_t index, Lane**pplane, size_t num);
	private:
		/*������˵��
		0:
		1:û���������
		2:û�нڵ�����
		3:�����д��ڹ���������
		4:��֧��ʼ��ĩ�ڵ���ͬ
		5:�����֧û�ж�Ӧ���
		6:��֧����ֵС�ڵ���0,������Ϊ����ֵ
		7:�����ĳ�˲���������Ҳδ�������ͨ,�Ѻ���.
		8:�󾮲���ֻ��һ������.
		9:���Ӵ������������һ�˶�ͷ,�Ѻ�������.
		*/
		void add_error_info(int32 id, EErrorType type, int32 explainNum);

		//����������ݵĴ���
		bool check_input_data();
		//����ת��Ϊventnet���������ָ�
		bool creat_vent_net();

	private:
		vector<Lane*> v_lane_;//�������е�����lane
		vector<GVertex*> v_node_;//�������е�����node
		vector<Fan*> v_fan_;//�������е�����fan
		VentNet ventnet;//���������������ݵķ�֧�ͽڵ����ݣ��ڲ������磬��һ����ͨ
		vector<VentNet> v_vent_net_;//����ָ�õ���·����ͨͼ
		SimParas sim_para_;//�������
		vector<ErrorInfo> v_error_info_;//���ش�����Ϣ
	};

};
