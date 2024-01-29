#include "Grammer.h"

// 构造函数
Grammer::Grammer(const char *file_path)
{
    ifstream infstream(file_path); // 文件输入流
    char ch;                       // 用于存储读取的字符
    char array[30];                // 存储读取的单词的字符数组
    char *word;                    // 指向存储单词的动态分配内存
    int i;                         // 循环计数器
    int codeNum;                   // 存储单词对应的编码
    int line = 1;                  // 记录当前处理的行数
    int count = 0;                 // 记录当前处理的单词在一行中的位置
    int orNum = 0;                 // 记录当前处理的" | "的数量

    if (!infstream.is_open())
    {
        cout << "文法文件打开失败！请检查路径" << endl;
        exit(1);
    }

    // 初始化特殊符号映射表等
    specialMap.clear();
    specialMap.emplace_back("->", GRAMMAR_ARROW);
    specialMap.emplace_back("|", GRAMMAR_OR);
    specialMap.emplace_back("$", GRAMMAR_NULL);
    specialMap.emplace_back("#", GRAMMAR_SPECIAL);
    nonTerMap.clear();
    terMap.clear();

    // 清空
    memset(proc, -1, sizeof(proc));
    memset(first, -1, sizeof(first));
    memset(follow, -1, sizeof(follow));
    memset(select_, -1, sizeof(select_));
    memset(connectFirst, -1, sizeof(connectFirst));
    memset(firstVisit, 0, sizeof(firstVisit));
    memset(followVisit, 0, sizeof(followVisit));
    memset(empty, -1, sizeof(empty));
    memset(emptyRecu, -1, sizeof(emptyRecu));
    memset(followRecu, -1, sizeof(followRecu));
    memset(analytable, -1, sizeof(analytable));

    ch = infstream.get();
    i = 0;

    // 循环读取文件内容，直到文件末尾
    while (ch != EOF)
    {
        // 获取一个单词
        word = getWord(infstream, array, ch);

        // 获取单词对应的编码
        codeNum = seekWordNum(word);

        // 如果不是空白行，则处理当前单词
        if (codeNum != 0)
        {
            count++;
            // 如果是" | "，记录数量
            if (codeNum == GRAMMAR_OR)
                orNum++;
            // 记录当前单词的编码到proc数组中
            proc[line][count] = codeNum;
        }

        // 如果当前字符为换行符，表示一行的处理结束，拆分"|"文法
        if (ch == '\n')
        {
            // 文法增广
            split(proc, line, orNum);
            count = 0; // 重置
            orNum = 0; // 重置
            line++;    // 行号++
            ch = infstream.get();
        }
    }

    // 产式式数量
    procNum = line - 1;
}

