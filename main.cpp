#include <iostream>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"


std::pair<int, float> convertToPair(const RelativeIndex &index) {
    return {static_cast<int>(index.doc_id), index.rank};
}

std::vector<std::vector<std::pair<int, float>>> convertAnswers(
        const std::vector<std::vector<RelativeIndex>> &originalAnswers) {
    std::vector<std::vector<std::pair<int, float>>> convertedAnswers;
    for (const auto &row: originalAnswers) {
        std::vector<std::pair<int, float>> convertedRow;
        for (const auto &item: row) {
            convertedRow.push_back(convertToPair(item));
        }
        convertedAnswers.push_back(convertedRow);
    }
    return convertedAnswers;
}

int main() {
    auto converter = std::make_unique<ConverterJSON>();
    auto invertedIndex = std::make_unique<InvertedIndex>();
    invertedIndex->UpdateDocumentBase(converter->GetTextDocuments());
    auto searchServer = std::make_unique<SearchServer>(*std::move(invertedIndex));
    std::vector<std::string> requests = converter->GetRequests();
    auto answers = searchServer->search(requests);
    converter->putAnswers(convertAnswers(answers));
    return 0;
}