#include "llm_model.h"
#include "llama.h"
#include <iostream>
#include "common.h"

InferenceModel::InferenceModel (const std::string& path, float p, float temp) {

    llama_context_params ctxParams = llama_context_default_params();
    initSampler(p, temp);
    auto success = loadModel(path);
    if (!success) return;
        
    initModel(ctxParams);

    // Get token IDs for stop sequences once at startup
    token_eos = llama_token_eos(m_vocab);                
    token_eot = llama_token_eot(m_vocab);
}

void InferenceModel::initSampler(float p, float temp) {

    llama_sampler_chain_params samplerParams = llama_sampler_chain_default_params();
    samplerParams.no_perf = true;
    m_sampler = llama_sampler_chain_init(samplerParams);
    llama_sampler_chain_add(m_sampler, llama_sampler_init_temp(temp));
    llama_sampler_chain_add(m_sampler, llama_sampler_init_min_p(p, 1));
    llama_sampler_chain_add(m_sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));
    

}

bool InferenceModel::loadModel(const std::string& path) {

    llama_model_params modelParams = llama_model_default_params();
    modelParams.n_gpu_layers = 40;
    
    m_model = llama_load_model_from_file(path.c_str(), modelParams);
    if (!m_model) {
        std::cerr << "Failed to load model at: " << path << "\n";
        return false;
    }
}

void InferenceModel::initModel(llama_context_params& ctxParams) {

    ctxParams.n_ctx = 0;
    ctxParams.n_threads = 3;
    m_ctx = llama_new_context_with_model(m_model, ctxParams);
    m_vocab = llama_model_get_vocab(m_model);
    m_formattedMessages = std::vector<char>(llama_n_ctx(m_ctx));
    m_messages.clear();
}

void InferenceModel::addMessage(const std::string& prompt, const char* role) {

    m_messages.push_back(llama_chat_message(role, strdup(prompt.c_str())));
}

void InferenceModel::startInference(const std::string& query) {
    
    addMessage(query, "user");

    int32_t newLen = llama_chat_apply_template(nullptr, m_messages.data(), m_messages.size(), true, m_formattedMessages.data(), m_formattedMessages.size());
    if (newLen < 0) 
    {
        std::cout << "Failed to apply template." << std::endl;
        return;
    }
    else if (newLen > (int32_t)m_formattedMessages.size()) {
        m_formattedMessages.resize(newLen);
        newLen = llama_chat_apply_template(nullptr, m_messages.data(), m_messages.size(), true, m_formattedMessages.data(), m_formattedMessages.size());
    }

    std::string prompt(m_formattedMessages.begin() + m_prevLen, m_formattedMessages.begin() + newLen);
    m_tokens = common_tokenize(m_ctx, prompt, true, true);
    m_batch = llama_batch_get_one(m_tokens.data(), (int)m_tokens.size());

}

std::string InferenceModel::getResponse() {

    int32_t contextSize = llama_n_ctx(m_ctx);
    int32_t usedCtx = llama_kv_self_used_cells(m_ctx);
    if (usedCtx + m_batch.n_tokens > contextSize) {

        std::cout << "Context size exceeded." << std::endl;
        exit(0);
    }

    if (llama_decode(m_ctx, m_batch) < 0) throw "Error decoding";

    int32_t idx = m_batch.n_tokens - 1;
    m_currentToken = llama_sampler_sample(m_sampler, m_ctx, idx);
    if (m_currentToken == token_eos || m_currentToken == token_eot || llama_token_is_eog(m_vocab, m_currentToken))
    {
        addMessage(strdup(m_response.data()), "assistant");
        m_response.clear();
        return "EOG";
    }

    std::string piece = common_token_to_piece(m_ctx, m_currentToken, true);

    m_response += piece;
    m_batch.token = &m_currentToken;
    m_batch.n_tokens = 1;

    return piece;
}

void InferenceModel::stopInference() {

    m_prevLen = llama_chat_apply_template(nullptr, m_messages.data(), m_messages.size(), false, nullptr, 0);
    if (m_prevLen < 0) std::cout << "stopInference() failed." << std::endl;
}

InferenceModel::~InferenceModel() {

    std::cout << "Deleting inference model" << std::endl;
    for (auto& message : m_messages) {
        delete message.content;
    }
  
    llama_kv_self_clear(m_ctx);
    llama_sampler_free(m_sampler);
    llama_free(m_ctx);
    llama_model_free(m_model);
}


