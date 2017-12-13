#pragma once
#include "GraphStruct.h"

typedef signed int int32;
#define Compare_Error  (0.02)		//浮点数比对误差
//三维点
typedef struct _StGeoVecter3D
{
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
/*错误类型*/
enum EErrorType
{
	EROADWAYERROR = 0,
	ENETERROR = 1,
	EINPUTPARAERROR = 2,
	EROADWAYWARING = 3,
	EOTHERERROR = 4,
	EOTHERWARING = 5,
};
/*针对于通风网络解算的分支类型*/
enum ELaneType
{
	ELOSEFLOW = -2,//漏风分支
	EFIXFLOW = -1,//固定流量分支
	EAUTOFLOW = 0,//自然分风分支
	EFANFLOW = 1,//风机分支
	EVIRTUALBRANCH = 2,//虚拟分支、大气分支
	EFIXPRESURE = 3//固定压力分支
};
/*错误信息*/
struct ErrorInfo
{
	int32 ID;
	EErrorType Etype;
	int32 explain;
	ErrorInfo() :ID(0), Etype(EROADWAYERROR), explain(0){}
};
/*巷道位置*/
enum EPosition
{
	EINTERAL = 0,
	ETOGROUND = 1,
	EEOTHER = 2
};
/*网络模拟类型*/
enum ESimType
{
	ENATURESIM = 0,
	EFIXSIM = 1,
	EFIXSIM3D = 2,
	EREVERSESIM = 3
};

/*风机数据*/
struct VentFan
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
	VentFan():ID(0),laneID(0),fanstatus(1),inverfact(0.7),cur_a(0.0),
		cur_b(0.0),cur_c(0.0),cur_d(0.0),cur_e(0.0),cur_f(0.0),maxQ(0.0),
		maxP(0.0),minQ(0.0),minP(0.0),workP(0.0),workQ(0.0)
	{}
};

/*输入节点数据格式*/
struct VentNode
{
	int32 ID;
	GeoVector3D coor;
	bool b_choosed;
	bool b_del;
	VentNode():ID(0),coor(0.0,0.0,0.0){
	}
};

/*输入巷道数据格式*/
struct Branch 
{
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
	VentFan* p_fan;
	int32 istree;
	double nature_pressure;
	double dh;
	double QR;
	Branch() :ID(0), begin_node_ID(0), end_node_ID(0), type(EAUTOFLOW), Q(0.0), Qm(0.0), R(0.01),
		rR(0.01), rho(1.2), H(0.0), length(100.0), pos(EINTERAL)
	{}
};
/*模拟参数*/
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

class VentNet : public Graph
{
public:
	VentNet(void);
	~VentNet(void);
	bool AddNode(){	};
	bool DelNode(){};
	bool AddEdge(){	};
	bool DelEdge(){};
	/*获取分支数量*/
	size_t get_net_lanes_num();
	/*检查+纠错*/
	bool check_net();
	/*建立分支节点双向索引*/
	void creat_topo();
	/*计算分支高差*/
	void calc_dh();

	/*计算网络的连通性，同时分割网络*/
	void calc_connect(vector<VentNet>&);
	/*添加虚拟分支*/
	void add_virtual_branch();
	/*求解最小树*/
	bool tree();
	/*求解回路*/
	bool loop();
	/*风量赋初值*/
	void iniQ(bool b_autoini = true);
	/*风阻赋初值*/
	void iniR(SimParas);
	/*迭代计算：带自然风压*/
	bool incompressible_NVP_iterate_C(SimParas sim_para);
	/*迭代计算，不考虑自然风压*/
	bool incompressible_iterate_C(SimParas sim_para);
	/*计算回路自然风压*/
	void calLoopNVP();
	/*计算分支的自然风压*/
	void calBranchNP();
	/*获取风机的当前工作压力*/
	double getworkP(VentFan*fan);
	/*获取风机的当前工作压力的倒数*/
	double getworkdP(VentFan *mfan);
	void set_lanes(Branch* plane, size_t num);
	//合并接口
	//设置节点
	void set_node(VentNode *pnode, size_t num);
	//设置风机
	void set_fan(VentFan *pfan, size_t num);

	//设置解算参数
	void set_simparas(SimParas simpara);
	//模拟解算动作
	bool simulate(ESimType);
	//获取子网络数量
	size_t get_vent_netnum();

	//获取指定子网络的分支数
	size_t get_vent_net_lane_num(size_t index);

	//获取指定子网络的分支
	void get_vent_net_lanes(size_t index, Branch**pplane, size_t num);

	//获取指定子网络的最小树数值数量
	size_t get_vent_net_tree_lane_num();
	//获取指定子网络的最小树
	void get_vent_net_tree(size_t index, Branch**pplane, size_t num);
private:
	/*错误编号说明
	0:
	1:没有巷道数据
	2:没有节点数据
	3:网络中存在孤立点请检查
	4:分支的始、末节点相同
	5:风机分支没有对应风机
	6:分支风阻值小于等于0,已修正为绝对值
	7:巷道的某端不在网络中也未与大气连通,已忽略.
	8:矿井不能只有一个井口.
	9:连接大气的巷道至少一端独头,已忽略设置.
	*/
	void add_error_info(int32 id, EErrorType type, int32 explainNum);

	//检查输入数据的错误
	bool check_input_data();
	//数据转化为ventnet，完成网络分割
	bool creat_vent_net();

private:
	vector<Branch*> v_lane_;//保存所有的输入lane
	vector<VentNode*> v_node_;//保存所有的输入node
	vector<VentFan*> v_fan_;//保存所有的输入fan
	VentNet ventnet;//保存整个输入数据的分支和节点数据，内部大网络，不一定连通
	vector<VentNet> v_vent_net_;//保存分割好的网路，连通图
	SimParas sim_para_;//解算参数
	vector<ErrorInfo> v_error_info_;//返回错误信息
	/*标识网络是否连通*/
	bool b_connect;

	vector<Branch*> virtual_branches;//仅属于测网络的虚拟分支
	vector<Node*> virtual_node2es;//仅属于此网络的虚拟节点
	vector<vector<int32>> m_LoopDr;//回路中标识分支方向
	vector<vector<Branch*>> m_Loop;//回路集合
	vector<double> m_LoopPresure;//回路风压
};

