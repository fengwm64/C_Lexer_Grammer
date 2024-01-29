#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <queue>
#include <fstream>
#include <cctype>

using namespace std;

#define MAXID 5000

/* *
 * ------------------------- .lex文件语法说明 ------------------------- *
 * - %{ %}  声明区块（包括变量的声明、符号常量的声明、希望出现在目标C源码中的代码）
 * - %@ %@  定义区块（字母定义、数字定义等等）
 * - %% %%  规则区块（用正规表达式描述词法规则，每个非结束正规表达式以%$结束）
 * */

struct Node
{
    Node(int fromId, string input, int toId)
    {
        this->fromId = fromId;
        this->input = input;
        this->toId = toId;
    }

    int fromId;
    string input;
    int toId;
};

// 词法分析器类
class myLex
{
public:
    // 构造函数
    myLex(string lexFile, string outCodeFile);
    // 生成词法分析器代码
    void generate();
    // 解析lex源文件
    void scanner();
    // 输出错误
    static inline void printErr(int line, const string &str, const string &mess);
    // 输出警告
    static inline void printWarn(int line, const string &str, const string &mess);
    // 解析定义区块
    void getFunc(string str, int line);
    // 解析正规式
    void getRegular(string str, int line);
    // 添加NFA节点
    inline void addNFANode(int fromId, string input, int toId);
    // 添加DFA节点
    inline void addDFANode(int fromId, string input, int toId);
    // 正规式转NFA
    void regrToNFA(vector<pair<string, bool>> regr, int startId, int endId, int &itemId);
    // 生成NFA
    void getNFA();
    // 生成DFA
    void getDFA();
    // nfa查询
    void findBeGo(int i, const string &input, vector<int> *state);
    // 获取nfa目标状态集合
    vector<int> *getState(vector<int> *fromState, const string &input);
    // 获取终态
    void getVt(const map<vector<int>, int> &stateMap);
    // 类运行主函数
    void run();

private:
    string lexFile;
    string outCFile;
    // 自定义输入对应方法体
    map<string, string> funcMap;
    // 正规式
    vector<vector<pair<string, bool>>> regrVet;
    // 方法体
    vector<string> funcVet;
    // NFA
    vector<Node> nfaVet[MAXID];
    // DFA
    vector<Node> dfaVet[MAXID];
    // 储存所有输入
    set<string> inputSet;
    // DFAend
    int dfaEnd[MAXID];
    // NFAend
    int nfaEnd[MAXID];
    int nfaNum;
    int dfaNum;
};