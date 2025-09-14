#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <ctime>
#include <format>
#include <map>
#include <iostream>
#include <fstream>
#include <ostream>



class Memento {

public:
	virtual ~Memento() {};
	virtual const std::string getName() const = 0;
	virtual const std::string getDate() const = 0;
	virtual const nlohmann::json getState() const = 0;
};


class ConcreteMemento : public Memento{
public:
	ConcreteMemento(nlohmann::json state) : mState(state){
	
		auto time = std::time(0);
		mDate = std::ctime(&time);
		std::cout << "Enter name of save file: " << std::endl;
		std::getline(std::cin, mFilename);
	}
	const std::string getName() const override { return std::format("{}, {}", mFilename, mDate); }
	const std::string getDate() const override { return mDate; }
	const nlohmann::json getState() const override { return mState; }

private:
	std::string mFilename;
	std::string mDate;
	nlohmann::json mState;
};

class SaveComponent {
public:
	void load(const Memento& saveState) { mState = saveState.getState(); }
	std::shared_ptr<Memento> save() { return std::make_shared<ConcreteMemento>(mState); }
	void setState(nlohmann::json state) { mState = state; };
private:
	nlohmann::json mState;
};


class SaveManager {
public:
	SaveManager(std::shared_ptr<SaveComponent> saveComponent) : mSaveComponent(saveComponent){}
	void save();
	void restore(std::string saveDate);
	void showHistory();

private:
	std::shared_ptr<SaveComponent> mSaveComponent;
	std::map<std::string, std::shared_ptr<Memento>> mMementos;
};