#include "SearchServer.h"
#include "ConverterJSON.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> answers;
    for (auto &request: queries_input) {
        std::stringstream streamRequest;
        streamRequest << request;
        std::string wordRequest;
        std::map<size_t, float> answerMap;
        while (streamRequest >> wordRequest) {
            std::vector<Entry> answerVector = _index.GetWordCount(wordRequest);
            for (auto &answerElement: answerVector) {
                answerMap[answerElement.doc_id];
                answerMap[answerElement.doc_id] += i.count;
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
    return answers;
}