// ================================= tools ================================= //
const char *Grammer::searchMap(int num)
{
    // 标志符
    if (num == IDENTIFER)
        return "id";

    // 处理文法中的特殊符号
    for (auto &i : specialMap)
        if (i.second == num)
            return i.first;

    // 处理非终结符
    for (auto &i : nonTerMap)
        if (i.second == num)
            return i.first;

    // 处理终结符
    for (auto &i : terMap)
        if (i.second == num)
            return i.first;
}
// 获取文法中的一个单词
char *Grammer::getWord(ifstream &infstream, char *array, char &ch)
{
    int i = 0;

    // 跳过空格和制表符
    while (ch == ' ' || ch == '\t')
        ch = infstream.get();

    // 读取一个单词
    while (ch != ' ' && ch != '\n' && ch != EOF)
    {
        array[i++] = ch;
        ch = infstream.get();
    }

    // 跳过空格和制表符
    while (ch == ' ' || ch == '\t')
        ch = infstream.get();

    // 动态分配内存存储单词
    char *word = new char[i + 1];
    memcpy(word, array, i);
    word[i] = '\0';

    return word;
}
// 分割函数，用于将包含" | "的文法行进行拆分
void Grammer::split(int p[][Max_Length], int &line, int orNum)
{
    // 如果当前行无内容或者没有需要拆分的文法，则直接返回
    if (p[line][1] == -1 || orNum == 0)
        return;

    int head = p[line][1];       // 记录文法头部
    int push = p[line][2];       // 记录文法第二部分（紧跟在头部之后）
    int length = 0;              // 用于计算当前行文法的长度
    int right, left;             // 用于拆分文法的边界索引
    int lineTrue = line + orNum; // 计算拆分后新的行号

    // 计算当前行文法的长度
    for (length = 3;; length++)
        if (p[line][length] == -1)
            break;

    length--;

    for (left = length, right = length; left >= 2;)
    {
        // 如果找到了" | "或者已经到达行首
        if (p[line][left] == GRAMMAR_OR || left == 2)
        {
            // 将拆分的文法信息存入新的行
            p[line + orNum][1] = head;
            p[line + orNum][2] = push;
            for (int i = left + 1; i <= right; i++)
                p[line + orNum][i - left + 2] = p[line][i];
            p[line + orNum][right - left + 3] = -1;

            right = left = left - 1;
            orNum--;
        }
        else
            left--;
    }

    // 更新行号
    line = lineTrue;
}
// 查找word对应编码
int Grammer::seekWordNum(char *word)
{
    // 处理文法中的特殊符号
    for (auto &i : specialMap)
        if (strcmp(word, i.first) == 0)
            return i.second;

    // 先搜索终结符映射表中有没有此终结符
    for (auto &i : terMap)
        if (strcmp(word, i.first) == 0)
            return i.second;

    // 在关键字映射表中查找
    for (auto &i : keyMap)
    {
        if (strcmp(word, i.first) == 0)
        {
            terMap.push_back(make_pair(word, i.second));
            return i.second;
        }
    }

    // 在运算符映射表中查找
    for (auto &i : opMap)
    {
        if (strcmp(word, i.first) == 0)
        {
            terMap.push_back(make_pair(word, i.second));
            return i.second;
        }
    }

    // 在限界符映射表中查找
    for (auto &i : limMap)
    {
        if (strcmp(word, i.first) == 0)
        {
            terMap.push_back(make_pair(word, i.second));
            return i.second;
        }
    }
    // 处理标志符
    if (strcmp(word, "id") == 0)
    {
        terMap.push_back(make_pair(word, IDENTIFER));
        return IDENTIFER;
    }
    // 处理关键字、运算符、限界符表，即非终结符
    else
        return dynamicNonTer(word);
}
// 将来源集合加入至目标集合中，type = 1代表包括空（$），type = 2代表不包括空
void Grammer::joinSet(int *destination, const int *source, int type)
{
    for (int i = 0; source[i] != -1; i++)
    {
        int element = source[i];

        // 检查目标集合中是否已经包含该元素
        bool exists = false;
        for (int j = 0; destination[j] != -1; j++)
        {
            if (destination[j] == element)
            {
                exists = true;
                break;
            }
        }

        // 如果目标集合中不包含该元素，则添加
        if (!exists)
        {
            // 如果 type = 2 且元素为 GRAMMAR_NULL，则不添加
            if (type == 2 && element == GRAMMAR_NULL)
                continue;

            // 添加元素到目标集合中
            int index = 0;
            while (destination[index] != -1)
                index++;

            destination[index] = element;
            destination[index + 1] = -1;
        }
    }
}
// 查找非终结符在nonTerMap中的位置
int Grammer::findNonTerIndex(int nonTerSymbol)
{
    for (int k = 0; k < nonTerMap.size(); k++)
        if (nonTerMap[k].second == nonTerSymbol)
            return k;

    return -1;
}
// 查找终结符在TerMap中的位置
int Grammer::findTerIndex(int TerSymbol)
{
    for (int k = 0; k < terMap.size(); k++)
        if (terMap[k].second == TerSymbol)
            return k;

    return -1;
}
// 获取产生式长度
int Grammer::getProcLength(const int proc[])
{
    int length = 3;
    for (length = 3; proc[length] != -1; length++)
        ;
    return length;
}
// 计算能够直接推出空的非终结符集合
void Grammer::getNullSet(int currentNum)
{
    // 遍历所有产生式
    for (int j = 1; j <= procNum; j++)
    {
        // 如果右边的第一个符号是当前字符，且产生式长度只有1
        if (proc[j][3] == currentNum && proc[j][4] == -1)
        {
            // 将产生式左部加入能够推出空的集合
            int item[2];
            item[0] = proc[j][1];
            item[1] = -1;
            joinSet(empty, item, 1);

            // 递归处理产生式左部
            getNullSet(proc[j][1]);
        }
    }
}
// 判断该非终结符是否能推出空
int Grammer::canNonTer2Null(int currentNon)
{
    int item[2];
    item[0] = currentNon;
    item[1] = -1;

    int result = 1;
    int mark = 0;

    joinSet(emptyRecu, item, 1); // 先将此符号并入防递归集合中
    if (inEmpty(currentNon) == 1)
        return 1;

    for (int j = 1; j <= procNum; j++)
    {
        if (proc[j][1] == currentNon)
        {
            // 求出长度
            int rLen = getProcLength(proc[j]);

            // 如果右侧只有一个非终结符，并且这个非终结符可以推出空
            if (rLen - 2 == 1 && inEmpty(proc[j][rLen]))
                return 1;

            // 如果右侧只有一个终结符
            else if (rLen - 2 == 1 && isTer(proc[j][rLen]))
                return 0;

            // 右侧不只有一个非终结符
            else
            {
                // 检查该产生式右边有没有重复符号（重复就递归爆炸了啊啊啊！！！）
                for (int k = 3; k <= rLen; k++)
                    if (inEmptyRecu(proc[j][k]))
                        mark = 1;
                if (mark == 1)
                    continue;
                else
                {
                    // 没有重复的话，就挨个递归检查是否可以推导出空
                    for (int k = 3; k <= rLen; k++)
                    {
                        result *= canNonTer2Null(proc[j][k]); // 其实是挨个与运算，有一个可以推导出空，整个都可以
                        item[0] = proc[j][k];                 // 经典集合操作，不赘述
                        item[1] = -1;
                        joinSet(emptyRecu, item, 1); // 防止递归
                    }
                }
            }
            // 当前产生式能推导出空，寄！
            if (result == 1)
                return 1;
        }
    }
    return 0;
}
// 将First结合起来的函数
void Grammer::connectFirstSet(int *p)
{
    int i = 0;
    int flag = 0;
    int item[2];
    // 如果P的长度为1
    if (p[1] == -1)
    {
        if (p[0] == GRAMMAR_NULL)
        {
            connectFirst[0] = GRAMMAR_NULL;
            connectFirst[1] = -1;
        }
        else
        {
            for (i = 0; i < nonTerMap.size(); i++)
            {
                if (nonTerMap[i].second == p[0])
                {
                    flag = 1;
                    joinSet(connectFirst, first[i], 1);
                    break;
                }
            }
            // 也可能是终结符
            if (flag == 0)
            {
                for (i = 0; i < terMap.size(); i++)
                {
                    if (terMap[i].second == p[0])
                    {
                        item[0] = terMap[i].second;
                        item[1] = -1;
                        joinSet(connectFirst, item, 2); // 终结符的First集就是其本身
                        break;
                    }
                }
            }
        }
    }
    // 如果p的长度大于1
    else
    {
        for (i = 0; i < nonTerMap.size(); i++)
        {
            if (nonTerMap[i].second == p[0])
            {
                flag = 1;
                joinSet(connectFirst, first[i], 2);
                break;
            }
        }
        // 也可能是终结符
        if (flag == 0)
        {
            for (i = 0; i < terMap.size(); i++)
            {
                if (terMap[i].second == p[0])
                {
                    item[0] = terMap[i].second;
                    item[1] = -1;
                    joinSet(connectFirst, item, 2); // 终结符的First集就是其本身
                    break;
                }
            }
        }
        flag = 0;
        int length = 0;
        for (length = 0;; length++)
            if (p[length] == -1)
                break;

        for (int k = 0; k < length; k++)
        {
            emptyRecu[0] = -1; // 相当于初始化这个防递归集合

            // 如果右部的当前字符能推出空并且还不是最后一个字符，就将之后的一个字符并入First集中
            if (canNonTer2Null(p[k]) == 1 && k < length - 1)
            {
                int u = 0;
                for (u = 0; u < nonTerMap.size(); u++)
                {
                    // 注意是记录下一个符号的位置
                    if (nonTerMap[u].second == p[k + 1])
                    {
                        flag = 1;
                        joinSet(connectFirst, first[u], 2);
                        break;
                    }
                }
                // 也可能是终结符
                if (flag == 0)
                {
                    for (u = 0; u < terMap.size(); u++)
                    {
                        // 注意是记录下一个符号的位置
                        if (terMap[u].second == p[k + 1])
                        {
                            item[0] = terMap[i].second;
                            item[1] = -1;
                            joinSet(connectFirst, item, 2);
                            break;
                        }
                    }
                }
                flag = 0;
            }
            // 到达最后一个字符，并且产生式右部都能推出空,将$并入First集中
            else if (canNonTer2Null(p[k]) == 1 && k == length - 1)
            {
                item[0] = GRAMMAR_NULL;
                item[1] = -1;
                joinSet(connectFirst, item, 1);
            }
            else
                break;
        }
    }
}
// 动态生成非终结符，在基点的基础上，确保不和终结符冲突
int Grammer::dynamicNonTer(char *word)
{
    int i = 0;
    int dynamicNum;
    for (i = 0; i < nonTerMap.size(); i++)
    {
        if (strcmp(word, nonTerMap[i].first) == 0)
            return nonTerMap[i].second;
    }
    if (i == nonTerMap.size())
    {
        if (i == 0)
        {
            dynamicNum = GRAMMAR_BASE;
            nonTerMap.push_back(make_pair(word, dynamicNum));
        }
        else
        {
            dynamicNum = nonTerMap[nonTerMap.size() - 1].second + 1;
            nonTerMap.push_back(make_pair(word, dynamicNum));
        }
    }
    return dynamicNum;
}
// 判断某个标号是不是非终结符的标号,1代表是，0代表否
int Grammer::isNonTer(int n)
{
    for (int i = 0; i < nonTerMap.size(); i++)
        if (nonTerMap[i].second == n)
            return 1;

    return 0;
}
// 判断某个标号是不是终结符的标号，1代表是，0代表否
int Grammer::isTer(int n)
{
    for (int i = 0; i < terMap.size(); i++)
        if (terMap[i].second == n)
            return 1;

    return 0;
}
// 判断某个标号在不在此时的empty集中，1代表是，0代表否
int Grammer::inEmpty(int n)
{
    // 当前Empty集的长度
    int emptyLength = 0;
    for (emptyLength = 0;; emptyLength++)
        if (empty[emptyLength] == -1)
            break;

    for (int i = 0; i < emptyLength; i++)
        if (empty[i] == n)
            return 1;

    return 0;
}
// 判断某个标号在不在此时的emptyRecu集中，1代表是，0代表否
int Grammer::inEmptyRecu(int n)
{
    // 当前Empty集的长度
    int emptyLength = 0;
    for (emptyLength = 0;; emptyLength++)
        if (emptyRecu[emptyLength] == -1)
            break;

    for (int i = 0; i < emptyLength; i++)
        if (emptyRecu[i] == n)
            return 1;

    return 0;
}
// 判断某个标号在不在此时的followRecu集中，1代表是，0代表否
int Grammer::inFollowRecu(int n)
{
    int followLength = 0;
    for (followLength = 0;; followLength++)
        if (followRecu[followLength] == -1)
            break;

    for (int i = 0; i < followLength; i++)
        if (followRecu[i] == n)
            return 1;

    return 0;
}
// 判断某个标号是不是在产生式的右边
int Grammer::inProcRight(int n, const int *proc)
{
    // 直接看右边
    for (int i = 3;; i++)
    {
        if (proc[i] == -1)
            return 0;
        if (proc[i] == n)
            return 1;
    }
}

