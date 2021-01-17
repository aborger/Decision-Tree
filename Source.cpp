#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include "List.h"
#include "Attribute.h"
#include "Tree.h"

using namespace std;

// function prototypes
void readFile(List<Attribute> &attributeList, List<List<string>> &dataSet, string fileName);
void outputData(List<Attribute> attributeList, List<List<string>> dataSet);
void waitForEnterKey();

int main(int argc, char* argv[]) {
	//string fileName;
	//cout << "FileName: ";
	//cin >> fileName;
	string fileName = argv[1];
	List<Attribute> attributeList;
	List<List<string>> dataSet;



	readFile(attributeList, dataSet, fileName);
	
	//outputData(attributeList, dataSet);



	Tree decisionTree(dataSet, attributeList);

	decisionTree.outputTree();
	
	cout << "Press Enter to exit";
	waitForEnterKey();
}

void readFile(List<Attribute> &attributeList, List<List<string>> &dataSet, string fileName) {
	ifstream input;

	input.open(fileName);
	// input validation
	while (!input) {
		cout << "File Not Found  ";
		cout << "\n";
		cout << "Input File Name: ";
		cin >> fileName;
		input.open(fileName);
	}
	int numAttributes = 0;
	int attributeNum = 0;
	string inString;
	char inChar; // char array so the string can be appended

	getline(input, inString);
	numAttributes = stoi(inString);
	
	// --------------- Read into attributeList ------------------
	for (int att = 0; att < numAttributes + 1; att++) {
		getline(input, inString, ' ');
		Attribute *newAttribute = new Attribute;
		newAttribute->setName(inString);
		inString = "";
		while (input.get(inChar)) {
			if (inChar == '\n') {
				if (inString != " " && inString != "") {
					newAttribute->addLabel(inString);
				}
				inString = "";
				break;
			}
			else if (inChar == ' ') {
				if (inString != " " && inString != "") {
					newAttribute->addLabel(inString);
					inString = "";
				}
			}
			else {
				inString.push_back(inChar);
			}
		}
		attributeList.newNodeTail(*newAttribute);
	}


	// --------------- Read into dataSet ---------------------------
	List<string> *row = new List<string>;
	while (input.get(inChar)) {
		if (inChar == '\n') {
			if (inString != " " && inString != "") {
				row->newNodeTail(inString);
			}
				inString = "";
				dataSet.newNodeTail(*row);
				row = new List<string>;
		}
		else if (inChar == ' ') {
			if (inString != " " && inString != "") {
				row->newNodeTail(inString);
				inString = "";
			}
		}
		else {
			inString.push_back(inChar);
		}
	}


}

// used for debugging
void outputData(List<Attribute> attributeList, List<List<string>> dataSet) {
	// output attributeList
	cout << "======================= Attributes ===============================\n";
	for (int att = 0; att < attributeList.getListSize(); att++) {
		cout << "Attribute: " << attributeList.getAt(att).getName() << endl;
		for (int label = 0; label < attributeList.getAt(att).getLabelList().getListSize(); label++) {
			cout << "    Label: " << attributeList.getAt(att).getLabelList().getAt(label) << endl;
		}
	}

	// output dataSet
	cout << "====================== Data Set ===================================\n";
	for (int row = 0; row < dataSet.getListSize(); row++) {
		for (int col = 0; col < dataSet.getAt(row).getListSize(); col++) {
			cout << dataSet.getAt(row).getAt(col) << ",";
		}
		cout << endl;
	}
	cout << "========================================================================\n";
}

void waitForEnterKey() {
	cin.ignore();
	char getchar[1];
	cin.getline(getchar, 1);
	return;
}
