#include "Tree.h"

bool debug = false;

Tree::Tree(List<List<string>> dataSet, List<Attribute> attributeList)
{
	continuousTest(dataSet, attributeList);
	Node *newNode = new Node;
	root = newNode;
	populate(root, dataSet, attributeList);
}

void Tree::populate(Node *thisNode, List<List<string>> &dataSet, List<Attribute> attributeList) {
	int attributeNum = highestGain(dataSet, attributeList);

	if (attributeNum == -1) {
		// this is a leaf node
		thisNode->isLeaf = true;
		if (debug)
			cout << "This attribute is a leaf" << endl;
		for (int a = 0; a < attributeList.getTail().getLabelList().getListSize(); a++) {
			// test for what the answer is, all rows have same answer so it doesn't matter which row, but the last one is used
			if (dataSet.getTail().getTail() == attributeList.getTail().getLabelList().getAt(a)) {
				thisNode->ans = dataSet.getTail().getTail();
			}
		}
	}
	else {
		if (debug) {
			cout << "________________________________________________________________________" << endl; // debug
			cout << "Picked Attribute: " << attributeList.getAt(attributeNum).getName() << endl; // debug
		}
		thisNode->predictor = attributeList.getAt(attributeNum);

		// create child node for each label of this attribute
		// ----------------------------------------------------------------------------------------------------
		for (int label = 0; label < attributeList.getAt(attributeNum).getLabelList().getListSize(); label++) {
			// check to make sure this label is in the current dataSet
			/*bool onDataSet = false;
			for (int row = 0; row < dataSet.getListSize(); row++) {
				if (attributeList.getAt(attributeNum).getLabelList().getAt(label) == dataSet.getAt(row).getAt(attributeNum))
					onDataSet = true;
			}*/

			//if (onDataSet) {
				thisNode->connector.newNodeTail(attributeList.getAt(attributeNum).getLabelList().getAt(label));
				Node *newNode = new Node;

				// create new dataSet and attributeList without this attribute
				List<Attribute> newAttributeList;
				for (int att = 0; att < attributeList.getListSize(); att++) {
					newAttributeList.newNodeTail(attributeList.getAt(att));
				}

				newAttributeList.deleteAt(attributeNum);

				List<List<string>> newDataSet;



				for (int row = 0; row < dataSet.getListSize(); row++) {
					if (attributeList.getAt(attributeNum).getContinuous()) {
						// continuous so do for less than or equal and then again for greater than
						if (label == 0) {
							if (dataSet.getAt(row).getAt(attributeNum) <= attributeList.getAt(attributeNum).getSplit()) {
								// row corresponds to this label, add row to newDataSet, but without this attribute
								List<string> temp;
								for (int col = 0; col < dataSet.getAt(row).getListSize(); col++) {
									temp.newNodeTail(dataSet.getAt(row).getAt(col));
								}
								temp.deleteAt(attributeNum);
								newDataSet.newNodeTail(temp);
							}
						}
						else {
							// only thing that changes is greater than instead of less than or equal
							if (dataSet.getAt(row).getAt(attributeNum) > attributeList.getAt(attributeNum).getSplit()) {
								// row corresponds to this label, add row to newDataSet, but without this attribute
								List<string> temp;
								for (int col = 0; col < dataSet.getAt(row).getListSize(); col++) {
									temp.newNodeTail(dataSet.getAt(row).getAt(col));
								}
								temp.deleteAt(attributeNum);
								newDataSet.newNodeTail(temp);
							}
						}
					}
					else if (dataSet.getAt(row).getAt(attributeNum) == attributeList.getAt(attributeNum).getLabelList().getAt(label)) {
						// row corresponds to this label, add row to newDataSet, but without this attribute
						List<string> temp;
						for (int col = 0; col < dataSet.getAt(row).getListSize(); col++) {
							temp.newNodeTail(dataSet.getAt(row).getAt(col));
						}
						temp.deleteAt(attributeNum);
						newDataSet.newNodeTail(temp);
					}
				}
				// ----------------------------------------------------------------------------------------------------


				// ----------------- Debugging -----------------------------------
				if (debug)  {
					cout << "New data set for label: " << attributeList.getAt(attributeNum).getLabelList().getAt(label) << endl;
					for (int row = 0; row < newDataSet.getListSize(); row++) {
						for (int col = 0; col < newDataSet.getAt(row).getListSize(); col++) {
							cout << " " << newDataSet.getAt(row).getAt(col);
						}
						cout << endl;

					}
				}
				// ----------------------------------------------------------


				populate(newNode, newDataSet, newAttributeList);

				thisNode->children.newNodeTail(newNode);
			//}
		}
	}
}