// 输出文法
void Grammer::printProc()
{
    cout << "=======================================文法=======================================\n";
    // 遍历处理后的文法
    for (int k = 1; k <= procNum; k++)
    {
        for (int j = 1; j < Max_Length; j++)
        {
            if (proc[k][j] != -1)
                cout << searchMap(proc[k][j]) << " ";
            else
                break;
        }
        cout << endl;
    }

    cout << endl;
    for (int k = 1; k <= procNum; k++)
    {
        for (int j = 1; j < Max_Length; j++)
        {
            if (proc[k][j] != -1)
                cout << proc[k][j] << " ";
            else
                break;
        }
        cout << endl;
    }
}
// 输出文法中的终结符
void Grammer::printTer()
{
    cout << "\n\n=====================================文法终结符=====================================\n";
    for (auto &item : terMap)
        cout << item.first << "\t\t\t" << item.second << endl;
}
// 输出文法中的非终结符
void Grammer::printNonTer()
{
    cout << "\n\n====================================文法非终结符====================================\n";

    for (auto &item : nonTerMap)
        cout << item.first << "\t\t\t" << item.second << endl;
    cout << endl;
}
// 输出first集
void Grammer::printFirstSet()
{
    cout << "\n************************************First******************************\n";
    for (int i = 0; i < nonTerMap.size(); i++)
    {
        printf("First[%s] = ", nonTerMap[i].first);
        for (int j = 0;; j++)
        {
            if (first[i][j] == -1)
                break;
            cout << searchMap(first[i][j]) << " ";
        }
        cout << endl;
    }
}
// 输出follow集
void Grammer::printFollowSet()
{
    cout << "\n************************************Follow集******************************\n";

    for (int i = 0; i < nonTerMap.size(); i++)
    {
        printf("Follow[%s] = ", nonTerMap[i].first);
        for (int j = 0;; j++)
        {
            if (follow[i][j] == -1)
                break;

            cout << searchMap(follow[i][j]) << " ";
        }
        cout << endl;
    }
}
// 输出select集
void Grammer::printSelectSet()
{
    cout << "\n************************************Select集******************************\n";

    for (int i = 0; i < procNum; i++)
    {
        cout << "select[" << i + 1 << "] = ";
        for (int j = 0;; j++)
        {
            if (select_[i][j] == -1)
                break;

            cout << searchMap(select_[i][j]) << " ";
        }
        cout << endl;
    }
}
bool isDescribe(const string &token)
{
    if (token == "auto" ||
        token == "const" ||
        token == "unsigned" ||
        token == "signed" ||
        token == "static")
    {
        return true;
    }
    return false;
}
bool isType(const string &token)
{
    if (token == "int" ||
        token == "char" ||
        token == "double" ||
        token == "float" ||
        token == "void")
    {
        return true;
    }
    return false;
}

