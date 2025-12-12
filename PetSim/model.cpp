#include "Model.h"
#include <iostream>
#include <functional>

Model::Model(const std::string& path, float p, float temp) 
{
    mModel = std::make_shared<InferenceModel>(path, p, temp);
}

void Model::inputLoop() 
{
    while (true) 
    {
        std::cout << "Enter query: " << std::endl;
        std::string query;
        std::getline(std::cin, query);
        if (query == "exit") break;

        mModel->startInference(query);
        std::string response;
        while ((response = mModel->getResponse()) != "EOG") 
        {
            std::cout << response << std::endl;
            std::fflush(stdout);
        }

        std::cout << '\n';
        mModel->stopInference();
    }
}

void Model::userQuery(const std::string& query) 
{

    std::lock_guard<std::mutex> guard(mModelMutex);
    mModel->startInference(query);
    std::string response;
    std::string line;
    std::string output;
    while ((response = mModel->getResponse()) != "EOG") 
    {

        if (line.size() + response.size() > 70) 
        {

            output += (line + '\n');
            line = response;
        }
        else 
        {
            line += response;
        }
    }

    output += line;

    while (!mStringBuffer.empty()) {};

    mStringBuffer = output;
    std::cout << mStringBuffer << std::endl;

    mModel->stopInference();
}

void Model::addSystemPrompt(const std::string& prompt) 
{
    mModel->addMessage(prompt, "system");
}

std::string Model::getModelStringBuffer() 
{ 
    std::string str = mStringBuffer; 
    mStringBuffer.clear(); 
    return str; 
}

void Model::clearModelStringBuffer() 
{
    mStringBuffer.clear();
}