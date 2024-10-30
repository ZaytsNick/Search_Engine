#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>
//
#include "gtest/gtest.h"
TEST(sample_test_case, sample_test)
{
EXPECT_EQ(1, 1);
}
//

std::vector<std::vector<std::pair<int, float>>> setAnswer( )
{
    std::vector<std::vector<std::pair<int, float>>> answers;
    for (int i = 0; i < 5; i++) {
        std::vector<std::pair<int, float>> tmp;
        for (int j = 0; j < 5; j++) {
            if (i != 2) { tmp.push_back(std::pair<int, float>(j, 0.21)); }
        }
        answers.push_back(tmp);
    }
    return answers;
}

std::vector<std::string> GetTextDocuments()
{
    std::ifstream configFile("config.json");
    if(configFile.is_open()) {
        nlohmann::json dict;
        configFile>>dict;
        std::vector<std::string> tmp;
        if (dict.contains("files") && dict["files"].is_array()) {
            for (const auto& file : dict["files"]) {
                tmp.push_back(file.get<std::string>());
            }
        }
        return tmp;
    } else {
        throw std::runtime_error("Unable to open config file.");
    }
}

int GetResponsesLimit()
{
    std::ifstream configFile("config.json");
    if(configFile.is_open()) {
        nlohmann::json dict;
        configFile>>dict;
        if (dict.contains("max_responses"))
        {            return dict["max_responses"];}
    } else {
        throw std::runtime_error("Unable to open config file.");
    }
}
std::vector<std::string> GetRequests()
{
    std::ifstream requestsFile("requests.json");
    if(requestsFile.is_open()) {
        nlohmann::json dict;
        requestsFile>>dict;
        std::vector<std::string> tmp;
        if (dict.contains("requests") && dict["requests"].is_array()) {
            for (const auto& file : dict["requests"]) {
                tmp.push_back(file.get<std::string>());
            }
        }
        return tmp;
    } else {
        throw std::runtime_error("Unable to open config file.");
    }
}

void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    std::ofstream answerFile("answer.json");
    nlohmann::json dict;
    for (int i = 0; i < 5; i++) {
        nlohmann::json request;
        std::ostringstream tmp;
        tmp << "request" << std::setw(3) << std::setfill('0') << i + 1;
        std::string requestId = tmp.str();
        if (answers[i].size() != 0) {
            nlohmann::json ansRENAME;
            for (int j = 0; j < 5; j++) {
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

int main() {


//    std::vector<std::vector<std::pair<int, float>>> answers= setAnswer();
//    putAnswers(answers);
    std::vector<std::string>sss= GetRequests();
    for(auto& a:sss)
    {
        std::cout<<a<<std::endl;
    }

    return 0;
}