// ================================= first ================================= //
// 计算一个非终结符的first集(传入的参数是在非终结符集合中的序号)
void Grammer::getSingleFirstSet(int i)
{
    int currentNon = nonTerMap[i].second; // 当前的非终结符编号
    // 依次遍历全部产生式
    for (int j = 1; j <= procNum; j++) // j代表第几个产生式
    {
        // 找到该非终结符的产生式
        if (currentNon == proc[j][1]) // 注意从1开始
        {
            // 当右边的第一个是终结符或者空的时候，直接加入first集
            if (isTer(proc[j][3]) == 1 || proc[j][3] == GRAMMAR_NULL)
            {
                // 并入当前非终结符的first集中
                int item[2];
                item[0] = proc[j][3];
                item[1] = -1; // 集合最后一个元素标记为-1，便于遍历加入
                joinSet(first[i], item, 1);
            }

            // 当右边的第一个是非终结符的时候
            else if (isNonTer(proc[j][3]) == 1)
            {
                // 左递归处理
                if (proc[j][3] == currentNon)
                {
                    cout << "异常！文法存在左递归！！" << endl;
                    exit(1);
                }

                // 记录下右边第一个非终结符的位置
                int k = findNonTerIndex(proc[j][3]);

                // 当右边第一个非终结符还未访问过的时候，递归
                if (firstVisit[k] == 0)
                {
                    getSingleFirstSet(k);
                    firstVisit[k] = 1;
                }

                // 如果first[k]此时有空值的话，暂时不把空值并入first[i]中
                joinSet(first[i], first[k], 2);

                // 获取产生式右侧长度
                int rLength = getProcLength(proc[j]);

                // 循环处理后面的
                for (k = 3; k < rLength; k++)
                {
                    emptyRecu[0] = -1; // 相当于初始化这个防递归集合

                    // 如果右部的当前字符能推出空并且还不是最后一个字符，就将之后的一个字符并入First集中
                    if (canNonTer2Null(proc[j][k]) == 1 && k < rLength - 1)
                    {
                        // 找到下一个符号的下标
                        int u = findNonTerIndex(proc[j][k + 1]);

                        // 如果没被访问过，递归啊啊啊啊
                        if (firstVisit[u] == 0)
                        {
                            getSingleFirstSet(u);
                            firstVisit[u] = 1;
                        }
                        joinSet(first[i], first[u], 2);
                    }

                    // 到达最后一个字符，并且产生式右部都能推出空,将$并入First集中
                    else if (canNonTer2Null(proc[j][k]) == 1 && k == rLength - 1)
                    {
                        int item[2];
                        item[0] = GRAMMAR_NULL;
                        item[1] = -1;
                        joinSet(first[i], item, 1);
                    }
                    else
                        break;
                }
            }
        }
    }
    // 标记该非终结符已近被使用
    firstVisit[i] = 1;
}
// 计算文法中所有非终结符的first集合
void Grammer::getFirstSet()
{
    // 先求出能直接推出空的非终结符集合
    getNullSet(GRAMMAR_NULL);

    // 循环求每一个非终结符的fist集
    for (int i = 0; i < nonTerMap.size(); i++)
        getSingleFirstSet(i);
}

