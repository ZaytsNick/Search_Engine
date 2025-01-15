#include "ConverterJSON.h"
#include "InvertedIndex.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        nlohmann::json dict;
        configFile >> dict;
        std::vector<std::string> tmp(0);
        if (dict.contains("files") && dict["files"].is_array()) {
            for (const auto &file: dict["files"]) {
                std::ifstream fileToSearch(file);
                if (fileToSearch.is_open()) {
                    std::ostringstream fileContents;
                    fileContents << fileToSearch.rdbuf();
                    tmp.push_back(fileContents.str());
                }
            }
        }
        return tmp;
    }
    throw std::runtime_error("Unable to open config.json");
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        nlohmann::json dict;
        configFile >> dict;
        if (dict.contains("config") && dict["config"].contains("max_responses")) {
            return dict["config"]["max_responses"];
        }
    }
    throw std::runtime_error("Unable to open config.json");
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream requestsFile("requests.json");
    if (requestsFile.is_open()) {
        nlohmann::json dict;
        requestsFile >> dict;
        std::vector<std::string> tmp;
        if (dict.contains("requests") && dict["requests"].is_array()) {
            for (const auto &file: dict["requests"]) {
                tmp.push_back(file.get<std::string>());
            }
        }
        return tmp;
    }
    throw std::runtime_error("Unable to open config file.");
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ofstream answerFile("answer.json");
    nlohmann::json dict;
    for (int i = 0; i < answers.size(); i++) {
        nlohmann::json request;
        std::ostringstream tmp;
        tmp << "request" << std::setw(3) << std::setfill('0') << i + 1;
        std::string requestId = tmp.str();
        if (!answers.empty()) {
            nlohmann::json ansRENAME;
            for (int j = 0; j < answers[i].size(); j++) {
                ansRENAME += {{"docid", answers[i][j].first},
                              {"rank",  answers[i][j].second}};
            }
            request[requestId] = {{"result",    true},
                                  {"relevance", ansRENAME}};
        } else {
            request[requestId] = {"result", false};
        }
        dict["answers"] += request;
    }
    answerFile << dict;
    answerFile.close();
}