int Tree::highestGain(List<List<string>> dataSet, List<Attribute> attributeList) {
	if (debug)
		cout << "======================= Gain ===================================\n" << endl; //debug
	float infoD;
	List<float> info; // corresponds to each attribute on attributeList
	// ---------------- Calculate Info(D) -------------------------
	int numTotal = dataSet.getListSize();
	// get num of tuples corresponding to first class of ans
	int numTrue = 0;
	for (int a = 0; a < numTotal; a++) {
		if (dataSet.getAt(a).getTail() == attributeList.getTail().getLabelList().getAt(0)) {
			numTrue++;
		}
	}

	infoD = entropy(numTrue, numTotal);
	if (debug)
		cout << "infoD: " << infoD << endl;
	// ---------- Calculate info for each Attribute -----------------
	// calculate for each attribute, subtracted by 1 so ans is not included
	for (int att = 0; att < attributeList.getListSize() - 1; att++) {
		if (debug)
			cout << "Attribute: " << attributeList.getAt(att).getName() << endl;
		info.newNodeTail(infoD - calculateInfo(dataSet, attributeList, att));
	}

	// debug
	if (debug) {
		for (int a = 0; a < attributeList.getListSize() - 1; a++) {
			cout << "Attribute: " << attributeList.getAt(a).getName() << " gain: " << info.getAt(a) << endl;
		}
	}

	// -------------------- if all attributes have gain of 0 its leaf return -1 -------------------
	int perfectGainCnt = 0;
	for (int a = 0; a < info.getListSize(); a++) {
		if (info.getAt(a) == 0)
			perfectGainCnt++;
	}
	if (perfectGainCnt == info.getListSize())
		return -1;
	
	// --------------------------- check for duplicate information gain -----------------------------------------
	// find median
	int median = info.getListSize() / 2;
	// test if any values are equal to any other value
	for (int a = 0; a < median; a++) {
		for (int b = median; b < info.getListSize(); b++) {
			if (info.getAt(a) == info.getAt(b)) {
				parity++;
			}
		}
	}
	


	// --------------------- Find which attribute has the highest gain ----------------------------
	int highAttribute = 0;
	float highGain = -1;
	for (int att = 0; att < attributeList.getListSize() - 1; att++) {
		if (info.getAt(att) > highGain) {
			highAttribute = att;
			highGain = info.getAt(att);
		}
	}
	return highAttribute;

}

float Tree::calculateInfo(List<List<string>> dataSet, List<Attribute> attributeList, int attNum) {
	float info = 0;
	int numTuples = 0; // get num tuples with attClass label
	int numTrue = 0; // find how many of those tuples give ans yes (or whichever ans label is in first column)
	
	if (attributeList.getAt(attNum).getContinuous()) {
		// attribute is continuous, find info with lesser than and greater than
		// lesser than first
		for (int dataRow = 0; dataRow < dataSet.getListSize(); dataRow++) {
			// test if tuple is in range
			if (dataSet.getAt(dataRow).getAt(attNum) <= attributeList.getAt(attNum).getSplit()) {
				numTuples++;
				// test if true
				if (dataSet.getAt(dataRow).getTail() == attributeList.getTail().getLabelList().getAt(0)) {
					numTrue++;
				}
			}
		}
		info = ((float)numTuples / (float)dataSet.getListSize())*entropy(numTrue, numTuples);

		numTuples = 0;
		numTrue = 0;

		// now greater than
		for (int dataRow = 0; dataRow < dataSet.getListSize(); dataRow++) {
			// test if tuple is in range
			if (dataSet.getAt(dataRow).getAt(attNum) > attributeList.getAt(attNum).getSplit()) {
				numTuples++;
				if (dataSet.getAt(dataRow).getTail() == attributeList.getTail().getLabelList().getAt(0)) {
					numTrue++;
				}
			}
		}
		info += ((float)numTuples / (float)dataSet.getListSize())*entropy(numTrue, numTuples);
	}
	else {
		
		for (int attClass = 0; attClass < attributeList.getAt(attNum).getLabelList().getListSize(); attClass++) {
			if (debug)
				cout << "Label: " << attributeList.getAt(attNum).getLabelList().getAt(attClass) << endl;
			numTuples = 0;
			numTrue = 0;


			for (int dataRow = 0; dataRow < dataSet.getListSize(); dataRow++) {
				// test if tuple has this attribute's class
				if (dataSet.getAt(dataRow).getAt(attNum) == attributeList.getAt(attNum).getLabelList().getAt(attClass)) {
					numTuples++;
					// find if that tuple gave ans true
					if (dataSet.getAt(dataRow).getTail() == attributeList.getTail().getLabelList().getAt(0)) {
						numTrue++;
					}
				}
			}
			info = info + ((float)numTuples / (float)dataSet.getListSize())*entropy(numTrue, numTuples);
			if (debug)
				cout << "   info: " << info << endl;
		}
	}
	return info;
}
float Tree::entropy(int nTrue, int nTotal) {
	if (nTrue == 0) {
		return 0;
	}
	float numTrue = (float)nTrue;
	float numTotal = (float)nTotal;
	float numFalse = numTotal - numTrue;
	float mainGain = -(numTrue / numTotal)*(log2(numTrue / numTotal));
	if (nTrue != nTotal) {
		mainGain -= (numFalse / numTotal)*log2(numFalse / numTotal);
	}
	return mainGain;
}

