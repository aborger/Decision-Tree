#pragma once
#include <string>
#include "List.h"
class Attribute
{
private:
	string Name;
	List<string> labelList;

	bool isContinuous = false;
	string splitPoint;
public:
	Attribute();

	// Getters
	string getName();
	List<string> getLabelList();
	bool getContinuous();
	string getSplit();
	// Setters
	void setName(string newName);
	void addLabel(string newLabel);
	void setContinuous();
	void setSplit(string split);

	~Attribute();
};

