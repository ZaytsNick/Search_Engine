#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <mutex>
#include <thread>
//
#include <ctime>
//
#include "gtest/gtest.h"

//
std::mutex mutex;
//
TEST(sample_test_case, sample_test) {
    EXPECT_EQ(1, 0);
}
//

//std::vector<std::vector<std::pair<int, float>>> setAnswer( )
//{
//    std::vector<std::vector<std::pair<int, float>>> answers;
//    for (int i = 0; i < 5; i++) {
//        std::vector<std::pair<int, float>> tmp;
//        for (int j = 0; j < 5; j++) {
//            if (i != 2) { tmp.emplace_back(j, 0.21); }//std::pair<int, float>(j, 0.21)); }
//        }
//        answers.push_back(tmp);
//    }
//    return answers;
//}
class ConverterJSON {
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        std::ifstream configFile("config.json");
        if (configFile.is_open()) {
            nlohmann::json dict;
            configFile >> dict;
            std::vector<std::string> tmp;
            if (dict.contains("files") && dict["files"].is_array()) {
                for (const auto &file: dict["files"]) {
                    tmp.push_back(file.get<std::string>());
                }
            }
            return tmp;
        }
        throw std::runtime_error("Unable to open config file.");
    }

    int GetResponsesLimit() {
        std::ifstream configFile("config.json");
        if (configFile.is_open()) {
            nlohmann::json dict;
            configFile >> dict;
            if (dict.contains("max_responses")) {
                return dict["max_responses"];
            }
        }
        throw std::runtime_error("Unable to open config file.");
    }

    std::vector<std::string> GetRequests() {
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

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        std::ofstream answerFile("answer.json");
        nlohmann::json dict;
        for (int i = 0; i < 5; i++) {
            nlohmann::json request;
            std::ostringstream tmp;
            tmp << "request" << std::setw(3) << std::setfill('0') << i + 1;
            std::string requestId = tmp.str();
            if (!answers.empty() /*answers[i].size() != 0*/) {
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

};

//
struct Entry {
    size_t doc_id, count;

    // Данный оператор необходим для проведения тестовых сценариев
    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    //
    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        ConverterJSON updateBase;
        input_docs = updateBase.GetTextDocuments();

        std::vector<std::thread> invertedIndexThread;
        for (size_t i = 0; i < docs.size(); ++i) {
            invertedIndexThread.emplace_back(
                    [this, doc = docs[i], doc_id = i]() {//!!!invertedIndexThread.push_back(std::thread{
                        std::stringstream strStreamDocs;
                        strStreamDocs << doc;
                        std::string tmp;
                        while (strStreamDocs >> tmp) {
                            mutex.lock();
                            GetWordCount(tmp);
                            if (freq_dictionary.find(tmp) != freq_dictionary.end()) {
                                freq_dictionary[tmp];
                                for (int i = 0; i <= freq_dictionary[tmp].size(); ++i) {
                                    if (i == freq_dictionary[tmp].size()) {
                                        freq_dictionary[tmp].push_back({doc_id, 1});
                                    } else if (freq_dictionary[tmp][i].doc_id == doc_id) {
                                        freq_dictionary[tmp][i].count++;
                                        break;
                                    }
                                }
//                                 std::cout << "Слово \"" << tmp << "\" найдено в словаре!"<< std::endl;
                            } else {
                                freq_dictionary[tmp];
                                freq_dictionary[tmp].push_back({doc_id, 1});
//                                std::cout << "Слово \"" << tmp << "\" отсутствует в словаре." << std::endl;
                            }
                            mutex.unlock();
                        }
                    });
        }
        for (auto &thread: invertedIndexThread) {
            thread.join();
        }
    }

    /**
    * Метод определяет количество вхождений слова word в загруженной базе
            документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string &word) {
//        std::vector<Entry>  =freq_dictionary[word]
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};


int main() {
//    std::vector<std::string> docs = {
//            "london is the capital of great britain",
//            "paris is the capital of france",
//            "berlin is the capital of germany",
//            "rome is the capital of italy",
//            "madrid is the capital of spain",
//    };




//    std::vector<std::vector<std::pair<int, float>>> answers= setAnswer();
//    putAnswers(answers);
//    int aaa = GetResponsesLimit();
//    std::cout<<aaa<<std::endl;
//    std::vector<std::string>qqq= GetTextDocuments();
//    std::vector<std::string>sss= GetRequests();
//    for(auto& a:sss)
//    {
//        std::cout<<a<<std::endl;
//    }

    return 0;
}
