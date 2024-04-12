#include <Windows.h>

#include "read_input_functions.h"
#include "search_server.h"
#include "create_search_server.h"
#include "test_server.h"
#include "paginator.h"

void PrintResultQuery(const SearchServer& server, const string& query)
{
    int size_pages = 2;
    vector<SearchServer::Document> all_documents;

    auto ActualDocumentsPredicate = [](int document_id, SearchServer::DocumentStatus status, int rating) 
        {
        return status == SearchServer::DocumentStatus::ACTUAL;
        };

    auto DeletedDocumentsPredicate = [](int document_id, SearchServer::DocumentStatus status, int rating) 
        {
        return status == SearchServer::DocumentStatus::DELETED;
        };

    auto AbsentDocumentsPredicate = [](int document_id, SearchServer::DocumentStatus status, int rating) {
        return status == SearchServer::DocumentStatus::ABSENT;
        };

    const auto actual_documents = server.FindTopDocuments(query, ActualDocumentsPredicate);
    all_documents.insert(all_documents.end(), actual_documents.begin(), actual_documents.end());

    const auto deleted_documents = server.FindTopDocuments(query, DeletedDocumentsPredicate);
    all_documents.insert(all_documents.end(), deleted_documents.begin(), deleted_documents.end());

    const auto absent_documents = server.FindTopDocuments(query, AbsentDocumentsPredicate);
    all_documents.insert(all_documents.end(), absent_documents.begin(), absent_documents.end());

    const auto pages = Paginate(all_documents, size_pages);

    size_t count_page = 0;
    for (const auto& page : pages) 
    {
        count_page++;
        cout << "Страница №" << count_page << endl;
        for (const auto& doc : page) 
        {
            vector<string> doc_words = server.MatchDocument(query, doc.id);
            string str = GetPlusWords(doc_words);
            cout << doc << ", +WORDS : [" << str << "] } " << endl;
        }
        cout << endl;
    }
}

int main() 
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    TestSearchServer(); // автоматические тесты
    cout << endl;

    //const SearchServer search_server = CreateSearchServer(); // пользовательский ввод
    const SearchServer search_server = CreateAutoSearchServer();
    
    cout << "Enter a search query : ";
    string query = ReadLine();
    cout << endl << endl;

    PrintResultQuery(search_server, query);
}