#include "InvertedIndex.h"

std::mutex mutex;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    std::vector<std::thread> invertedIndexThread;
    for (size_t i = 0; i < input_docs.size(); ++i) {
        invertedIndexThread.emplace_back(
                [this, doc = input_docs[i], doc_id = i]() {
                    std::stringstream streamDocs;
                    streamDocs << doc;
                    std::string tmp;
                    while (streamDocs >> tmp) {
                        std::lock_guard<std::mutex> lock(mutex);
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
                            freq_dictionary[tmp].push_back({doc_id, 1});
                        }
                    }
                });
    }
    for (auto &thread: invertedIndexThread) {
        thread.join();
    }
}
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
        return freq_dictionary[word];
    }



