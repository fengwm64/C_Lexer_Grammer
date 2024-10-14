#ifndef _SYNANALYSIS_H
#define _SYNANALYSIS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Lexer.h"
#include "Grammer.h"

#define GRAMMAR_ARROW 2000   //->
#define GRAMMAR_OR 2001      // |
#define GRAMMAR_NULL 2002    // 空值
#define GRAMMAR_SPECIAL 2003 // 特殊符号#
#define GRAMMAR_BASE 2010    // 动态生成的基值

#define Stack_Size 5000 // 栈的最大容量
#define Max_Proc 500    // 产生式的最大数量
#define Max_Length 500  // 产生式的最大长度
#define Max_NonTer 60   // 非终结符集合的最大数量
#define Max_Ter 60      // 终结符集合的最大数量
#define Max_Length2 100 // 分析表中产生式右部的长度

using namespace std;

// 栈元素
typedef struct
{
    char *token;
    int type;
    int line;
    int col;
} elemType;
// 分析栈
typedef struct
{
    elemType elem[Stack_Size];
    int top;
} SeqStack;

// 语法分析器类
class Grammer : public Lexer
{
public:
    explicit Grammer(const char *file_path);
    void getFirstSet();
    void getFollowSet();
    void getSelectSet();
    void getTable();
    void runAnalysis();

    void printProc();
    void printTer();
    void printNonTer();
    void printFirstSet();
    void printFollowSet();
    void printSelectSet();
    const char *searchMap(int num);
    void showStack(SeqStack *S);

private:
    int procNum;                                      // 产生式数量，proc的维数都是从1开始的
    int proc[Max_Proc][Max_Length]{};                 // 产生式的数组，里边存储了终结符或者非终结符对应的编号
    int first[Max_Proc][Max_Length]{};                // 存储First集的数组
    int follow[Max_Proc][Max_Length]{};               // 存储Follow集的数组
    int select_[Max_Proc][Max_Length]{};              // 存储Select集的数组
    int analytable[Max_NonTer][Max_Ter][Max_Length2]; // 预测分析表，analytable[i][j]表示非终结符i和终结符j对应的产生式编号
    int connectFirst[Max_Length]{};                   // 将某些First集结合起来的集合
    int firstVisit[Max_Proc]{};                       // 记录某非终结符的First集是否已经求过
    int followVisit[Max_Proc]{};                      // 记录某非终结符的Follow集是否已经求过
    int empty[Max_Proc]{};                            // 可推出空的非终结符的编号
    int emptyRecu[Max_Proc]{};                        // 在求可推出空的非终结符的编号集时使用的防治递归的集合
    int followRecu[Max_Proc]{};                       // 在求Follow集时使用的防治递归的集合
    fstream analyResult;                              // 分析结果输出文件流
    vector<pair<const char *, int>> nonTerMap;        // 非终结符映射表,不可重复
    vector<pair<const char *, int>> terMap;           // 终结符映射表,不可重复
    vector<pair<const char *, int>> specialMap;       // 文法中的特殊符号映射表，包括-> | $(空)

    // 拆分包含" | "的文法行
    static void split(int p[][Max_Length], int &line, int orNum);
    // 查找word对应编码
    int seekWordNum(char *word);
    // 计算能够直接推出空的非终结符集合
    void getNullSet(int currentNum);
    // 将来源集合合并至目标集合中
    void joinSet(int *destination, const int *source, int type);
    // 计算一个非终结符的first集(传入的参数是在非终结符集合中的序号)
    void getSingleFirstSet(int i);
    // 将First结合起来的函数
    void connectFirstSet(int *p);
    // 判断该非终结符是否能推出空，但终结符也可能传入，但没关系
    int canNonTer2Null(int currentNon);
    void getSingleFollowSet(int i);
    int dynamicNonTer(char *word);
    int isNonTer(int n);
    int isTer(int n);
    int inEmpty(int n);
    int inEmptyRecu(int n);
    int inFollowRecu(int n);
    int inProcRight(int n, const int *p);
    char *getWord(ifstream &infstream, char *array, char &ch);
    int findNonTerIndex(int nonTerSymbol);
    int findTerIndex(int TerSymbol);
    int getProcLength(const int proc[]);
};

#endif
