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
		m_dialogNodes.push_back(std::make_shared<DialogNode>());
	}

	for (int i = 0; i < len; i++)
	{
		populateTree(data[i], m_dialogNodes[i]);
	}
}

DialogTree::DialogTree(nlohmann::json tree)
{
	auto len = tree.size();
	for (int i = 0; i < len; i++)
	{
		m_dialogNodes.push_back(std::make_shared<DialogNode>());
	}

	for (int i = 0; i < len; i++)
	{
		populateTree(tree[i], m_dialogNodes[i]);
	}
}

void DialogTree::populateTree(nlohmann::json& node, std::shared_ptr<DialogNode> dialogNode)
{
	dialogNode->setDialog(node["dialogText"]);
	for (auto& option : node["dialogOptions"])
	{
		std::shared_ptr<DialogNode> nextNode = nullptr;
		if (option["nextNode"] != nullptr)
		{
			nextNode = m_dialogNodes[option["nextNode"]];
		}
		dialogNode->dialogOptions.push_back(std::make_shared<DialogOption>(option["optionText"], nextNode));
	}

	
}

void DialogTree::performDialog()
{
	if (!m_dialogNodes.empty())
	{
		for (auto& node : m_dialogNodes)
		{
			std::cout << node->dialogText << std::endl;
			for (auto& option : node->dialogOptions)
			{
				std::cout << option->optionText << std::endl;
			}
		}
	}
}