void Tree::continuousTest(List<List<string>> dataSet, List<Attribute> &attributeList) {
	for (int att = 0; att < attributeList.getListSize(); att++) {
		if (attributeList.getAt(att).getLabelList().getListSize() == 1 && attributeList.getAt(att).getLabelList().getTail() == "continuous") {
			// set continuous
			Attribute tempAtt = attributeList.getAt(att);
			tempAtt.setContinuous();
			attributeList.setAt(att, tempAtt);
			// take values for this attribute in data set and sort in ascending order
			List<float> attributeData;
			for (int row = 0; row < dataSet.getListSize(); row++) {
				attributeData.newNodeTail(stof(dataSet.getAt(row).getAt(att)));
			}
			attributeData.sort();

			// get split points
			List<string> splitPoints;

			for (int v = 0; v < attributeData.getListSize() - 1; v++) {
				splitPoints.newNodeTail(to_string((attributeData.getAt(v) + attributeData.getAt(v + 1)) / 2));
			}

			// find info for each split point
			List<float> splitInfo;
			for (int point = 0; point < splitPoints.getListSize(); point++) {
				tempAtt = attributeList.getAt(att);
				tempAtt.setSplit(splitPoints.getAt(point));
				attributeList.setAt(att, tempAtt);

				splitInfo.newNodeTail(calculateInfo(dataSet, attributeList, att));
				if (debug)
					cout << "Point: " << splitPoints.getAt(point) << " Info: " << splitInfo.getTail() << endl; // debug
			}

			// find which split point has the minimum info
			int point = -1;
			float minInfo = 2;
			for (int p = 0; p < splitInfo.getListSize(); p++) {
				if (splitInfo.getAt(p) < minInfo) {
					point = p;
					minInfo = splitInfo.getAt(p);
				}
			}

			// make that the split point
			tempAtt = attributeList.getAt(att);
			tempAtt.setSplit(splitPoints.getAt(point));
			attributeList.setAt(att, tempAtt);


		}
	}
}




void Tree::outputTree() {
	cout << "========================= Output ============================\n";
	for (int p = 0; p < parity; p++) {
		cout << "** duplicate information gain **" << endl;
	}
	output(root, 0);
}

void Tree::output(Node *thisNode, int indent) {
	if (thisNode->isLeaf) {
		// output
		for (int i = 0; i < indent; i++) {
			cout << "\t";
		}
		cout << thisNode->ans << endl;
	}
	else {
		// output all children
		for (int a = 0; a < thisNode->children.getListSize(); a++) {
			// indent
			for (int i = 0; i < indent; i++) {
				cout << "\t";
			}
			if (thisNode->predictor.getContinuous())
				cout << thisNode->predictor.getName() << thisNode->connector.getAt(a) << thisNode->predictor.getSplit() << ":" << endl;
			else
				cout << thisNode->predictor.getName() << "=" << thisNode->connector.getAt(a) << ":" << endl;
			output(thisNode->children.getAt(a), indent + 1);
		}
	}
}

Tree::~Tree()
{
}
