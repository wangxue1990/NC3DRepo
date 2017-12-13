// vent_simulate.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "vent_simulate.h"
namespace vent
{
	VentSimulate::VentSimulate()
	{
	}

	VentSimulate::~VentSimulate()
	{
		for (size_t i = 0; i < ventnet.v_p_node_.size(); i++)
		{
			delete ventnet.v_p_node_[i];
		}
		for (size_t i = 0; i < ventnet.v_p_branch_.size(); i++)
		{
			delete ventnet.v_p_branch_[i];
		}
	}

	void VentSimulate::set_lanes(Lane* plane, size_t num)
	{
		for (size_t i = 0; i < num; i++)
		{
			v_lane_.push_back(plane + i);
		}
	}

	void VentSimulate::set_node(GVertex *pnode, size_t num)
	{
		for (size_t i = 0; i < num; i++)
		{
			v_node_.push_back(pnode + i);
		}
	}

	void VentSimulate::set_fan(Fan *pfan, size_t num)
	{
		for (size_t i = 0; i < num; i++)
		{
			v_fan_.push_back(pfan + i);
		}
	}

	void VentSimulate::set_simparas(SimParas simpara)
	{
		sim_para_ = simpara;
	}

	size_t VentSimulate::get_vent_netnum()
	{
		return v_vent_net_.size();
	}

	size_t VentSimulate::get_vent_net_lane_num(size_t index)
	{
		return v_vent_net_.at(index).get_net_lanes_num();
	}

	void VentSimulate::get_vent_net_lanes(size_t index, Lane**pplane, size_t num)
	{
		for (size_t i = 0; i < v_vent_net_.at(index).v_p_branch_.size(); i++)
		{
			*(pplane + i) = v_vent_net_[index].v_p_branch_[i]->p_lane;//for
		}
	}
	bool VentSimulate::simulate(ESimType simtype){
		switch (simtype)
		{
		case EREVERSESIM://反风模拟
			/*	if (!checkData()) return false;

			if (!transData()) return false;
			if (!iniSim()) return false;
			if (!minTree()) return false;
			if (!fundLoop()) return false;
			if (!iniQ(simPara.autoiniQ))return false;
			if (!reverseSimulate()) return false;
			if (!calculateH())return false;
			if (!result2RoadWay()) return false;*/
			break;
		case ENATURESIM://自然分风模拟
			//this->AddInfo(0, EOtherErr, L"解算类型未定义."); return false;
			break;
		case EFIXSIM://固定风量
			//if (!checkData()) return false;
			////if (!simplifyNet())return false;
			//if (!transData()) return false;
			//if (!iniSim()) return false;
			//if (!minTree()) return false;
			//if (!fundLoop()) return false;
			//if (!iniQ(simPara.autoiniQ))return false;
			//if (simPara.m_Nvpn){
			//	if (!calLoopNVP()) return false;
			//}
			//if (!incompressible_iterate_C()) return false;
			//if (!calculateH())return false;
			//if (!result2RoadWay()) return false;
			break;
		case EFIXSIM3D:
			if (!check_input_data()) return false;
			if (!creat_vent_net()) return false;
			if (sim_para_.m_Nvpn)
			{
				ventnet.calc_dh();
			}
			for (size_t i = 0; i < v_vent_net_.size(); i++)
			{
				if (!v_vent_net_[i].check_net())
				{
					continue;
				}
				v_vent_net_[i].add_virtual_branch();
				v_vent_net_[i].tree();
				v_vent_net_[i].loop();
				v_vent_net_[i].iniQ(sim_para_.autoiniQ);
				if (sim_para_.m_Nvpn)
				{
					v_vent_net_[i].incompressible_NVP_iterate_C(sim_para_);
				}
				else{
					v_vent_net_[i].incompressible_iterate_C(sim_para_);
				}
				
			}
			break;
		default:
			return false;
		};
		if (sim_para_.iscalnodepre)
		{
			//CalNodePre();
		}
		return true;

	}