// ================================= follow ================================= //
// 计算一个非终结符的follow集(传入的参数是在非终结符集合中的序号)
void Grammer::getSingleFollowSet(int i)
{
    int currentNon = nonTerMap[i].second; // 当前的非终结符标号
    int result = 1;

    // 将当前标号加入防递归集合中
    int item[2];
    item[0] = currentNon;
    item[1] = -1;
    joinSet(followRecu, item, 1);

    // 如果当前符号就是开始符号,把特殊符号加入其Follow集（标识输入串的开始位置）
    if (proc[1][1] == currentNon)
    {
        item[0] = GRAMMAR_SPECIAL;
        item[1] = -1;
        joinSet(follow[i], item, 1);
    }

    // 开始遍历每一个产生式
    for (int j = 1; j <= procNum; j++)
    {
        // 如果该非终结符在某个产生式的右部存在
        if (inProcRight(currentNon, proc[j]) == 1)
        {
            int flag = 0;
            int k = 0;
            int rLen = 1;
            int leftNum = proc[j][1]; // 产生式的左边
            int kArray[Max_Length2];
            memset(kArray, -1, sizeof(kArray));

            // 找到该终结符在该产生式右侧的位置
            int h = findNonTerIndex(leftNum);

            // 找到所有包含当前非终结符的位置
            for (rLen = 1;; rLen++)
            {
                if (currentNon == proc[j][rLen + 2])
                    kArray[k++] = rLen;
                if (proc[j][rLen + 2] == -1)
                    break;
            }
            rLen--;

            // 遍历kArray
            for (int y = 0;; y++)
            {
                // 遍历完了
                if (kArray[y] == -1)
                    break;

                // 如果该非终结符在右部产生式的最后
                if (kArray[y] == rLen)
                {
                    // 产生式的左部在防递归集合followRecu中，说明已经处理过该非终结符的Follow集
                    // 直接并入
                    if (inFollowRecu(leftNum) == 1)
                    {
                        joinSet(follow[i], follow[h], 1);
                        continue;
                    }
                    // 如果没有访问过，递归
                    if (followVisit[h] == 0)
                    {
                        getSingleFollowSet(h);
                        followVisit[h] = 1;
                    }
                    // 递归回来后，直接并入
                    joinSet(follow[i], follow[h], 1);
                }

                // 如果不在最后
                else
                {
                    result = 1;
                    //  遍历从位置kArray[y]+1 到rLen的符号
                    for (int n = kArray[y] + 1; n <= rLen; n++)
                    {
                        // 重置一下防止递归
                        emptyRecu[0] = -1;
                        // 挨个测试kArray[y]+1往后是否可以推导出空
                        result *= canNonTer2Null(proc[j][n + 2]);
                    }

                    // 至少有一个不能推导出空
                    if (result == 1)
                    {
                        // 如果在左部防递归集合followRecu中，说明已经处理过该非终结符的Follow集，直接并入
                        if (inFollowRecu(leftNum) == 1)
                        {
                            joinSet(follow[i], follow[h], 1);
                            continue;
                        }
                        // 如果不在，说明还没求过，递归去求！！！
                        if (followVisit[h] == 0)
                        {
                            getSingleFollowSet(h);
                            followVisit[h] = 1;
                        }
                        // 求完了放进去
                        joinSet(follow[i], follow[h], 1);
                    }

                    // 如果都可以推导出空
                    int item[Max_Length];
                    memset(item, -1, sizeof(item));
                    for (int n = kArray[y] + 1; n <= rLen; n++)
                        item[n - kArray[y] - 1] = proc[j][n + 2];

                    // 数组尾部设置-1表示结尾
                    item[rLen - kArray[y]] = -1;
                    connectFirst[0] = -1; // 重新初始化
                    connectFirstSet(item);
                    joinSet(follow[i], connectFirst, 2);
                }
            }
        }
    }
    followVisit[i] = 1;
}
// 计算文法中所有非终结符的follow集合
void Grammer::getFollowSet()
{
    for (int i = 0; i < nonTerMap.size(); i++)
    {
        followRecu[0] = -1;
        getSingleFollowSet(i);
    }
}

