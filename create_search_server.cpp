#include "create_search_server.h"

SearchServer::DocumentStatus SetDocumentStatus()
{
    cout << "Enter status document(1.ACTUAL, 2.DELETED, 3.ABSENT) : ";
    int status = 0;
    cin >> status;

    SearchServer::DocumentStatus document = SearchServer::DocumentStatus::ABSENT;

    if (status == 1)
    {
        document = SearchServer::DocumentStatus::ACTUAL;
    }
    else if (status == 2)
    {
        document = SearchServer::DocumentStatus::DELETED;
    }
    else
    {
        document = SearchServer::DocumentStatus::ABSENT;
    }

    return document;
}

SearchServer CreateAutoSearchServer() // функци€ с уже заполненными данными,потому что надоело писать ручками)))
{
    SearchServer search_server(SplitIntoWords("и в на дл€ за"s));

    search_server.AddDocument(1, "пропала кошка , черного цвета , окрас короткошерстный , кличка ћурка", SearchServer::DocumentStatus::ACTUAL, { 7, 8, 2, 1, 4, 6, 5, 9, 10 });
    search_server.AddDocument(2, "ищем потер€вшегос€ пса , порода : лабрадор, окрас светлый , кличка –эмбо", SearchServer::DocumentStatus::ACTUAL, { 3, 1, 5, 7, 9, 4, 2, 8, 10, 6 });
    search_server.AddDocument(3, "обнаружена бездомна€ собака , кобель , среднего размера , игривый и дружелюбный", SearchServer::DocumentStatus::ACTUAL, { 4, 6, 2, 8, 1, 5, 9, 10, 7, 3 });
    search_server.AddDocument(4, "пропала кошечка , трехцветна€ , стерилизована , ответственный владелец", SearchServer::DocumentStatus::DELETED, { 9, 7, 3, 6, 2, 1, 10, 8, 5, });
    search_server.AddDocument(5, "найден милый котенок , серого окраса , возраст примерно 2 мес€ца", SearchServer::DocumentStatus::ACTUAL, { 2, 4, 8, 9, 10, 7, 1, 3, 6, 5 });
    search_server.AddDocument(6, "потер€лась домашн€€ черепаха , уникальные отметины на панцире", SearchServer::DocumentStatus::ABSENT, { 1, 5, 6, 8, 4, 3, 9, 7, 10 });
    search_server.AddDocument(7, "ищем владельца потер€вшейс€ попугайчицы , зеленого цвета , говорит на русском", SearchServer::DocumentStatus::ABSENT, { 3, 2, 10, 1, 4, 9, 5, 6, 8, 7 });
    search_server.AddDocument(8, "пропал кролик , белого цвета , ласковый и дружелюбный", SearchServer::DocumentStatus::ACTUAL, { 6, 8, 5, 10, 7, 9, 4, 2, 1, 3 });
    search_server.AddDocument(9, "обнаружен потер€вшийс€ аквариумный рыбки , разноцветные , разновидность гуппи", SearchServer::DocumentStatus::ACTUAL, { 7, 9, 1, 4, 3, 10, 6, 2, 8 });
    search_server.AddDocument(10, "ищем владельца потер€вшегос€ хом€чка , коричневый окрас , ласковый", SearchServer::DocumentStatus::ACTUAL, { 5, 3, 6, 1, 7, 9, 4, 10, 2, 8 });
    search_server.AddDocument(11, "пропала домашн€€ птица , канарейка , желтого цвета , поюща€", SearchServer::DocumentStatus::ACTUAL, { 8, 6, 9, 2, 5, 7, 1, 10, 4 });
    search_server.AddDocument(12, "обнаружен потер€вшийс€ грызун , морска€ свинка , светлый окрас , ласкова€", SearchServer::DocumentStatus::ACTUAL, { 1, 7, 8, 3, 10, 2, 5, 4, 6, 9 });
    search_server.AddDocument(13, "ищем владельца потер€нной черной лошади , бела€ звезда на лбу", SearchServer::DocumentStatus::DELETED, { 2, 5, 4, 6, 8, 1, 10, 3, 7, 9 });
    search_server.AddDocument(14, "пропал семейный хаски , серо-белого цвета , голубые глаза , нагрудный ремень", SearchServer::DocumentStatus::ACTUAL, { 9, 3, 7, 4, 8, 2, 1, 6, 5, 10 });
    search_server.AddDocument(15, "обнаружен потер€вшийс€ кот , рыжего окраса , агрессивен к посторонним", SearchServer::DocumentStatus::ABSENT, { 10, 1, 9, 8, 6, 3, 5, 4, 7, 2 });

    return search_server;
}

SearchServer CreateSearchServer() // функци€ создани€ объекта класса
{
    cout << "Enter the stop-words : ";
    const vector<string> stop_words_vector = SplitIntoWords(ReadLine());
    cout << endl;

    SearchServer search_server{ stop_words_vector };

    cout << "Number of documents to be added : "s;
    const int document_count = ReadLineWithNumber();
    cout << endl;

    cout << "Enter "s << document_count << " documents :"s << endl;
    for (int document_id = 1; document_id <= document_count; ++document_id)
    {
        cout << document_id << ". "s;
        const string document = ReadLine();
        cout << endl;

        cout << "Enter the number of ratings of the document : "s;
        int ratings_size;
        cin >> ratings_size;
        cout << endl;

        vector<int> ratings(ratings_size, 0);

        cout << "Enter "s << ratings_size << " site ratings : "s;
        for (int& rating : ratings)
        {
            cin >> rating;
        }
        cout << endl;

        SearchServer::DocumentStatus status = SetDocumentStatus();

        search_server.AddDocument(document_id, document, status, ratings);
    }
    cout << endl;

    return search_server;
}