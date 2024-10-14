#include "Lexer.h"

using namespace std;

Lexer::Lexer()
{
    string line;
    string token;
    int num;
    head = tail = nullptr;

    // 初始化keyMap
    ifstream file_1("./Grammer/Mapping/keyMapping");
    if (!file_1.is_open())
    {
        cerr << "错误：无法打开文件\n";
        return;
    }
    keyMap.clear();
    while (getline(file_1, line))
    {
        stringstream ss(line);
        ss >> token >> num;
        char* tokenCopy = new char[token.size() + 1];
        strcpy(tokenCopy, token.c_str());
        keyMap.emplace_back(tokenCopy,num);
    }
    file_1.close();

    // 初始化opMap
    ifstream file_2("./Grammer/Mapping/opMapping");
    if (!file_2.is_open())
    {
        cerr << "错误：无法打开文件\n";
        return;
    }
    opMap.clear();
    while (getline(file_2, line))
    {
        stringstream ss(line);
        ss >> token >> num;
        char* tokenCopy = new char[token.size() + 1];
        strcpy(tokenCopy, token.c_str());
        opMap.emplace_back(tokenCopy,num);
    }
    file_2.close();

    // 初始化limMap
    ifstream file_3("./Grammer/Mapping/limMapping");
    if (!file_3.is_open())
    {
        cerr << "错误：无法打开文件\n";
        return;
    }
    limMap.clear();
    while (getline(file_3, line))
    {
        stringstream ss(line);
        ss >> token >> num;
        char* tokenCopy = new char[token.size() + 1];
        strcpy(tokenCopy, token.c_str());
        limMap.emplace_back(tokenCopy,num);
    }
    file_3.close();
}

// 从文件读取内容并构造链表的函数
void Lexer::scanner(const char *filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "错误：无法打开文件\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        Node *newNode = createNode(line);
        if (head == nullptr)
            head = tail = newNode;
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    file.close();
}

// 从文件内容的一行创建新节点的函数
Node *Lexer::createNode(const string &line)
{
    Node *newNode = new Node;

    // 使用 stringstream 来解析每一列的内容
    stringstream ss(line);

    // 读取每一列的内容并设置到节点中
    ss >> newNode->type >> newNode->lexeme >> newNode->line >> newNode->column >> newNode->length;

    // 设置 next 指针为 nullptr，因为这是新链表的最后一个节点
    newNode->next = nullptr;

    return newNode;
}

// 显示链表内容的函数
void Lexer::displayList()
{
    Node *current = head;
    while (current != nullptr)
    {
        cout << current->type << "\t" << current->lexeme << "\t"
             << current->line << "\t" << current->column << "\t" << current->length << "\n";
        current = current->next;
    }
}
