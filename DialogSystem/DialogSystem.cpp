#include "DialogSystem.h"


DialogTree::DialogTree(std::string filename)
{
	std::ifstream f(filename);
	if (!f.is_open()) {
		std::cout << "Failed to open file\n";
		return;
	}

	nlohmann::json data;

	try {
		data = nlohmann::json::parse(f);
	}
	catch (const nlohmann::json::parse_error& e) {
		std::cout << e.what() << '\n';
	};

	auto len = data.size();
	for (int i = 0; i < len; i++)
	{
		mDialogNodes.push_back(std::make_shared<DialogNode>());
	}

	for (int i = 0; i < len; i++)
	{
		populateTree(data[i], mDialogNodes[i]);
	}
}

DialogTree::DialogTree(nlohmann::json tree)
{
	auto len = tree.size();
	for (int i = 0; i < len; i++)
	{
		mDialogNodes.push_back(std::make_shared<DialogNode>());
	}

	for (int i = 0; i < len; i++)
	{
		populateTree(tree[i], mDialogNodes[i]);
	}

	mCurrentNode = mDialogNodes[0];
}

void DialogTree::populateTree(nlohmann::json& node, std::shared_ptr<DialogNode> dialogNode)
{
	dialogNode->setDialog(node["dialogText"]);
	for (auto& option : node["dialogOptions"])
	{
		std::shared_ptr<DialogNode> nextNode = nullptr;
		if (option["nextNode"] != nullptr)
		{
			nextNode = mDialogNodes[option["nextNode"]];
		}
		dialogNode->dialogOptions.push_back(std::make_shared<DialogOption>(option["optionText"], nextNode));
	}
}

void DialogTree::printTree()
{
	for (auto& node : mDialogNodes)
	{
		std::cout << "Dialog text: " << node->dialogText << std::endl;
		for (auto& option : node->dialogOptions)
		{
			std::cout << "Option: " << option->optionText << ", next option: " << option->nextNode << std::endl;
		}
		std::cout << "\n" << std::endl;
	}
}


