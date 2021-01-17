#include "Attribute.h"



Attribute::Attribute()
{
}


// Getters
string Attribute::getName() {
	return Name;
}
List<string> Attribute::getLabelList() {
	return labelList;
}
bool Attribute::getContinuous() {
	return isContinuous;
}
string Attribute::getSplit() {
	return splitPoint;
}
// Setters
void Attribute::setName(string newName) {
	Name = newName;
}
void Attribute::addLabel(string newLabel) {
	labelList.newNodeTail(newLabel);
}
void Attribute::setContinuous() {
	isContinuous = true;
	labelList.deleteTail();
	labelList.newNodeTail("<=");
	labelList.newNodeTail(">");
}
void Attribute::setSplit(string split) {
	splitPoint = split;
}

Attribute::~Attribute()
{
}