// ================================= select ================================= //
// 求Select集,注意Select集中不能含有空($),因而Type=2
void Grammer::getSelectSet()
{
    for (int i = 1; i <= procNum; i++) // i代表第几个产生式
    {
        int leftNum = proc[i][1]; // 产生式的左边
        int h = 0;
        int result = 1;
        for (h = 0; h < nonTerMap.size(); h++)
            if (nonTerMap[h].second == leftNum)
                break;

        int rightLength = 1;
        for (rightLength = 1;; rightLength++)
            if (proc[i][rightLength + 2] == -1)
                break;

        rightLength--;

        // 如果右部推出式的长度为1并且是空,select[i-1] = follow[左边]
        if (rightLength == 1 && proc[i][rightLength + 2] == GRAMMAR_NULL)
            joinSet(select_[i - 1], follow[h], 2);

        // 如果右部不是空的时候,select[i-1] = first[右部全部]
        // 如果右部能够推出空，select[i-1] = first[右部全部] ^ follow[左边]
        else
        {
            int temp2[Max_Length];
            int n = 0;
            memset(temp2, -1, sizeof(temp2));
            for (n = 1; n <= rightLength; n++)
                temp2[n - 1] = proc[i][n + 2];

            temp2[rightLength] = -1;
            connectFirst[0] = -1; // 应该重新初始化一下
            connectFirstSet(temp2);
            joinSet(select_[i - 1], connectFirst, 2);

            for (n = 1; n <= rightLength; n++)
            {
                emptyRecu[0] = -1;
                result *= canNonTer2Null(proc[i][n + 2]);
            }

            // 如果右部能推出空，将follow[左边]并入select[i-1]中
            if (result == 1)
                joinSet(select_[i - 1], follow[h], 2);
        }
    }
}

