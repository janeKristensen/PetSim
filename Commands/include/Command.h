#pragma once
#include <memory>

class Game;

class Command{
public:
	virtual ~Command() {};
	virtual void execute() const = 0;
};


class SaveCommand : public Command{
public:
	SaveCommand(std::shared_ptr<Game> game) : mGame(game){}
	void execute() const override;
	
private:
	std::shared_ptr<Game> mGame;
};