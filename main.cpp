#include <Windows.h>

#include "read_input_functions.h"
#include "search_server.h"
#include "create_search_server.h"
#include "test_server.h"

void PrintResultQuery(const SearchServer& search_server, const string& query)
{

    cout << "Search results in the list of 'Actual' :" << endl;
    for (const auto& document : search_server.FindTopDocuments(query, SearchServer::DocumentStatus::ACTUAL))
    {
        vector<string> doc = search_server.MatchDocument(query, document.id);

        string str = GetPlusWords(doc);

        cout << "{ document_id = "s << document.id << ", " << "relevance = "s << document.relevance << ", "s << "rating = "s << document.rating << ", "s << "status = "s << static_cast<int>(document.status) << ", "s << "words+ = ["s << str << "] }"s << endl;
    }
    cout << endl << endl;

    cout << "Search results in the list of 'Deleted' :" << endl;
    for (const auto& document : search_server.FindTopDocuments(query, SearchServer::DocumentStatus::DELETED))
    {
        vector<string> doc = search_server.MatchDocument(query, document.id);

        string str = GetPlusWords(doc);

        cout << "{ document_id = "s << document.id << ", " << "relevance = "s << document.relevance << ", "s << "rating = "s << document.rating << ", "s << "status = "s << static_cast<int>(document.status) << ", "s << "words+ = ["s << str << "] }"s << endl;
    }
    cout << endl;

    cout << "Search results in the list of 'Absent' :" << endl;
    for (const auto& document : search_server.FindTopDocuments(query, SearchServer::DocumentStatus::ABSENT))
    {
        vector<string> doc = search_server.MatchDocument(query, document.id);

        string str = GetPlusWords(doc);

        cout << "{ document_id = "s << document.id << ", " << "relevance = "s << document.relevance << ", "s << "rating = "s << document.rating << ", "s << "status = "s << static_cast<int>(document.status) << ", "s << "words+ = ["s << str << "] }"s << endl;
    }
    cout << endl;
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