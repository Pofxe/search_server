#pragma once
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include "read_input_functions.h"
#include "string_processing.h"

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double ALLOWED_DISCREPANCY = 1e-6;

class SearchServer 
{
public:

    enum class DocumentStatus { ACTUAL = 1, DELETED = 2, ABSENT = 3 };

    struct Document
    {
        Document() = default;

        Document(int id_, double relevance_, DocumentStatus status_, int rating_) :
            id(id_), relevance(relevance_), status(status_), rating(rating_) {};

        int id = 0;
        double relevance = 0.0;
        DocumentStatus status = DocumentStatus::ACTUAL;
        int rating = 0;
    };

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words);

    explicit SearchServer(const string& stop_words_text);

    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings);

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const;

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const;

    vector<Document> FindTopDocuments(const string& raw_query) const;

    size_t GetDocumentCount() const;

    size_t GetDocumentId(int index) const;

    vector<string> MatchDocument(const string& raw_query, int document_id) const;

private:

    struct DocumentInfo
    {
        DocumentStatus status;
        int rating;
    };

    struct QueryWord
    {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string& text) const;

    struct Query
    {
        set<string> plus_words;
        set<string> minus_words;
    };

    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentInfo> document_info;
    vector<int> document_ids;

    bool IsStopWord(const string& word) const
    {
        return stop_words_.count(word) > 0;
    }

    static bool IsValidWord(const string& word);

    vector<string> SplitIntoWordsNoStop(const string& text) const;

    static int ComputeAverageRating(const vector<int>& ratings);

    Query ParseQuery(const string& text) const;

    double ComputeWordInverseDocumentFreq(const string& word) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const;
};


template<typename StringContainer>
inline SearchServer::SearchServer(const StringContainer& stop_words) : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
{
    for (const auto& stop_word : stop_words_)
    {
        if (IsValidWord(stop_word) == false)
        {
            throw invalid_argument("invalid argument"s);
        }
    }
}

template<typename DocumentPredicate>
inline vector<SearchServer::Document> SearchServer::FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const
{
    const Query query = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs)
        {
            if (abs(lhs.relevance - rhs.relevance) < ALLOWED_DISCREPANCY)
            {
                return lhs.rating > rhs.rating;
            }
            else
            {
                return lhs.relevance > rhs.relevance;
            }
        });

    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT)
    {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}

template<typename DocumentPredicate>
inline vector<SearchServer::Document> SearchServer::FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const
{
    map<int, double> document_to_relevance;

    for (const string& word : query.plus_words)
    {
        if (word_to_document_freqs_.count(word) == 0)
        {
            continue;
        }

        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto& document : word_to_document_freqs_.at(word))
        {
            if (document_predicate(document.first, document_info.at(document.first).status, document_info.at(document.first).rating))
            {
                document_to_relevance[document.first] += document.second * inverse_document_freq;
            }
        }
    }

    for (const string& word : query.minus_words)
    {
        if (word_to_document_freqs_.count(word) == 0)
        {
            continue;
        }
        for (const auto& document : word_to_document_freqs_.at(word))
        {
            document_to_relevance.erase(document.first);
        }
    }

    vector<Document> matched_documents;
    for (const auto& document : document_to_relevance)
    {
        matched_documents.push_back({ document.first, document.second, document_info.at(document.first).status, document_info.at(document.first).rating });
    }
    return matched_documents;
}

