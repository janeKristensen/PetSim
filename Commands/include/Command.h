#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include <string>

class Game;
class ShopScene;
class Scene;
class Entity;
class DialogOption;

class Command{
public:
	virtual ~Command() {};
	virtual void execute() const = 0;
};


class StartCommand : public Command {
public:
	StartCommand(std::shared_ptr<Game> game) : mGame(game) {}
	void execute() const override;
	void setScreenSize(sf::Vector2f size) { mScreenSize = size; }

private:
	std::shared_ptr<Game> mGame;
	sf::Vector2f mScreenSize;
};


class SaveCommand : public Command{
public:
	SaveCommand(std::shared_ptr<Game> game) : mGame(game){}
	void execute() const override;
	
private:
	std::shared_ptr<Game> mGame;
};


class QuitCommand : public Command {
public:
	QuitCommand(std::shared_ptr<Game> game) : mGame(game) {}
	void execute() const override;

private:
	std::shared_ptr<Game> mGame;
};


class LoadCommand : public Command {
public:
	LoadCommand(std::shared_ptr<Game> game) : mGame(game) {}
	void execute() const override;
	void setFilename(const std::string& filename) { mFilename = filename; }

private:
	std::shared_ptr<Game> mGame;
	std::string mFilename = "";
};


class ContinueCommand : public Command {
public:
	ContinueCommand(std::shared_ptr<Game> game) : mGame(game) {}
	void execute() const override;
	
private:
	std::shared_ptr<Game> mGame;
};


class BuyCommand : public Command {
public:
	BuyCommand(std::shared_ptr<ShopScene> scene) : mScene(scene) {}
	void execute() const override;

private:
	std::shared_ptr<ShopScene> mScene;
};

class DialogCommand : public Command{
public:
	DialogCommand(std::shared_ptr<Entity> entity, std::shared_ptr<Game> game, std::vector<std::shared_ptr<DialogOption>>* dialogOptions, size_t index)
		: mEntity(entity), mGame(game), mDialogOptions(dialogOptions), mIndex(index){}
	void execute() const override;
private:
	std::shared_ptr<Entity> mEntity;
	std::shared_ptr<Game> mGame;
	std::vector<std::shared_ptr<DialogOption>>* mDialogOptions;
	size_t mIndex;
};