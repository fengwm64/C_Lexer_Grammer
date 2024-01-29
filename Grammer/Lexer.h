#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

#define DESCRIBE 4000
#define TYPE 4001
#define STRING 24
#define DIGIT 26
#define IDENTIFER 25


// 定义链表节点的结构体
struct Node
{
    int type;
    string lexeme;
    int line;
    int column;
    int length;
    Node *next;
};

// 词法分析类
class Lexer
{
public:
    Lexer();
    void scanner(const char *);

protected:
    Node *head;
    Node *tail;
    vector<pair<const char *, int> > keyMap;
    vector<pair<const char *, int> > opMap;
    vector<pair<const char *, int> > limMap;

    void displayList();
    static Node *createNode(const string &line);
};
