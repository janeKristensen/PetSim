#include "include/Memento.h"

void SaveManager::save() { 
	
	auto save = mSaveComponent->save();  
	mMementos.insert({ save->getDate(), save }); 
	auto o = std::ofstream("pretty.json");
	o << save->getState().dump(4) << std::endl;
}

void SaveManager::restore(std::string saveDate) { 

	auto memento = mMementos.find(saveDate)->second;
	mSaveComponent->load(*memento); 
}

void SaveManager::showHistory() {

	for (auto const& [key, value] : mMementos) {

		std::cout << "Saved file: " << value.get()->getName() << std::endl;
		std::cout << "Save data: " << value.get()->getState() << std::endl;
	}
	
}