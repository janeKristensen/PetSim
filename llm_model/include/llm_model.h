#pragma once
#include "llama.h"
#include <string>
#include <vector>
#include <memory>


class InferenceModel {

public:
	InferenceModel() {}
	InferenceModel(const std::string& path, float p, float temp);
	~InferenceModel();

	void addMessage(const std::string& prompt, const char* role);
	void startInference(const std::string& query);
	void stopInference();
	std::string getResponse();

private:
	bool loadModel(const std::string& path);
	void initSampler(float p, float temp);
	void initModel(llama_context_params& ctxParams);

	llama_model* m_model;
	llama_context* m_ctx;
	llama_sampler* m_sampler;
	const llama_vocab* m_vocab;
	llama_batch m_batch;
	llama_token m_currentToken;

	std::vector<llama_chat_message> m_messages;
	std::vector<char> m_formattedMessages;
	std::vector<llama_token> m_tokens;
	int32_t m_prevLen = 0;
	std::string m_response = "";

	llama_token token_eos;                
	llama_token token_eot;

};

