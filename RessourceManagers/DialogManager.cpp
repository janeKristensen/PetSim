#include "DialogManager.h"




void DialogManager::loadDialog(std::string filename)
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

	for (auto& obj : data)
	{
		auto tree = std::make_shared<DialogTree>(obj["dialogTree"]);
		DialogName dialogName;
		try {
			dialogName = obj["dialogName"].get<DialogName>();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		};
		
		mDialogTrees.insert({ dialogName, tree});
	}
	
}

void DialogManager::attachDialog(std::shared_ptr<Entity> entity, DialogName dialog)
{
	auto it = mDialogTrees.find(dialog); 
	if (it != mDialogTrees.end())
	{
		auto dialogTree = it->second;
		if (!mDialogs.contains(entity))
		{
			mDialogs.insert({ entity, dialogTree });
		}
		else
		{
			mDialogs.at(entity) = dialogTree;
		}
	}
	
}

void DialogManager::removeDialog(std::shared_ptr<Entity> entity)
{
	if (!mDialogs.contains(entity))
	{
		mDialogs.at(entity) = nullptr;
	}
}

std::vector<std::shared_ptr<DialogOption>> DialogManager::performDialog(std::shared_ptr<Entity> entity)
{
	auto tree = mDialogs.at(entity);
	if (tree)
	{
		return tree->getNode()->dialogOptions;
	}
}