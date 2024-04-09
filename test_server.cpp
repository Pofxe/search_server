#include "test_server.h"
#include "test_define.h"

template <typename T, typename U>
ostream& operator<<(ostream& out, const pair<T, U>& container)
{
    return out << container.first << ": " << container.second;
}

template <typename T>
void Print(ostream& out, const T& container)
{
    bool is_first = true;
    for (const auto& element : container)
    {
        if (is_first)
        {
            out << element;
            is_first = false;
        }
        else
        {
            out << ", "s << element;
        }
    }
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container)
{
    out << "["s;
    Print(out, container);
    out << "]"s;

    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const set<T>& container)
{
    out << "{"s;
    Print(out, container);
    out << "}"s;

    return out;
}

template <typename T, typename U>
ostream& operator<<(ostream& out, const map<T, U>& container)
{
    out << "{"s;
    Print(out, container);
    out << "}"s;

    return out;
}


void TestExcludeStopWordsFromAddedDocumentContent()
{
    const int doc_id = 1;
    const int doc_id2 = 2;
    const string content = "пушистый кот пушистый хвост"s;
    const string content2 = "пушистый пес пушистый хвост"s;
    const vector<int> ratings = { 1, 2, 3 };

    {
        SearchServer server("и в на"s);
        server.AddDocument(doc_id, content, SearchServer::DocumentStatus::ACTUAL, ratings);

        const auto found_docs = server.FindTopDocuments("кот"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_EQUAL(found_docs[0].id, doc_id);
    }

    {
        SearchServer server("и в на"s);
        server.AddDocument(doc_id, content, SearchServer::DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, SearchServer::DocumentStatus::ABSENT, ratings);

        const auto found_docs = server.FindTopDocuments("пушистый хвост"s);
        ASSERT_EQUAL(found_docs.size(), 1);
        ASSERT_EQUAL(found_docs[0].id, doc_id);
    }
}

void TestFindAddedDocumentByDocumentWord()
{
    const int doc_id = 1;
    const string content = "пушистый кот пушистый хвост"s;
    const vector<int> ratings = { 1, 2, 3 };

    {
        SearchServer server("и в на"s);

        ASSERT_EQUAL(server.GetDocumentCount(), 0);

        server.AddDocument(doc_id, content, SearchServer::DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);

        server.AddDocument(doc_id + 1, "пушистая черная собака"s, SearchServer::DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 2);

        const auto found_docs = server.FindTopDocuments("кот"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_EQUAL(found_docs[0].id, doc_id);
    }
}

void TestExcludeDocumentsWithMinusWordsFromResults()
{
    SearchServer server("и в на"s);
    server.AddDocument(1, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, { 1,2,3 });
    server.AddDocument(2, "пушистый черный пес"s, SearchServer::DocumentStatus::ACTUAL, { 1,2,3 });

    {
        const auto found_docs = server.FindTopDocuments("пушистый -пес"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_EQUAL(found_docs[0].id, 1);
    }

    {
        const auto found_docs = server.FindTopDocuments("пушистый -кот"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_EQUAL(found_docs[0].id, 2);
    }

    {
        const auto found_docs = server.FindTopDocuments("красивый -кот и -пес");
        ASSERT_EQUAL(found_docs.size(), 0);
    }

}
 
void TestMatchedDocuments()
{
    SearchServer server("и в на"s);
    server.AddDocument(1, "пушистый кот и черная собака в ошейнике"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });

    {
        const auto matched_words = server.MatchDocument("собака и кот"s, 1);
        const vector<string> expected_result = { "кот"s, "собака"s };
        ASSERT_EQUAL(expected_result, matched_words);
    }

    {
        const auto matched_words = server.MatchDocument("собака и -кот"s, 1);
        const vector<string> expected_result = {}; 

        ASSERT_EQUAL(expected_result, matched_words);
        ASSERT(matched_words.empty());
    }
}

void TestSortResultsByRelevance()
{
    SearchServer server("и в на"s);
    server.AddDocument(1, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(2, "белый кот и модный ошейник"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(3, "ухоженный скворец геннадий"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });

    {
        const auto found_docs = server.FindTopDocuments("пушистый ухоженный кот"s);
        ASSERT_EQUAL(found_docs.size(), 3u);

        for (size_t i = 1; i < found_docs.size(); i++)
        {
            ASSERT(found_docs[i - 1].relevance >= found_docs[i].relevance);
        }
    }
}

void TestCalculateDocumentRating()
{
    SearchServer server("и в на"s);
    const vector<int> ratings = { 10, 11, 3 };
    const int average = (10 + 11 + 3) / 3;
    server.AddDocument(0, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, ratings);

    {
        const auto found_docs = server.FindTopDocuments("пушистый ухоженный кот"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_EQUAL(found_docs[0].rating, average);
    }
}

void TestDocumentSearchByPredicate()
{
    SearchServer server("и в на"s);
    server.AddDocument(1, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(2, "белый кот и модный ошейник"s, SearchServer::DocumentStatus::ABSENT, { -1, -2, -3 });
    const auto found_docs = server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, SearchServer::DocumentStatus status, int rating)
        {
            return rating > 0 && status == SearchServer::DocumentStatus::ACTUAL;
        });

    {
        ASSERT_EQUAL(found_docs.size(), 1u);
        ASSERT_HINT(found_docs[0].id == 1, "Wrong predicate");
    }
}

void TestDocumentSearchByStatus()
{
    SearchServer server("и в на"s);
    int id1 = 1;
    int id2 = 2;
    int id3 = 3;

    server.AddDocument(id1, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, { 7 });
    server.AddDocument(id2, "белый кот и модный ошейник"s, SearchServer::DocumentStatus::ABSENT, { 6 });
    server.AddDocument(id3, "ухоженный скворец геннадий"s, SearchServer::DocumentStatus::DELETED, { 6 });

    {
        vector<SearchServer::Document> result1 = server.FindTopDocuments("пушистый ухоженный кот"s, SearchServer::DocumentStatus::ACTUAL);
        ASSERT_EQUAL(result1.size(), 1);

        vector<SearchServer::Document> result2 = server.FindTopDocuments("пушистый ухоженный кот"s, SearchServer::DocumentStatus::ABSENT);
        ASSERT_EQUAL(result2.size(), 1);

        vector<SearchServer::Document> result3 = server.FindTopDocuments("пушистый ухоженный кот"s, SearchServer::DocumentStatus::DELETED);
        ASSERT_EQUAL(result3.size(), 1);
    }
}

void TestCalculateRelevance()
{
    SearchServer server("и в на"s);
    server.AddDocument(1, "пушистый кот пушистый хвост"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(2, "белый кот и модный ошейник"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    server.AddDocument(3, "ухоженный скворец геннадий"s, SearchServer::DocumentStatus::ACTUAL, { 1, 2, 3 });
    const auto found_docs = server.FindTopDocuments("пушистый ухоженный кот"s);
    double relevance_query = log((3 * 1.0) / 1) * (2.0 / 4.0) + log((3 * 1.0) / 2) * (1.0 / 4.0);

    {
        ASSERT_HINT(fabs(found_docs[0].relevance - relevance_query) < 1e-6, "Wrong calculation relevance");
    }
}

void TestSearchServer()
{
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestFindAddedDocumentByDocumentWord);
    RUN_TEST(TestExcludeDocumentsWithMinusWordsFromResults);
    RUN_TEST(TestMatchedDocuments);
    RUN_TEST(TestSortResultsByRelevance);
    RUN_TEST(TestCalculateDocumentRating);
    RUN_TEST(TestDocumentSearchByPredicate);
    RUN_TEST(TestDocumentSearchByStatus);
    RUN_TEST(TestCalculateRelevance);
}