	bool VentSimulate::check_input_data(){
		if (v_lane_.size() == 0)
		{
			add_error_info(0, EOTHERERROR, 1);
			return false;
		}
		if (v_node_.size() < 2)
		{
			add_error_info(0, EOTHERERROR, 2);
			return false;
		}

		bool res2 = false;
		for (size_t i = 0; i < v_lane_.size(); i++)
		{
			switch (v_lane_[i]->type)
			{
			case EVIRTUALBRANCH:
				v_lane_[i]->R = 0;
				break;
			case EAUTOFLOW:
				if (v_lane_[i]->R <= 0)
				{
					v_lane_[i]->R = -v_lane_[i]->R;
					add_error_info(v_lane_[i]->ID, EROADWAYWARING, 6);
				}
				break;
			case EFANFLOW:
				res2 = false;
				for (size_t j = 0; j < v_fan_.size(); j++)
				{
					if (v_fan_[j]->laneID == v_lane_[i]->ID)
					{
						res2 = true;
					}
				}
				if (!res2)
				{
					add_error_info(v_lane_[i]->ID, EROADWAYWARING, 5);
					v_lane_[i]->type = EAUTOFLOW;
				}
				if (v_lane_[i]->R <= 0)
				{
					v_lane_[i]->R = -v_lane_[i]->R;
					add_error_info(v_lane_[i]->ID, EROADWAYWARING, 6);
				}
				break;
			case EFIXFLOW:
				if (v_lane_[i]->R <= 0)
				{
					v_lane_[i]->R = -v_lane_[i]->R;
					add_error_info(v_lane_[i]->ID, EROADWAYWARING, 6);
				}
				break;
			case ELOSEFLOW:
				if (v_lane_[i]->R <= 0)
				{
					v_lane_[i]->R = -v_lane_[i]->R;
					add_error_info(v_lane_[i]->ID, EROADWAYWARING, 6);
				}
				break;
			default:
				break;
			}
		}

		return true;
	}

	bool VentSimulate::creat_vent_net()
	{
		for (size_t i = 0; i < v_node_.size(); i++)
		{
			Node *tempnode = new Node;
			tempnode->node = v_node_[i];
			tempnode->b_choosed = false;
			ventnet.v_p_node_.push_back(tempnode);
		}

		for (size_t i = 0; i < v_lane_.size(); i++)
		{
			Branch *tempbranch = new Branch;
			tempbranch->p_lane = v_lane_[i];
			if (v_lane_[i]->type == EFANFLOW){
				for (size_t j = 0; j < v_fan_.size(); j++)
				{
					if (v_fan_[j]->laneID == v_lane_[i]->ID)
					{
						if (v_fan_[j]->fanstatus == 0)
						{
							v_lane_[i]->type = EAUTOFLOW;
						}
						else
						{
							tempbranch->p_fan = v_fan_[j];
						}
					}
				}
			}
			ventnet.v_p_branch_.push_back(tempbranch);
		}

		ventnet.creat_topo();
		//分割网络
		ventnet.calc_connect(v_vent_net_);
		return true;
	}

	void VentSimulate::add_error_info(int32 id, EErrorType type, int32 explainNum)
	{
		ErrorInfo temperrorinfo;
		temperrorinfo.ID = id;
		temperrorinfo.Etype = type;
		temperrorinfo.explain = explainNum;
		v_error_info_.push_back(temperrorinfo);
	}

	VentNet::VentNet() :b_connect(false)
	{

	};
	VentNet::~VentNet()
	{
		//删除虚拟分支
		for (size_t i = 0; i < virtual_branches.size(); i++)
		{
			delete virtual_branches[i]->p_lane; virtual_branches[i]->p_lane = nullptr;
			delete virtual_branches[i]; virtual_branches[i] = nullptr;
		}
		for (size_t i = 0; i < virtual_node2es.size(); i++)
		{
			delete virtual_node2es[i]; virtual_node2es[i] = nullptr;
		}
	};

	size_t VentNet::get_net_lanes_num()
	{
		return v_p_branch_.size();
	};

