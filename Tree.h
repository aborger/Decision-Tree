#pragma once
#include "Attribute.h"
class Tree
{
private:
	struct Node
	{
		bool isLeaf = false;
		string ans; //ans is only defined if it is leaf


		Attribute predictor;
		List<string> connector;
		List<Node*> children;
	};
	Node* root;
	int parity;
public:
	Tree(List<List<string>> dataSet, List<Attribute> attributeList);

	void populate(Node *thisNode,List<List<string>> &dataSet, List<Attribute> attributeList);

	// returns index for attribute in attribute list
	int highestGain(List<List<string>> dataSet, List<Attribute> attributeList);
	float calculateInfo(List<List<string>> dataSet, List<Attribute> attributeList, int attNum);
	float entropy(int nTrue, int nTotal);

	void continuousTest(List<List<string>> dataSet, List<Attribute> &attributeList);

	void outputTree();
	void output(Node *thisNode, int indent);

	~Tree();
};

