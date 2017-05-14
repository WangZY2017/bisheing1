
/**
	@RRT�ռ��µĹ��캯������(RRT�㷨�õ��Ĺ��ܺ���)
	@author zhao lu
	@version 20170415
*/

#include <path_planning/rrt.h>    //rrt.h��path_planning����
#include <math.h>
#include <cstddeef>
#include <iostream>

using namespace rrt;

/************************** Function Definitions *****************************/

/**
	@��ʼ��class�࣬ initializes source to 0,0��adds sorce to rrtTree
	@author zhao lu
	@version 20170415
*/
RRT::RRT()
{
	RRT::rrtNode newNode;
	newNode.posX = 0;
	newNode.posY = 0;
	newNode.parentID = 0;
	newNode.nodeID = 0;
	rrtTree.push_back(newNode);   //��β������һ���µĽڵ�
}

/**
	 @��ʼ����ʼ�ڵ������ ��adds sorce to rrtTree
	 @author zhao lu
	 @version 20170415
*/
RRT::RRT(double input_PosX, double input_PosY)
{
	RRT::rrtNode newNode;
	newNode.posX = input_PosX;
	newNode.posY = input_PosY;
	newNode.parentID = 0;
	newNode.nodeID = 0;
	rrtTree.push_back(newNode);
}

/**
	@���ص�ǰ��RRT��
	@author zhao lu
	@version 20170415
*/
vector<RRT::rrtNode> RRT::getTree()
{
	return rrtTree;
}

/**
	 @For setting the rrtTree to the inputTree
	 @param rrtTree
*/
void RRT::setTree(vector<RRT::rrtNode> input_rrtTree)
{
	rrtTree = input_rrtTree;
}

/**
	@to get the number of nodes in the rrt Tree
	@return tree size
*/
int RRT::getTreeSize()
{
	return rrtTree.size();
}

/**
	@adding a new node to the rrt Tree

*/
void RRT::addNewNode(RRT::rrtNode node)
{
	rrtTree.push_back(node);
}

/**
	@removing a node from the RRT Tree
	@return the removed tree
*/
RRT::rrtNode RRT::removeNode(int id)   //rrtNode�ǻ��࣬removeNode������
{
	RRT::rrtNode tempNode = rrtTree[id];
	rrtTree.erase(rrtTree.begin() + id );   //�����ڴ�
	return tempNode;
}

/**
	 getting a specific node
	 @param node id for the required node
	 @return node in the rrtNode structure
*/
RRT::rrtNode RRT::getNode(inr id)
{
	return rrtTree[id];
}

/**
	 ���Ѹ��������ڵ��еõ�����Ľڵ�
	 @param X position in X cordinate
	 @param Y position in Y cordinate
	 @return nodeID of the nearest Node
*/
int RRT::getNearesNodeID(double X, double Y)
{
	int i, returnID;
	double distance = 9999, tempDistance;  //WHY9999
	for (i = 0; i < this->getTreeSize(); i++)
	{
		tempDistance = getEuclideanDistanc((X, Y, getPosX(i), getPosY(i));  //�õ�ŷ����¾���
		if (tempDistance < distance)
		{
			distance = tempDistance;
			returnID = id;
		}
	}
	return returnID;
}

/**
	returns X coordinate(����) of the given node
*/
double RRT::getPosX(int nodeID)
{
	return rrtTree[nodeID].posX;
}

/**
	returns Y coordinate(����) of the given node
*/
double RRT::getPosY(int nodeID)
{
	return rrtTree[nodeID].posY;
}

/**
	 set X coordinate of the given node
*/
void RRT::setPosX(int nodeID, double input_PosX)
{
	rrtTree[nodeID].posX = input_PosX;
}

/**
  set Y coordinate of the given node
*/
void RRT::setPosY(int nodeID, double input_PosY)
{
	rrtTree[nodeID].posY = input_PosY;
}

/**
	 returns parentID of the given node
*/
RRT::rrtNode RRT::getParent(int id)
{
	return rrtTree[rrtTree[id].parentID];
}

/**
   set parentID of the given node
*/
void RRT::setParentID(int nodeID, int parentID)
{
	rrtTree[nodeID].parentID = parentID;
}

/**
   add a new childID to the children list of the given node
*/
void RRT::addChildID(int nodeID, int childID)
{
	rrtTree[nodeID].children.push_back(childID);
}

/**
  returns the children list of the given node
*/
vector<int> RRT::getChildren(int id)
{
	return rrtTree[id].children;
}

/**
  returns the children list of the given node
*/
vector<int> RRT::getChildren(int id)
{
	return rrtTree[id].children;
}

/**
   returns number of children of a given node
*/
int RRT::getChildrenSize(int nodeID)
{
	return rrtTree[nodeID].children.size();
}

/**
  ����euclidean�����������Ѿ��е�X,Y������
*/
double RRT::getEuclideanDistance(double sourceX, double sourceY, double destinationX, double destinationY)
{
	return sqrt(pow(destinationX - sourceX, 2) + pow(destinationY - sourceY, 2));
}

/**
	@����·�����õ���������·��
	@param endNodeID of the end node
	@���صõ�·����ÿ���ڵ��ID
*/
vector<int> RRT::getRootToEndPath(int endNodeID)
{
	vector<int> path;
	path.push_back(endNodeID);   //�����һ���ڵ����β��
	while (rrtTree[path.front()].nodeID != 0)  //����ʼ�ڵ��ID��Ϊ0����һֱ����
	{
		//��ָ��λ��beginλ�ò��룬front()����Ԫ��ֵ��begin()���ص�������ָ�룩��ָ��ָ���ֵ=front()����Ԫ��ֵ
		//һֱ����ǰ����룬ֱ��=0
		path.insert(path.begin(), rrtTree[path.front()].parentID);
	}
	return path;   //����һ����̬���飬��Ϊvector��һ�����ݽṹ���ࣩ
}