	void VentNet::creat_topo()
	{
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			for (size_t j = 0; j < v_p_node_.size(); j++)
			{
				if (v_p_branch_[i]->p_lane->begin_node_ID == v_p_node_[j]->node->ID)
				{
					v_p_branch_[i]->p_begin_node = v_p_node_[j];
					v_p_node_[j]->p_out_branch.push_back(v_p_branch_[i]);
				}
				if (v_p_branch_[i]->p_lane->end_node_ID == v_p_node_[j]->node->ID)
				{
					v_p_branch_[i]->p_end_node = v_p_node_[j];
					v_p_node_[j]->p_in_branch.push_back(v_p_branch_[i]);
				}
			}
		}

	}
	/*after calc topo*/
	void VentNet::calc_dh()
	{
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			v_p_branch_[i]->dh = v_p_branch_[i]->p_begin_node->node->coor.z - v_p_branch_[i]->p_end_node->node->coor.z;
		}
	}
	/*计算网络连通性，图的遍历，分割网络*/
	void VentNet::calc_connect(vector<VentNet>&ventnets)
	{
		vector<Node*> nodefrom, nodefrom2;
		VentNet tempNet;
		tempNet.v_p_node_.push_back(v_p_node_[0]);

		nodefrom.push_back(v_p_node_[0]);
		v_p_node_[0]->b_choosed = true;

		while (true)
		{
			for (size_t i = 0; i < nodefrom.size(); i++)
			{
				for (size_t j = 0; j < nodefrom[i]->p_in_branch.size(); j++)
				{
					if (nodefrom[i]->p_in_branch[j]->istree == 1)
					{
						continue;
					}
					else
					{
						tempNet.v_p_branch_.push_back(nodefrom[i]->p_in_branch[j]);
						nodefrom[i]->p_in_branch[j]->istree = 1;
						if (nodefrom[i]->p_in_branch[j]->p_begin_node->b_choosed)
						{
							continue;
						}
						else
						{
							nodefrom[i]->p_in_branch[j]->p_begin_node->b_choosed = true;
							tempNet.v_p_node_.push_back(nodefrom[i]->p_in_branch[j]->p_begin_node);
							nodefrom2.push_back(nodefrom[i]->p_in_branch[j]->p_begin_node);
						}
					}
				}
				for (size_t j = 0; j < nodefrom[i]->p_out_branch.size(); j++)
				{
					if (nodefrom[i]->p_out_branch[j]->istree == 1)
					{
						continue;
					}
					else
					{
						tempNet.v_p_branch_.push_back(nodefrom[i]->p_out_branch[j]);
						nodefrom[i]->p_out_branch[j]->istree = 1;
						if (nodefrom[i]->p_out_branch[j]->p_end_node->b_choosed)
						{
							continue;
						}
						else
						{
							nodefrom[i]->p_out_branch[j]->p_end_node->b_choosed = true;
							tempNet.v_p_node_.push_back(nodefrom[i]->p_out_branch[j]->p_end_node);
							nodefrom2.push_back(nodefrom[i]->p_out_branch[j]->p_end_node);
						}
					}
				}
			}
			nodefrom.clear();
			if (nodefrom2.size() == 0)
			{
				//creat one net 
				if (tempNet.v_p_node_.size() > 1 && tempNet.v_p_branch_.size() > 1)
				{
					tempNet.b_connect = true;
					ventnets.push_back(tempNet);
					tempNet.v_p_node_.clear();
					tempNet.v_p_branch_.clear();
				}

				//check nodeset
				bool isok = true;
				for (auto&pnode : v_p_node_)
				{
					if (pnode->b_choosed == false)
					{
						isok = false;
						nodefrom2.push_back(pnode);
						pnode->b_choosed = true;
						break;
					}
				}
				if (isok)
				{
					break;
				}
			}
			for (size_t i = 0; i < nodefrom2.size(); i++)
			{
				nodefrom.push_back(nodefrom2[i]);
			}
			nodefrom2.clear();
		}
	}
	/*添加虚拟分支*/
	void VentNet::add_virtual_branch()
	{
		bool firstnode = true;
		Node* mm = nullptr;
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			if (v_p_branch_[i]->p_lane->pos == ETOGROUND)
			{
				if ((v_p_branch_[i]->p_end_node->p_in_branch.size() + v_p_branch_[i]->p_end_node->p_out_branch.size()) == 1)
				{
					if (firstnode)
					{
						firstnode = false;
						mm = v_p_branch_[i]->p_end_node;
					}
					else
					{
						Branch * tempbranch = new Branch;//delete ~VentNet()
						tempbranch->p_lane = new Lane;//delete ~VentNet()
						tempbranch->p_lane->Q = v_p_branch_[i]->p_lane->Q;
						tempbranch->p_lane->R = 0.0;
						tempbranch->p_lane->rho = 1.2;
						tempbranch->p_begin_node = v_p_branch_[i]->p_end_node;
						tempbranch->p_end_node = mm;
						tempbranch->p_lane->type = EVIRTUALBRANCH;
						tempbranch->dh = tempbranch->p_begin_node->node->coor.z - tempbranch->p_end_node->node->coor.z;
						v_p_branch_.push_back(tempbranch);
						virtual_branches.push_back(tempbranch);
						tempbranch->p_begin_node->p_out_branch.push_back(tempbranch);
						mm->p_in_branch.push_back(tempbranch);
					}
				}
				if ((v_p_branch_[i]->p_begin_node->p_in_branch.size() + v_p_branch_[i]->p_begin_node->p_out_branch.size()) == 1)
				{
					if (firstnode)
					{
						firstnode = false;
						mm = v_p_branch_[i]->p_begin_node;
					}
					else
					{
						Branch * tempbranch = new Branch;//delete ~VentNet()
						tempbranch->p_lane = new Lane;//delete ~VentNet()
						tempbranch->p_lane->Q = v_p_branch_[i]->p_lane->Q;
						tempbranch->p_lane->R = 0.0;
						tempbranch->p_lane->rho = 1.2;
						tempbranch->p_begin_node = mm;
						tempbranch->p_end_node = v_p_branch_[i]->p_begin_node;
						tempbranch->p_lane->type = EVIRTUALBRANCH;
						tempbranch->dh = tempbranch->p_begin_node->node->coor.z - tempbranch->p_end_node->node->coor.z;
						v_p_branch_.push_back(tempbranch);
						virtual_branches.push_back(tempbranch);
						v_p_branch_[i]->p_begin_node->p_in_branch.push_back(tempbranch);
						mm->p_out_branch.push_back(tempbranch);
					}
				}
			}
		}
	}
	/**/
	bool VentNet::tree()
	{
		size_t m_branchCount = v_p_branch_.size();
		int32 m_nodeCount = v_p_node_.size();
		for (size_t i = 0; i<m_branchCount; i++){
			v_p_branch_[i]->QR = abs(v_p_branch_[i]->p_lane->Q*v_p_branch_[i]->p_lane->R);
			if (v_p_branch_[i]->QR > 9999999){
				v_p_branch_[i]->QR = 9999990;
			}
			if (v_p_branch_[i]->p_lane->type < 0){
				v_p_branch_[i]->QR = 9999998;
			}
			v_p_branch_[i]->istree = 0;
		}

		for (size_t i = 0; i < m_nodeCount; i++)
		{
			v_p_node_[i]->b_choosed = false;
			v_p_node_[i]->b_del = false;
		}
		int32 chooseNum = 1;

		vector<Node*> choosed;
		choosed.reserve(m_nodeCount);
		//vector<int> chooseddel;
		//chooseddel.resize(m_nodeCount);

		choosed.push_back(v_p_node_[0]);
		v_p_node_[0]->b_choosed = true;

		while (chooseNum < m_nodeCount)
		{
			//找已选节点的所有关联分支,关联分支的另一端不在已选节点中.即边割.
			//将边割中QR最小的分支加入到树中
			//保证固定风量和风机分支不在树中
			Branch* treechoosed = nullptr;
			double minQR = 9999999;
			for (size_t i = 0; i < choosed.size(); i++)
			{
				//int ii = choosed[i];
				if (choosed[i]->b_del) continue;
				choosed[i]->b_del = true;

				for (size_t j = 0; j < choosed[i]->p_in_branch.size(); j++)
				{
					//int jj = vecNode[ii].inBranch[j];
					Branch *jj = choosed[i]->p_in_branch[j];
					if (jj->p_begin_node->b_choosed == false && jj->istree == 0)
					{
						choosed[i]->b_del = false;
					}
					else continue;

					if (jj->QR < minQR)
					{
						treechoosed = jj;
						minQR = jj->QR;
					}
				}
				for (size_t j = 0; j < choosed[i]->p_out_branch.size(); j++)
				{
					Branch* jj = choosed[i]->p_out_branch[j];
					if (jj->istree == 0 && jj->p_end_node->b_choosed == false)
					{
						choosed[i]->b_del = false;
					}
					else continue;
					if (jj->QR < minQR)
					{
						treechoosed = jj;
						minQR = jj->QR;
					}
				}
			}

			if (minQR != 9999999 && treechoosed != nullptr)
			{

				if (treechoosed->p_end_node->b_choosed == false)
				{
					chooseNum++;
					choosed.push_back(treechoosed->p_end_node);
					treechoosed->p_end_node->b_choosed = true;
					treechoosed->p_end_node->link_tree = treechoosed;

					if (treechoosed->p_lane->type < 0)
					{
						//AddInfo(vecBranch[treechoosed].ID, ERoadwayWarning, L"固定风量分支设置不合理,请检查.");
					}
					//m_tree2.push_back(treechoosed);
					treechoosed->istree = chooseNum - 1;
				}
				else if (treechoosed->p_begin_node->b_choosed == false)
				{
					chooseNum++;
					choosed.push_back(treechoosed->p_begin_node);
					treechoosed->p_begin_node->b_choosed = true;//=-vecNode[vecBranch[treechoosed].InputStartNode].jno;
					treechoosed->p_begin_node->link_tree = treechoosed;

					if (treechoosed->p_lane->type < 0)
					{
						//AddInfo(vecBranch[treechoosed].ID, ERoadwayWarning, L"固定风量分支设置不合理,请检查.");
					}
					treechoosed->istree = -(chooseNum - 1);
				}
				else{
					//AddInfo(0, EOtherErr, L"图不是连通的."); return false;
				}
			}
			else {
				//AddInfo(0, EOtherErr, L"图不是连通的."); return false;
			}
		}
		return true;
	}
	/**/
	bool VentNet::loop()
	{
		size_t m_branchCount = v_p_branch_.size();
		size_t m_nodeCount = v_p_node_.size();
		m_Loop.clear();
		m_LoopDr.clear();
		m_Loop.resize(m_branchCount - m_nodeCount + 1);
		m_LoopDr.resize(m_branchCount - m_nodeCount + 1);
		int loopnum = 0;
		Node* beginNode, *endNode;
		for (size_t i = 0; i < m_Loop.size(); i++)
		{
			m_Loop[i].clear();
			m_LoopDr[i].clear();
		}

		for (size_t i = 0; i < m_branchCount; i++)
		{
			if (v_p_branch_[i]->istree == 0)//求最小树过程标记的余树支, 正数代表顺长, 负数代表负增长, 绝对值越大, 越年轻
			{
				beginNode = v_p_branch_[i]->p_begin_node;//余树枝的开始节点
				endNode = v_p_branch_[i]->p_end_node;//结束节点
				Branch* looptree = v_p_branch_[i];
				//顺着两端在树枝中查找,按照树生长的方向
				loopnum += 1;
				bool isok = false;
				int32 treedirect = 1;
				do {
					isok = false;
					m_LoopDr[loopnum - 1].push_back(treedirect);//记录回路方向
					m_Loop[loopnum - 1].push_back(looptree);//分支加入回路,回路分支树数+1
					if (beginNode == endNode) continue;
					if (beginNode == v_p_node_[0])//参考点,没有linktree
					{
						if (endNode->link_tree->istree > 0)
						{
							looptree = endNode->link_tree; treedirect = -1;
							endNode = endNode->link_tree->p_begin_node;
						}
						else
						{
							looptree = endNode->link_tree; treedirect = 1;
							endNode = endNode->link_tree->p_end_node;
						}
						isok = true;
					}
					else if (endNode == v_p_node_[0])
					{
						if (beginNode->link_tree->istree > 0)
						{
							looptree = beginNode->link_tree; treedirect = 1;
							beginNode = beginNode->link_tree->p_begin_node;
						}
						else
						{
							looptree = beginNode->link_tree; treedirect = -1;
							beginNode = beginNode->link_tree->p_end_node;
						}
						isok = true;
					}
					else{
						if (abs(beginNode->link_tree->istree) > abs(endNode->link_tree->istree))
						{
							if (beginNode->link_tree->istree > 0)
							{
								looptree = beginNode->link_tree; treedirect = 1;
								beginNode = beginNode->link_tree->p_begin_node;
							}
							else
							{
								looptree = beginNode->link_tree; treedirect = -1;
								beginNode = beginNode->link_tree->p_end_node;
							}
							isok = true;
						}
						else if (abs(beginNode->link_tree->istree) < abs(endNode->link_tree->istree))
						{
							if (endNode->link_tree->istree>0)
							{
								looptree = endNode->link_tree; treedirect = -1;
								endNode = endNode->link_tree->p_begin_node;
							}
							else
							{
								looptree = endNode->link_tree; treedirect = 1;
								endNode = endNode->link_tree->p_end_node;
							}
							isok = true;
						}
					}
				} while (isok == true);
			}
		}
		return true;
	}
	/**/
	void VentNet::iniQ(bool b_autoiniQ)
	{
		for (size_t i = 0; i < m_Loop.size(); i++)
		{
			for (size_t j = 1; j < m_Loop[i].size(); j++)
			{
				m_Loop[i][j]->p_lane->Q = 0;
			}
		}
		//每个回路的第一个分支是余树枝m_Loop[i][0]
		for (size_t i = 0; i < m_Loop.size(); i++)
		{
			if (b_autoiniQ)
			{
				if (m_Loop[i][0]->p_lane->type != EFIXFLOW && m_Loop[i][0]->p_lane->type != ELOSEFLOW)
				{
					m_Loop[i][0]->p_lane->Q = 10.0;
				}
			}
			double Q1 = m_LoopDr[i][0] * m_Loop[i][0]->p_lane->Q;
			for (size_t j = 1; j < m_Loop[i].size(); j++)
			{
				m_Loop[i][j]->p_lane->Q += m_LoopDr[i][j] * Q1;
			}
		}
		return;
	}
	bool VentNet::incompressible_iterate_C(SimParas sim_para)
	{
		double dqmax, dpsum, rqsum, dq;
		int inum = 0;
		int inum2 = 0;
		while (inum < sim_para.numMaxIterate) {
			dqmax = 0;
			for (size_t i = 0; i < m_Loop.size(); i++)
			{
				dpsum = 0; rqsum = 0;
				if (m_Loop[i][0]->p_lane->type <= -1) continue;
				for (size_t j = 0; j < m_Loop[i].size(); j++)
				{
					//N = m_Loop[i][j];
					if (m_Loop[i][j]->p_lane->type == EFANFLOW){
						if (m_LoopDr[i][j] == 1){
							m_Loop[i][j]->p_fan->workQ = m_Loop[i][j]->p_lane->Q;
							dpsum = dpsum + m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) - getworkP(m_Loop[i][j]->p_fan);
							rqsum = rqsum + m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q) - getworkdP(m_Loop[i][j]->p_fan);
						}
						else{
							m_Loop[i][j]->p_fan->workQ = m_Loop[i][j]->p_lane->Q;
							dpsum = dpsum - m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) + getworkP(m_Loop[i][j]->p_fan);
							rqsum = rqsum + m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q) - getworkdP(m_Loop[i][j]->p_fan);
						}
					}
					else {
						if (m_LoopDr[i][j] == 1){
							dpsum += m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q);
							rqsum += m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q);
						}
						else{
							dpsum -= m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q);
							rqsum += m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q);
						}
					}
				}
				if (rqsum == 0){
					rqsum = 0.001;
					dq = (dpsum) / rqsum * 0.5;
				}
				else{
					dq = (dpsum) / rqsum * 0.5;
				}

				for (size_t j = 0; j < m_Loop[i].size(); j++)	{

					if (m_LoopDr[i][j] == 1)
					{
						m_Loop[i][j]->p_lane->Q -= dq;
					}
					else
					{
						m_Loop[i][j]->p_lane->Q += dq;
					}
				}
				if (dqmax < fabs(dq)) dqmax = fabs(dq);
			}
			if (dqmax < sim_para.m_werr)	return true;

			if (inum2 > 30)
			{
				inum2 = 0;
				tree();
				loop();
			}
			inum += 1; inum2 += 1;
		}
		//if (inum > sim_para.numMaxIterate)
		//	;//AddInfo(0, EOtherErr, L"迭代次数超过最大次数,尚未满足精度!");
		//
		return true;
	}
	/**/
	bool VentNet::incompressible_NVP_iterate_C(SimParas sim_para){
		calLoopNVP();
		double dqmax, dpsum, rqsum, dq;
		int inum = 0;
		int inum2 = 0;
		while (inum < sim_para.numMaxIterate) {
			dqmax = 0;
			for (size_t i = 0; i < m_Loop.size(); i++)
			{
				dpsum = 0; rqsum = 0;
				if (m_Loop[i][0]->p_lane->type <= -1) continue;
				for (size_t j = 0; j < m_Loop[i].size(); j++)
				{
					//N = m_Loop[i][j];
					if (m_Loop[i][j]->p_lane->type == 1){
						if (m_LoopDr[i][j] == 1){
							m_Loop[i][j]->p_fan->workQ = m_Loop[i][j]->p_lane->Q;
							dpsum = dpsum + m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) - getworkP(m_Loop[i][j]->p_fan) - m_Loop[i][j]->nature_pressure;
							rqsum = rqsum + m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q) - getworkdP(m_Loop[i][j]->p_fan);
						}
						else{
							m_Loop[i][j]->p_fan->workQ = m_Loop[i][j]->p_lane->Q;
							dpsum = dpsum - m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) + getworkP(m_Loop[i][j]->p_fan) + m_Loop[i][j]->nature_pressure;
							rqsum = rqsum + m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q) - getworkdP(m_Loop[i][j]->p_fan);
						}
					}
					else {
						if (m_LoopDr[i][j] == 1){
							dpsum += m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) - m_Loop[i][j]->nature_pressure;
							rqsum += m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q);
						}
						else{
							dpsum -= m_Loop[i][j]->p_lane->R * m_Loop[i][j]->p_lane->Q * fabs(m_Loop[i][j]->p_lane->Q) - m_Loop[i][j]->nature_pressure;
							rqsum += m_Loop[i][j]->p_lane->R * fabs(m_Loop[i][j]->p_lane->Q);
						}
					}
				}
				if (rqsum == 0){
					rqsum = 0.001;
					dq = (dpsum - m_LoopPresure[i]) / rqsum * 0.5;
				}
				else{
					dq = (dpsum - m_LoopPresure[i]) / rqsum * 0.5;
				}

				for (size_t j = 0; j < m_Loop[i].size(); j++)	{

					if (m_LoopDr[i][j] == 1)
					{
						m_Loop[i][j]->p_lane->Q -= dq;
					}
					else
					{
						m_Loop[i][j]->p_lane->Q += dq;
					}
				}
				if (dqmax < fabs(dq)) dqmax = fabs(dq);
			}
			if (dqmax < sim_para.m_werr)	return true;

			if (inum2 > 30)
			{
				inum2 = 0;
				calLoopNVP();
				tree();
				loop();
			}
			inum += 1; inum2 += 1;
		}
		//if (inum > sim_para.numMaxIterate)
		//	;//AddInfo(0, EOtherErr, L"迭代次数超过最大次数,尚未满足精度!");
		//
		return true;
	}

	/**/
	void VentNet::calLoopNVP()
	{
		m_LoopPresure.clear();
		m_LoopPresure.resize(v_p_branch_.size() - v_p_node_.size() + 1);
		for (size_t i = 0; i < m_Loop.size(); i++)
		{
			m_LoopPresure[i] = 0;
			for (size_t j = 0; j < m_Loop[i].size(); j++)
			{
				if (m_LoopDr[i][j] == 1)
				{
					m_LoopPresure[i] += m_Loop[i][j]->nature_pressure;
				}
				else
				{
					m_LoopPresure[i] -= m_Loop[i][j]->nature_pressure;
				}
			}
		}
		return;
	}

	/**/
	void VentNet::calBranchNP()
	{
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			v_p_branch_[i]->nature_pressure = v_p_branch_[i]->p_lane->rho * 9.8 * v_p_branch_[i]->dh;
		}
	}

	/**/
	double VentNet::getworkP(Fan *mfan){
		if (mfan->workQ >= mfan->maxQ)
		{
			return mfan->minP;
		}
		else if (mfan->workQ <= mfan->minQ)
		{

			return mfan->maxP;
		}
		mfan->workP = mfan->cur_a + (mfan->cur_b + (mfan->cur_c + (mfan->cur_d + (mfan->cur_e + mfan->cur_f * mfan->workQ) * mfan->workQ)*mfan->workQ)*mfan->workQ)*mfan->workQ;
		return mfan->workP;
	}

	double VentNet::getworkdP(Fan *mfan)
	{
		if (mfan->workQ >= mfan->maxQ)
		{
			return 0;
		}
		else if (mfan->workQ <= mfan->minQ)
		{
			return 0;
		}
		return mfan->cur_b + (2 * mfan->cur_c + (3 * mfan->cur_d + (4 * mfan->cur_e + 5 * mfan->cur_f * mfan->workQ) * mfan->workQ)*mfan->workQ)*mfan->workQ;
	}

	void VentNet::iniR(SimParas simPara)
	{
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			v_p_branch_[i]->p_lane->R = v_p_branch_[i]->p_lane->R*simPara.m_xsj;
		}
	}

	/*连通的子网络原则上不存在孤立点和孤立巷道不做检查*/
	bool VentNet::check_net()
	{
		if (!b_connect)
		{
			return false;
		}

		if (v_p_branch_.size() <= 1)
		{
			return false;
		}
		if (v_p_node_.size() <= 2)
		{
			return false;
		}

		int jingkou = 0;
		//设置为连通大气的分支必须有一端开口
		for (size_t i = 0; i < v_p_branch_.size(); i++)
		{
			if (v_p_branch_[i]->p_lane->pos == ETOGROUND)
			{
				if ((v_p_branch_[i]->p_begin_node->p_out_branch.size() + v_p_branch_[i]->p_begin_node->p_in_branch.size()) == 1 ||
					(v_p_branch_[i]->p_end_node->p_out_branch.size() + v_p_branch_[i]->p_end_node->p_in_branch.size()) == 1)
				{
				}
				else
				{
					v_p_branch_[i]->p_lane->pos = EINTERAL;
				}
			}
		}
		Branch *p_jkou = nullptr;
		//只有一条分支连接的节点，必须是连通大气或者other
		for (size_t i = 0; i < v_p_node_.size(); i++)
		{
			if ((v_p_node_[i]->p_in_branch.size() + v_p_node_[i]->p_out_branch.size()) == 1)
			{
				if (v_p_node_[i]->p_in_branch.size() == 1)
				{
					if (v_p_node_[i]->p_in_branch[0]->p_lane->pos == EINTERAL)
					{
						v_p_node_[i]->p_in_branch[0]->p_lane->pos = EEOTHER;
					}
					if (v_p_node_[i]->p_in_branch[0]->p_lane->pos == ETOGROUND)
					{
						jingkou += 1;
						p_jkou = v_p_node_[i]->p_in_branch[0];
					}
				}
				if (v_p_node_[i]->p_out_branch.size() == 1)
				{
					if (v_p_node_[i]->p_out_branch[0]->p_lane->pos == EINTERAL)
					{
						v_p_node_[i]->p_out_branch[0]->p_lane->pos = EEOTHER;
					}
					if (v_p_node_[i]->p_out_branch[0]->p_lane->pos == ETOGROUND)
					{
						jingkou += 1;
						p_jkou = v_p_node_[i]->p_out_branch[0];
					}
				}
			}
		}
		if (jingkou == 1 && p_jkou != nullptr)
		{
			p_jkou->p_lane->pos = EEOTHER;
		}
		return true;
	}

};


