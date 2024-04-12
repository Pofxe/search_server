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

SearchServer CreateAutoSearchServer() // ������� � ��� ������������ �������,������ ��� ������� ������ �������)))
{
    SearchServer search_server(SplitIntoWords("� � �� ��� ��"s));

    search_server.AddDocument(1, "������� ����� , ������� ����� , ����� ��������������� , ������ �����", SearchServer::DocumentStatus::DELETED, { 7, 8, 2, 1, 4, 6, 5, 9, 10 });
    search_server.AddDocument(2, "���� ������������� ��� , ������ : ��������, ����� ������� , ������ �����", SearchServer::DocumentStatus::ACTUAL, { 3, 1, 5, 7, 9, 4, 2, 8, 10, 6 });
    search_server.AddDocument(3, "���������� ��������� ������ , ������ , �������� ������� , ������� � �����������", SearchServer::DocumentStatus::ACTUAL, { 4, 6, 2, 8, 1, 5, 9, 10, 7, 3 });
    search_server.AddDocument(4, "������� ������� , ����������� , ������������� , ������������� ��������", SearchServer::DocumentStatus::DELETED, { 9, 7, 3, 6, 2, 1, 10, 8, 5, });
    search_server.AddDocument(5, "������ ����� ������� , ������ ������ , ������� �������� 2 ������", SearchServer::DocumentStatus::ACTUAL, { 2, 4, 8, 9, 10, 7, 1, 3, 6, 5 });
    search_server.AddDocument(6, "���������� �������� �������� , ���������� �������� �� �������", SearchServer::DocumentStatus::ABSENT, { 1, 5, 6, 8, 4, 3, 9, 7, 10 });
    search_server.AddDocument(7, "���� ��������� ������������ ����������� , �������� ����� , ������� �� �������", SearchServer::DocumentStatus::ABSENT, { 3, 2, 10, 1, 4, 9, 5, 6, 8, 7 });
    search_server.AddDocument(8, "������ ������ , ������ ����� , �������� � �����������", SearchServer::DocumentStatus::ACTUAL, { 6, 8, 5, 10, 7, 9, 4, 2, 1, 3 });
    search_server.AddDocument(9, "��������� ������������ ����������� ����� , ������������ , ������������� �����", SearchServer::DocumentStatus::ACTUAL, { 7, 9, 1, 4, 3, 10, 6, 2, 8 });
    search_server.AddDocument(10, "���� ��������� ������������� ������� , ���������� ����� , ��������", SearchServer::DocumentStatus::ACTUAL, { 5, 3, 6, 1, 7, 9, 4, 10, 2, 8 });
    search_server.AddDocument(11, "������� �������� ����� , ��������� , ������� ����� , ������", SearchServer::DocumentStatus::ACTUAL, { 8, 6, 9, 2, 5, 7, 1, 10, 4 });
    search_server.AddDocument(12, "��������� ������������ ������ , ������� ������ , ������� ����� , ��������", SearchServer::DocumentStatus::ACTUAL, { 1, 7, 8, 3, 10, 2, 5, 4, 6, 9 });
    search_server.AddDocument(13, "���� ��������� ���������� ������ ������ , ����� ������ �� ���", SearchServer::DocumentStatus::DELETED, { 2, 5, 4, 6, 8, 1, 10, 3, 7, 9 });
    search_server.AddDocument(14, "������ �������� ����� , ����-������ ����� , ������� ����� , ��������� ������", SearchServer::DocumentStatus::ACTUAL, { 9, 3, 7, 4, 8, 2, 1, 6, 5, 10 });
    search_server.AddDocument(15, "��������� ������������ ��� , ������ ������ , ���������� � �����������", SearchServer::DocumentStatus::ABSENT, { 10, 1, 9, 8, 6, 3, 5, 4, 7, 2 });

    return search_server;
}

SearchServer CreateSearchServer() // ������� �������� ������� ������
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