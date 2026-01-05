#include "Command.h"
#include "PetSim.h"

void SaveCommand::execute() const
{
	mGame->saveGame();
}