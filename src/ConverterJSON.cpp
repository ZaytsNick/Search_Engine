#include "ConverterJSON.h"
#include "InvertedIndex.h"

bool checkingTheForStartup() {
    std::ifstream configFile("config.json");
    std::ifstream requestsFile("requests.json");
    if (configFile.is_open()) {
        nlohmann::json dict;
        configFile >> dict;
        if (dict.contains("files") && dict.contains("config") && dict["config"].contains("name")
            && dict["config"].contains("version")
            && dict["config"].contains("max_responses")) {
            if (requestsFile.is_open()) {
                return true;
            }
        } else { throw std::runtime_error("Config file is missing required keys"); }
    } else { throw std::runtime_error("Unable to open config.json"); }
    return false;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    if (checkingTheForStartup()) {
        std::ifstream configFile("config.json");
        if (configFile.is_open()) {
            nlohmann::json dict;
            configFile >> dict;
            std::vector<std::string> tmp;
            tmp.reserve(dict["files"].size());
            if (dict.contains("files") && dict["files"].is_array()) {
                for (const auto &file: dict["files"]) {
                    std::ifstream fileToSearch(file);
                    if (fileToSearch.is_open()) {
                        tmp.emplace_back(
                                (std::istreambuf_iterator<char>(fileToSearch)),
                                std::istreambuf_iterator<char>());
                    }
                }
            }
            return tmp;
        }
        throw std::runtime_error("Unable to open config.json");
    }
}

int ConverterJSON::GetResponsesLimit() {
    if (checkingTheForStartup()) {
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
}

std::vector<std::string> ConverterJSON::GetRequests() {
    if (checkingTheForStartup()) {
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
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    if (checkingTheForStartup()) {
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
}