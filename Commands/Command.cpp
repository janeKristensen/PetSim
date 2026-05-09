#include "Command.h"
#include "PetSim.h"
#include "LoadingScene.h"
#include "ShopScene.h"


void StartCommand::execute() const
{
	SceneManager::getInstance()->changeScene(std::make_shared<LoadingScene>(mScreenSize, mGame, mGame->getServices()));
}


void SaveCommand::execute() const
{
	mGame->saveGame();
}


void QuitCommand::execute() const
{
	mGame->quitGame();
}


void LoadCommand::execute() const
{
	mGame->loadGame(mFilename);
}


void ContinueCommand::execute() const
{
	SceneManager::getInstance()->removeScene();
}


void BuyCommand::execute() const
{
	// Add item to inventory
	auto tile = mScene->getSelectedTile();
	if (tile)
	{
		const auto& item_template = tile->getItem();
		auto& game_scene = mScene->getGameScene();
			auto item = game_scene.createItemFromType(item_template.getTypeId(), item_template.getTextureName(), item_template.getTextureRect(), item_template.getValue());
			game_scene.addItemToInventory(item, 1);
			game_scene.removeMoney(tile->getPrice());
	}
}

void DialogCommand::execute() const
{
	auto dialogManager = mGame->getServices().dialogManager;
	if (mOption->nextNode)
	{
		dialogManager->setDialog(mEntity, mOption);
		SceneManager::getInstance()->getScene()->setDialogOptions(dialogManager->performDialog(mEntity));
	}
	else
	{
		dialogManager->removeDialog(mEntity);
		auto scene = SceneManager::getInstance()->getScene();
		scene->setDialogOptions(nullptr);
		scene->deactivateDialog();
	}
}