// ================================= 构造分析表 ================================= //
// 输出预测分析表
void Grammer::getTable()
{
    // 打开文件用于写入预测分析表
    fstream outfile;
    outfile.open("./LL1分析表.gr", ios::out);

    // 遍历所有产生式
    for (int i = 0; i < procNum; i++)
    {
        int m = 0; // 非终结符的序号

        // 查找产生式左部对应的非终结符在 nonTerMap 中的位置
        for (int t = 0; t < nonTerMap.size(); t++)
        {
            if (nonTerMap[t].second == proc[i + 1][1])
            {
                m = t;
                break;
            }
        }

        // 遍历该产生式的所有 SELECT 集合
        for (int j = 0;; j++)
        {
            if (select_[i][j] == -1)
                break;

            // 查找终结符在 terMap 中的位置
            for (int k = 0; k < terMap.size(); k++)
            {
                if (terMap[k].second == select_[i][j])
                {
                    int n = 0;

                    // 将产生式右部写入预测分析表
                    for (n = 1; n <= Max_Length2; n++)
                    {
                        analytable[m][k][n - 1] = proc[i + 1][n];

                        if (proc[i + 1][n] == -1)
                            break;
                    }
                    break;
                }
            }
        }
    }

    // 写入预测分析表到文件
    outfile << endl
            << "*********************************预测分析表******************************" << endl;
    for (int i = 0; i < nonTerMap.size(); i++)
    {
        for (int j = 0; j < terMap.size(); j++)
        {
            outfile << "analytable[" << nonTerMap[i].first << "][" << terMap[j].first << "] = ";

            // 输出预测分析表中的产生式右部
            for (int k = 0;; k++)
            {
                if (analytable[i][j][k] == -1)
                    break;
                outfile << searchMap(analytable[i][j][k]);
            }
            outfile << endl;
        }
        outfile << endl
                << endl;
    }

    // 关闭文件
    outfile.close();
}

// ================================= 分析源程序 ================================= //
void initStack(SeqStack *S) /*初始化顺序栈*/
{
    S->top = -1;
}
int push(SeqStack *S, const elemType &e) /*进栈*/
{
    if (S->top == Stack_Size - 1)
        return 0;
    S->top++;
    S->elem[S->top] = e;
    return 1;
}
int pop(SeqStack *S) /*出栈*/
{
    if (S->top == -1)
        return 0;
    else
    {
        S->top--;
        return 1;
    }
}
int getTop(SeqStack *S, elemType *e) /*取栈顶元素*/
{
    if (S->top == -1)
        return 0;
    else
    {
        *e = S->elem[S->top];
        return 1;
    }
}
int getLen(elemType *array)
{
    for (int len = 0;; len++)
        if (array[len].type == -1)
            return len;
}
void Grammer::showStack(SeqStack *S) /*显示栈的字符，先输出栈底元素*/
{
    for (int i = S->top; i >= 0; i--)
        analyResult << searchMap(S->elem[i].type) << " ";
}

