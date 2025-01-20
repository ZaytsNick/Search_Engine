#include "InvertedIndex.h"

std::mutex mutex;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs.clear();
    freq_dictionary.clear();
    std::vector<std::thread> invertedIndexThread;
    for (size_t i = 0; i < input_docs.size(); ++i) {
        invertedIndexThread.emplace_back(
                [this, doc = input_docs[i], doc_id = i]() {
                    std::map<std::string, std::vector<Entry>> local_freq_dict;
                    std::stringstream streamDocs;
                    streamDocs << doc;
                    std::string tmp;
                    while (streamDocs >> tmp) {
                        bool match = false;
                        if (local_freq_dict.find(tmp) != local_freq_dict.end()) {
                            for (auto &entry: local_freq_dict[tmp]) {
                                if (entry.doc_id == doc_id) {
                                    entry.count++;
                                    match = true;
                                    break;
                                }
                            }
                        }
                        if (!match) {
                            local_freq_dict[tmp].push_back({doc_id, 1});
                        }
                    }
                    std::lock_guard<std::mutex> lock(mutex);
                    for (auto &[word, entries]: local_freq_dict) {
                        for (auto &entry: entries) {
                            bool found = false;
                            for (auto &global_entry: freq_dictionary[word]) {
                                if (global_entry.doc_id == entry.doc_id) {
                                    global_entry.count += entry.count;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                freq_dictionary[word].push_back(entry);
                            }
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