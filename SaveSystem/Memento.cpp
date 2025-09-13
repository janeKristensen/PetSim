#include "include/Memento.h"

void SaveManager::save() { 
	
	auto save = mSaveComponent->save();  
	mMementos.insert({ save->getDate(), std::move(save) }); 
}

void SaveManager::restore(std::string saveDate) { 

	auto memento = mMementos.find(saveDate)->second;
	mSaveComponent->load(*memento); 
}

void SaveManager::showHistory() {

	for (auto const& [key, value] : mMementos) {

		std::cout << "Saved file: " << value.get()->getName() << std::endl;
	}
	
}