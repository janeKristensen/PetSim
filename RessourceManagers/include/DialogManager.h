#pragma once
#include "DialogSystem.h"


enum class DialogName {
	ROBOT_FIRST_DIALOG,
	ROBOT_SECOND_DIALOG
};

class Entity;

class DialogManager
{
public:
	
	void loadDialog(std::string filename);
	std::shared_ptr<DialogTree> getDialog(std::shared_ptr<Entity> entity) { return mDialogs.at(entity); }
	void attachDialog(std::shared_ptr<Entity> entity, DialogName dialog);
	void removeDialog(std::shared_ptr<Entity> entity);
	void setDialog(std::shared_ptr<Entity> entity, std::shared_ptr<DialogOption> option) { mDialogs.at(entity)->setNode(option->nextNode); }
	std::vector<std::shared_ptr<DialogOption>> performDialog(std::shared_ptr<Entity> entity);

private:
	
	std::unordered_map<DialogName, std::shared_ptr<DialogTree>> mDialogTrees;
	std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<DialogTree>> mDialogs;
};
