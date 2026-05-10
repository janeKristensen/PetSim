#pragma once
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"
#include <iostream>

class DialogNode;

// Example json format
// [
//	{
//		"dialogText": "Hello world",
//			"dialogOptions" : [
//		{
//			"optionText": "Go away",
//				"nextNode" : null
//		},
//		{
//			"optionText": "Hi",
//			"nextNode" : 1
//		}]
//	}
//]


class DialogOption
{
public:
	DialogOption(std::string text, std::shared_ptr<DialogNode> node) : optionText(text), nextNode(node) {}

	std::string optionText = "";
	std::shared_ptr<DialogNode> nextNode;
};

class DialogNode
{
public:
	DialogNode(){}
	void setDialog(std::string text) { dialogText = text; }
	std::string dialogText = "";
	std::vector<std::shared_ptr<DialogOption>> dialogOptions;

};


class DialogTree
{
public:
	DialogTree(std::string filename);
	DialogTree(nlohmann::json tree);
	void setNode(std::shared_ptr<DialogNode> node) { mCurrentNode = node; }
	void printTree();
	std::shared_ptr<DialogNode> getNode() { return mCurrentNode; }

private:
	void populateTree(nlohmann::json& node, std::shared_ptr<DialogNode> dialogNode);

	std::vector<std::shared_ptr<DialogNode>> mDialogNodes;
	std::shared_ptr<DialogNode> mCurrentNode;
};