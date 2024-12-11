#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <mutex>
#include <thread>
#include <algorithm>
//
#include <ctime>
//
#include "gtest/gtest.h"
//
std::mutex mutex;
////TEST#1
TEST(sample_test_case, sample_test
) {
    EXPECT_EQ(1, 1);
}
//
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
//            if (dict.contains("max_responses")) {
//                return dict["max_responses"];
//            }
            if (dict.contains("config") && dict["config"].contains("max_responses")) {
                return dict["config"]["max_responses"];
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

    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

//
class InvertedIndex {
public:
    InvertedIndex() = default;
    //
    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        std::vector<std::thread> invertedIndexThread;
        for (size_t i = 0; i < input_docs.size(); ++i) {
            invertedIndexThread.emplace_back(
                    [this, doc = input_docs[i], doc_id = i]() {
                        std::stringstream streamDocs;
                        streamDocs << doc;
                        std::string tmp;
                        while (streamDocs >> tmp) {
                            mutex.lock();
                            freq_dictionary[tmp];
                            bool match = false;
                            for (auto &i: freq_dictionary[tmp]) {
                                if (i.doc_id == doc_id) {
                                    i.count++;
                                    match = true;
                                    break;
                                }
                            }
                            if (!match) {
//                                freq_dictionary[tmp];
                                freq_dictionary[tmp].push_back({doc_id, 1});
                            }
                            mutex.unlock();
                        }
                    });
        }
        for (auto &thread: invertedIndexThread) {
            thread.join();
        }
    }
    std::vector<Entry> GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};
////TEST#2
using namespace std;

void TestInvertedIndexFunctionality(
        const vector<string> &docs,
        const vector<string> &requests,
        const std::vector<vector<Entry>> &expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (auto &request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    {
        for (auto &i: result) {
            std::cout << "{\n";
            for (auto &j: i) {
                std::cout << "{ " << j.doc_id << ", " << j.count << " }";
            }
            std::cout << "\n}";
        }
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic
) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            },
            {
                    {0, 1}, {1, 3}
            }};
    TestInvertedIndexFunctionality(docs, requests, expected
    );
}

TEST(TestCaseInvertedIndex, TestBasic2
) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};////слова не совподают в тексте: "cappuccino"
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            },
            {
                    {0, 2}, {1, 2}, {2, 5}//// неправильно!!! на самом деле {0,3}
            },
            {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord
) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
            },
            {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected
    );
}

//
struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex &idx) : _index(idx) {};

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input) {
        std::vector<std::vector<RelativeIndex>> answers;
        for (auto &request: queries_input) {
            std::stringstream streamRequest;
            streamRequest << request;
            std::string wordRequest;
            std::map<size_t, float> answerMap;
            while (streamRequest >> wordRequest) {
                std::vector<Entry> sss = _index.GetWordCount(wordRequest);
                for (auto &i: sss) {
                    answerMap[i.doc_id];
                    answerMap[i.doc_id] += i.count;
                }
            }
            auto itMaxValue = std::max_element(
                    answerMap.begin(),
                    answerMap.end(),
                    [](const auto &a, const auto &b) {
                        return a.second < b.second;
                    }
            )->second;
            std::vector<RelativeIndex> answer;

            for (std::map<size_t, float>::iterator it = answerMap.begin(); it != answerMap.end(); ++it) {
                RelativeIndex tmp = {it->first, it->second /= itMaxValue};
                answer.emplace_back(tmp);
            }
            std::stable_sort(answer.begin(), answer.end(),
                             [](RelativeIndex a, RelativeIndex b) {
                                 return a.rank > b.rank;
                             });
            int responsesLimit = std::make_unique<ConverterJSON>()->GetResponsesLimit();
            if (answer.size() > responsesLimit) {
                answer.erase(answer.begin() + responsesLimit, answer.end());
            }
            answers.emplace_back(answer);
        }
////        }
        return answers;
    }


private:
    InvertedIndex _index;
};
////TEST
TEST(TestCaseSearchServer, TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> request = {"milk water", "sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway", "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}