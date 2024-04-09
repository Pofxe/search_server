#pragma once
#include "search_server.h"

template <typename T, typename U>
ostream& operator<<(ostream& out, const pair<T, U>& container);

template <typename T>
void Print(ostream& out, const T& container);

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container);

template <typename T>
ostream& operator<<(ostream& out, const set<T>& container);

template <typename T, typename U>
ostream& operator<<(ostream& out, const map<T, U>& container);

void TestExcludeStopWordsFromAddedDocumentContent();

void TestFindAddedDocumentByDocumentWord();

void TestExcludeDocumentsWithMinusWordsFromResults();

void TestMatchedDocuments();

void TestSortResultsByRelevance();

void TestCalculateDocumentRating();
 
void TestDocumentSearchByPredicate();

void TestDocumentSearchByStatus();

void TestCalculateRelevance();

void TestSearchServer();