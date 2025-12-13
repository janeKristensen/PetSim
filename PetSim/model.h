#pragma once
#include "llm_model.h"
#include <iostream>
#include <mutex>

class Model {

public:
	Model(const std::string& path, float p, float temp);
	~Model() { std::cout << "Deleting model" << std::endl; }
	Model(Model& other) {

		mModel = other.clone();
	}
	std::shared_ptr<InferenceModel> clone() const {

		return mModel;
	}
	void inputLoop();
	void userQuery(const std::string& query);
	void addSystemPrompt(const std::string& prompt);
	std::string getModelStringBuffer();
	void clearModelStringBuffer();

private:
	std::shared_ptr<InferenceModel> mModel = nullptr;
	std::string mStringBuffer = "";
	std::mutex mModelMutex;
};
