#include "search_server.h"

SearchServer::SearchServer(const string& stop_words) : SearchServer(SplitIntoWords(stop_words)) {};

void SearchServer::AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& rating)
{
    if (document_id < 0)
    {
        throw invalid_argument("the document is negative");
    }
    if (document_info.count(document_id))
    {
        throw invalid_argument("repeat document");
    }
    else
    {
        const vector<string> words = SplitIntoWordsNoStop(document);

        for (const string& word : words)
        {
            word_to_document_freqs_[word][document_id] += 1.0 / words.size();
        }

        document_info.emplace(document_id, DocumentInfo{ status, ComputeAverageRating(rating) });
        document_ids.push_back(document_id);
    }
}

vector<SearchServer::Document> SearchServer::FindTopDocuments(const string& raw_query, DocumentStatus status) const
{
    return FindTopDocuments(raw_query, [&status](int document_id, DocumentStatus new_status, int rating)
        {
            return new_status == status;
        });
}

vector<SearchServer::Document> SearchServer::FindTopDocuments(const string& raw_query) const
{
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

size_t SearchServer::GetDocumentCount() const
{
    return document_info.size();
}

size_t SearchServer::GetDocumentId(int index) const 
{
    return document_ids.at(index);
}

vector<string> SearchServer::MatchDocument(const string& raw_query, int document_id) const 
{
    vector<string>match_document;

    auto query = ParseQuery(raw_query);

    for (const auto& word : query.plus_words)
    {
        if (word_to_document_freqs_.count(word) == 0)
        {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id))
        {
            match_document.push_back(word);
        }
    }

    for (const auto& word : query.minus_words)
    {
        if (word_to_document_freqs_.count(word) == 0)
        {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id))
        {
            match_document.clear();
            break;
        }
    }

    return match_document;
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const 
{
    if (IsValidWord(text) == false)
    {
        throw invalid_argument("invalid character");
    }
    else
    {
        vector<string> words;

        for (const string& word : SplitIntoWords(text))
        {
            if (!IsStopWord(word))
            {
                words.push_back(word);
            }
        }
        return words;
    }
}

int SearchServer::ComputeAverageRating(const vector<int>& ratings) 
{
    if (ratings.empty())
    {
        return 0;
    }

    int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);

    return rating_sum / static_cast<int>(ratings.size());
}


SearchServer::QueryWord SearchServer::ParseQueryWord(string& text) const
{
    bool is_minus = false;

    if (text[0] == '-')
    {
        is_minus = true;
        text = text.substr(1);
    }
    if (text.empty() || text[0] == '-' || !IsValidWord(text))
    {
        throw invalid_argument("invalid character int findtop");
    }
    return { text, is_minus, IsStopWord(text) };
}

bool SearchServer::IsValidWord(const string& word) 
{
    return none_of(word.begin(), word.end(), [](char ch) {return ch >= '\0' && ch < ' '; });
}

SearchServer::Query SearchServer::ParseQuery(const string& text) const
{
    Query query;

    for (string& word : SplitIntoWords(text))
    {
        const QueryWord query_word = ParseQueryWord(word);
        if (!query_word.is_stop)
        {
            if (query_word.is_minus)
            {
                query.minus_words.insert(query_word.data);
            }
            else
            {
                query.plus_words.insert(query_word.data);
            }
        }
    }
    return query;
}

double SearchServer::ComputeWordInverseDocumentFreq(const string& word) const 
{
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}

ostream& operator<<(ostream& out, const SearchServer::Document& doc)
{
    out << "{ ID: " << doc.id << ", RELEVANCE: " << doc.relevance << ", RATING: " << doc.rating << ", STATUS : " << static_cast<int>(doc.status);
    return out;
}