// 进行语法分析
void Grammer::runAnalysis()
{
    // 打开结果文件
    analyResult.open("./语法分析结果.gr", ios::out);

    SeqStack s1; // 符号栈
    SeqStack s2; // 输入栈
    int i = 0;
    elemType e;
    elemType prevE;

    elemType reserve[Stack_Size]; // 辅助反向入栈变量
    memset(reserve, -1, sizeof(reserve));

    Node *p = head; // 遍历链表指针

    // 初始化两个栈
    initStack(&s1);
    initStack(&s2);
    // # 和 第一条产生式入栈
    e.type = GRAMMAR_SPECIAL;
    push(&s1, e);
    e.type = proc[1][1];
    push(&s1, e);
    e.type = GRAMMAR_SPECIAL;
    push(&s2, e);

    // 读取 lex 的 token 流到 reserve
    while (p != NULL)
    {
        // 使用 isDescribe 和 isType 函数判断 token 的类型
        if (isDescribe(p->lexeme))
        {
            e.type = DESCRIBE;
            e.token = (char *)malloc(strlen(p->lexeme.c_str()) + 1);
            strcpy(e.token, p->lexeme.c_str());
            e.line = p->line;
            e.col = p->column;
            reserve[i++] = e;
        }
        else if (isType(p->lexeme))
        {
            e.type = TYPE;
            e.token = (char *)malloc(strlen(p->lexeme.c_str()) + 1);
            strcpy(e.token, p->lexeme.c_str());
            e.line = p->line;
            e.col = p->column;
            reserve[i++] = e;
        }
        else
        {
            e.type = p->type;
            e.token = (char *)malloc(strlen(p->lexeme.c_str()) + 1);
            strcpy(e.token, p->lexeme.c_str());
            e.line = p->line;
            e.col = p->column;
            reserve[i++] = e;
        }
        p = p->next;
    }

    // 反向入栈
    for (i = getLen(reserve); i > 0; i--)
        push(&s2, reserve[i - 1]);

    // 开始分析
    elemType e1, e2;

    while (true)
    {
        int flag = 0;
        int h1, h2;
        analyResult << "符号栈:";
        showStack(&s1);
        analyResult << endl;
        analyResult << "输入栈:";
        showStack(&s2);
        analyResult << endl;

        // 取栈顶元素
        prevE = e2;
        getTop(&s1, &e1);
        getTop(&s2, &e2);

        // 当符号栈和输入栈都剩余#，无错误
        if (e1.type == GRAMMAR_SPECIAL && e2.type == GRAMMAR_SPECIAL)
        {
            analyResult << "成功!" << endl;
            break;
        }

        if (e1.type == GRAMMAR_SPECIAL && e2.type != GRAMMAR_SPECIAL)
        {
            analyResult << "失败!" << endl;
            break;
        }

        // 符号栈的栈顶元素和输入串的栈顶元素相同时，同时弹出
        if (e1.type == e2.type)
        {
            pop(&s1);
            pop(&s2);
            flag = 1;
        }
        // 查表分析
        else
        {
            // 记录下非终结符的位置
            h1 = findNonTerIndex(e1.type);
            // 记录下终结符的位置
            h2 = findTerIndex(e2.type);

            if (analytable[h1][h2][0] == -1)
            {
                analyResult << "\n\n============ 出现错误！============\n";
                analyResult << "===== 错误出现在下列两个token间 =====\n";
                analyResult << setw(10) << left << "token" << setw(10) << "line" << setw(10) << "colunm" << endl;
                analyResult << setw(10) << left << prevE.token << setw(10) << prevE.line << setw(10) << prevE.col << endl;
                analyResult << setw(10) << left << e2.token << setw(10) << e2.line << setw(10) << e2.col << endl;

                cout << "\n\n============ 出现错误！============\n";
                cout << "===== 错误出现在下列两个token间 =====\n";
                cout << setw(10) << left << "token" << setw(10) << "line" << setw(10) << "colunm" << endl;
                cout << setw(10) << left << prevE.token << setw(10) << prevE.line << setw(10) << prevE.col << endl;
                cout << setw(10) << left << e2.token << setw(10) << e2.line << setw(10) << e2.col << endl;
                break;
            }
            else
            {
                // 计算推导式的长度
                int len = getProcLength(analytable[h1][h2]);

                pop(&s1);
                // 如果不是空的话，反向入栈
                if (analytable[h1][h2][2] != GRAMMAR_NULL)
                {
                    for (int k = len - 1; k >= 2; k--)
                    {
                        e.type = analytable[h1][h2][k];
                        e.line = e.col = -1;
                        push(&s1, e);
                    }
                }
            }
        }

        if (flag == 1)
            analyResult << "匹配!" << endl
                        << endl;
        else
        {
            analyResult << "推出式:";
            int w = 0;
            // 记录下推导式的长度
            for (w = 0; analytable[h1][h2][w] != -1; w++)
                analyResult << searchMap(analytable[h1][h2][w]);
            analyResult << endl
                        << endl;
        }
    }

    // 关闭结果文件
    analyResult.close();
}