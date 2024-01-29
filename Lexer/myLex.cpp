#include "myLex.h"

// 构造函数
myLex::myLex(std::string inLexFile, std::string outCodeFile)
{
    // 词法分析规则文件
    myLex::lexFile = inLexFile;
    // 生成的词法分析器输出文件
    myLex::outCFile = outCodeFile;

    // 输入输出重定向
    // 打开输入的规则文件
    if (!freopen(lexFile.c_str(), "r", stdin))
    {
        perror("打开lex规则文件失败");
        exit(1);
    }
    // 打开输出文件
    FILE *outputFile = freopen(outCodeFile.c_str(), "w+", stdout);

    // 初始化数组
    const int UNINITIALIZED = -1;
    memset(dfaEnd, UNINITIALIZED, sizeof(dfaEnd));
    memset(nfaEnd, UNINITIALIZED, sizeof(nfaEnd));
}

// 错误输出部分
// ======================================================================================= //
// 打印错误提示
void myLex::printErr(int line, const string &str, const string &mess)
{
    cerr << "\033[1;31m错误: \033[0m" << mess << " :" << line << " --->\"" << str << "\"" << endl;
    exit(1);
}

// 打印警告提示
void myLex::printWarn(int line, const string &str, const string &mess)
{
    cerr << "\033[1;33m警告: \033[0m" << mess << ":" << line << " --->\"" << str << "\"" << endl;
}
// ======================================================================================= //

// 解析lex规则文件部分
// ======================================================================================= //
// 解析定义区块
void myLex::getFunc(string str, int line)
{
    string left, right;                  // 等号左右字符
    bool flag = false;                   // 是否出现警告
    string::iterator iter = str.begin(); // 字符串迭代器

    // Lambda函数，用于提取等号左右两侧的内容
    auto processSide = [&](string::iterator &iter, string &side)
    {
        while (iter != str.end() && *iter != '=')
        {
            // 获取当前字符
            char ch = *iter;

            // 判断当前字符是否为字母、数字或下划线
            if (isalpha(ch) || isdigit(ch) || ch == '_')
                side += *iter; // 加入到提取的字符串中
            else
                flag = true; // flag设为true，表示存在非法字符
            // 字符迭代器后移
            iter++;
        }
    };

    // 提取等号左侧
    processSide(iter, left);

    // 略过等号
    iter++;

    // 提取等号右侧
    processSide(iter, right);

    // 异常处理
    if (left.empty() || right.empty())
        printErr(line, str, "不合规范");

    // 输出警告
    if (flag)
    {
        char mess[100];
        sprintf(mess, "定义区块错误！存在非法字符，请确认 %s = %s", left.c_str(), right.c_str());
        printWarn(line, str, mess);
    }

    // 存储到funcMap
    funcMap.insert({left, right});
}

// 解析规则定义部分（解析正规表达式）
void myLex::getRegular(string str, int line)
{
    // 获取字符串的起始迭代器
    string::iterator iter = str.begin();

    // 用于存储规则左部分的向量，每个元素是一个 pair，包含字符串和一个布尔值
    // 布尔值表示字符串是否代表一个符号（true 代表符号，false 代表输入）
    vector<pair<string, bool>> left;

    // 用于存储规则右部分的字符串
    string right;

    // 跳过字符串开头的空格和制表符
    while (iter != str.end() && (*iter == ' ' || *iter == '\t'))
        iter++;

    // 如果字符串遍历完仍未找到规则内容，则输出错误信息并终止程序
    if (iter == str.end())
        printErr(line, str, "规则缺失");

    // [ ][%,(,),{,},|,*,$]
    // 遍历字符串中的字符，解析规则左部分
    while (iter != str.end() && *iter != ' ' && *iter != '\t')
    {
        // 用于存储当前解析的字符或字符串
        string t;

        // 如果当前字符为 '%'，表示转义输入
        if (*iter == '%' && iter + 1 != str.end())
        {
            iter++;

            // 根据下一个字符的不同情况进行处理
            switch (*iter)
            {
            case ' ':
            case '%':
            case '(':
            case ')':
            case '{':
            case '}':
            case '|':
            case '*':
                // 将转义字符添加到当前解析的字符串中
                t += *iter;
                // 将解析的结果存储到 left 向量中，表示需要转义的输入
                left.push_back(pair<string, bool>(t, false));
                // 将当前解析的字符串添加到 inputSet 中，用于记录输入的集合
                inputSet.insert(t);
                break;
            case '$':
                // 对于 '$'，表示空字符输入，将空字符串存储到 left 向量中
                left.push_back(pair<string, bool>("", false));
                break;
            default:
                // 对于其他情况，不进行特殊处理
                break;
            }
        }
        // 如果当前字符为 '{'，表示可能是自定义输入
        else if (*iter == '{')
        {
            iter++;

            // 继续遍历字符，直到找到 '}'
            while (iter != str.end() && *iter != '}')
                t += *iter++;

            // 如果未找到匹配的 '}'，则输出错误信息并终止程序
            if (iter == str.end())
                printErr(line, str, "规则正规式格式错误，未找到右括号");

            // 将解析的结果存储到 left 向量中，表示自定义输入
            left.push_back(pair<string, bool>(t, false));
            // 将当前解析的字符串添加到 inputSet 中
            inputSet.insert(t);
        }
        else
        {
            // 对于其他字符的处理
            switch (*iter)
            {
            case '(':
            case ')':
            case '*':
            case '|':
                // 将符号加两次，与同样输入相区分
                t += *iter;
                t += *iter;
                // 将解析的结果存储到 left 向量中，表示符号
                left.push_back(pair<string, bool>(t, true));
                break;
            default:
                // 对于其他字符，将当前字符加入解析的字符串中
                t += *iter;
                // 将解析的结果存储到 left 向量中，表示输入
                left.push_back(pair<string, bool>(t, false));
                // 将当前解析的字符串添加到 inputSet 中
                inputSet.insert(t);
                break;
            }
        }

        // 移动迭代器到下一个字符
        iter++;
    }

    // 跳过左部分与右部分之间的空格和制表符
    while (iter != str.end() && *iter != ' ' && *iter != '\t')
        iter++;

    // 如果字符串遍历完仍未找到规则方法体，则输出错误信息并终止程序
    if (iter == str.end())
        printErr(line, str, "规则方法体缺失");
    iter++;

    // 检查规则方法体的开头是否为 '{'
    if (*iter != '{')
        printErr(line, str, "规则方法体格式错误");

    // 将 '{' 加入右部分字符串
    right += *iter++;
    int cnt = 1;

    // 继续遍历字符，直到找到与 '{' 匹配的 '}'
    while (cnt != 0 && iter != str.end())
    {
        if (*iter == '{')
            cnt++;
        else if (*iter == '}')
            cnt--;
        right += *iter++;
    }

    // 如果未找到匹配的 '}'，则输出错误信息并终止程序
    if (cnt != 0)
        printErr(line, str, "规则方法体格式错误");

    // 检查右部分后是否还有多余的字符
    while (iter != str.end())
    {
        if (*iter != ' ')
        {
            char mess[100];
            sprintf(mess, "规则多余字符已忽略，请确认 %s", string(str.begin(), iter).c_str());
            printWarn(line, str, mess);
            break;
        }
        iter++;
    }

    // 保存规则左部分
    regrVet.push_back(left);

    // 保存规则方法体
    funcVet.push_back(right);
}

// 读取lex规则
void myLex::scanner()
{
    int stateId = 0; // 状态
    int line = 0;    // 行号
    string str;      // 目前位于的字符
    string outStr;   // 输出字符串

    /* *
     * 状态说明：
     *   - stateId = 0: 识别不同标志符号的开始
     *     %{         // 匹配以 "%{" 开头的字符串
     *     %@         // 匹配以 "%@" 开头的字符串
     *     %%         // 匹配以 "%%" 开头的字符串
     *
     *   - stateId = 1: 识别标志符 %} 的结束，或声明区块
     *
     *   - stateId = 2: 识别标志符 %@ 的结束，或定义区块
     *
     *   - stateId = 3: 规则区块处理
     * */
    // 不断读入每一行
    while (!cin.eof())
    {
        line++;
        getline(cin, str);
        switch (stateId)
        {
        case 0:
            // %{ 声明区块开始
            if (str.compare(0, 2, "%{") == 0)
            {
                stateId = 1;
                cout << "//%{ start" << endl;
            }
            // %@ 定义区块开始
            else if (str.compare(0, 2, "%@") == 0)
            {
                stateId = 2;
                cout << "//%@ start" << endl;
            }
            // %% 规则区块开始
            else if (str.compare(0, 2, "%%") == 0)
            {
                stateId = 3;
                outStr.clear();
                cout << "//%% start" << endl;
            }
            // \n
            else if (str.compare(0, 2, "") == 0)
                stateId = 0;
            // 非法符号
            else
            {
                stateId = 0;
                printWarn(line, str, "非法标志符号，忽略该行!");
            }
            break;
        case 1:
            if (str.compare(0, 2, "%}") == 0)
            {
                stateId = 0;
                cout << "//%} end" << endl;
            }
            else
                cout << str << endl;
            break;
        case 2:
            if (str.compare(0, 2, "%@") == 0)
            {
                stateId = 0;
                cout << "//%@ end" << endl;
            }
            else
                getFunc(str, line);
            break;
        case 3:
            if (str.compare(0, 2, "%%") == 0)
            {
                stateId = 0;
                getRegular(outStr, line);
                cout << "//%% end" << endl;
            }
            else if (str.compare(0, 2, "%$") == 0)
            {
                getRegular(outStr, line);
                outStr.clear();
            }
            else
                outStr += str;
            break;

        default:
            printErr(line, str, "结构不完整");
            break;
        }
    }
    if (stateId != 0)
        printErr(line, str, "结构不完整");
}
// ======================================================================================= //

// NFA部分
// ======================================================================================= //
// 添加NFA节点
void myLex::addNFANode(int fromId, string input, int toId)
{
    nfaVet[fromId].push_back(Node(fromId, input, toId));
}

// 正规式转NFA
void myLex::regrToNFA(vector<pair<string, bool>> regr, int startId, int endId, int &itemId)
{
    // 用栈保存状态、字符和字符串
    stack<int> s_start;
    stack<int> s_end;
    stack<string> s_str;

    // 初始化栈，将起始状态和终止状态入栈
    s_start.push(startId);
    s_end.push(endId);

    // 遍历正规表达式的向量
    for (auto iter = regr.begin(); iter != regr.end(); iter++)
    {
        // 获取符号
        string str = (*iter).first;
        // 获取是否为符号的标志
        bool isSign = (*iter).second;

        // 如果是符号
        if (isSign)
        {
            // 处理左括号
            if (str.compare("((") == 0)
            {
                s_end.push(itemId++);
                s_str.push("((");
            }
            // 处理右括号
            else if (str.compare("))") == 0)
            {
                int st = s_start.top();
                int ed = s_end.top();
                string s = s_str.top();

                // 将当前最后状态与终态空连接
                addNFANode(st, "", ed);

                // 将括号内的状态一一退出并连接
                while (!s_str.empty())
                {
                    s = s_str.top();
                    s_str.pop();
                    if (s.compare("((") == 0)
                        break;
                    int nxt = s_start.top();
                    s_start.pop();
                    int pre = s_start.top();
                    if (s.compare("##") != 0)
                        addNFANode(pre, s, nxt);
                }

                // 将括号当做一个输入#,使其与普通输入同样规则，解决括号递归问题
                s_str.push("##");
                s_start.push(s_end.top());
                s_end.pop();
            }
            else if (str.compare("||") == 0)
            {
                // 处理竖线，表示或
                int ed = s_end.top();
                int st = s_start.top();
                string s;

                // 增加当前最后状态到终态的连接
                addNFANode(st, "", ed);

                // 将括号至|内的状态一一退出并连接
                while (!s_str.empty())
                {
                    s = s_str.top();
                    if (s.compare("((") == 0)
                        break;
                    s_str.pop();
                    int nxt = s_start.top();
                    s_start.pop();
                    int pre = s_start.top();
                    if (s.compare("##") != 0)
                        addNFANode(pre, s, nxt);
                }
            }
            else if (str.compare("**") == 0)
            {
                // 处理星号，表示零或多次重复
                int nxt = s_start.top();
                s_start.pop();
                int pre = s_start.top();
                addNFANode(pre, "", nxt);
                addNFANode(nxt, "", pre);
                s_start.push(nxt);
            }
            // 非法符号，输出错误信息
            else
                cerr << "ERROR::regrToNFA" << endl;
        }
        else
        {
            // 非符号，入栈
            s_str.push(str);
            s_start.push(itemId++);
        }
    }

    // 将剩余的状态连接成最终的NFA
    addNFANode(s_start.top(), "", s_end.top());

    while (!s_str.empty())
    {
        int nxt = s_start.top();
        s_start.pop();
        int pre = s_start.top();
        string s = s_str.top();
        s_str.pop();
        if (s.compare("##") != 0)
            addNFANode(pre, s, nxt);
    }
}

// 生成NFA
void myLex::getNFA()
{
    // 初始状态和终止状态的ID
    int startId = 0;
    int endId = 1;

    // 当前处理的状态ID，用于生成新的状态
    int itemId = 2;

    // 记录NFA的编号，用于区分不同的正规表达式生成的NFA
    int num = 0;

    // 遍历所有正规表达式规则
    for (auto iter = regrVet.begin(); iter != regrVet.end(); iter++)
    {
        // 为每个规则生成起始和终止状态
        int iStartId = itemId++;
        int iEndId = itemId++;

        // 将起始状态与初始状态连接
        addNFANode(startId, "", iStartId);

        // 将终止状态与当前终止状态连接
        addNFANode(iEndId, "", endId);

        // 根据规则生成NFA
        regrToNFA(*iter, iStartId, iEndId, itemId);

        // 记录当前终止状态的NFA编号
        nfaEnd[iEndId] = num++;
    }

    // 记录生成的NFA的状态数量
    this->nfaNum = itemId;
}

// NFA查询
void myLex::findBeGo(int i, const string &input, vector<int> *stateId)
{
    for (auto x : nfaVet[i])
    {
        int sId = x.toId;
        bool flag = true;
        for (auto iter = stateId->begin(); iter != stateId->end(); iter++)
        {
            if ((*iter) == sId)
            {
                flag = false;
                break;
            }
        }
        if (flag && input.compare(x.input) == 0)
        {
            stateId->push_back(sId);
            findBeGo(sId, "", stateId);
        }
    }
}

// 获取NFA目标状态集合
vector<int> *myLex::getState(vector<int> *fromstateId, const string &input)
{
    vector<int> *stateId = new vector<int>;

    for (auto iter = fromstateId->begin(); iter != fromstateId->end(); iter++)
    {
        int num = (*iter);
        findBeGo(num, input, stateId);
    }
    sort(stateId->begin(), stateId->end());
    return stateId;
}
// ======================================================================================= //

// DFA部分
// ======================================================================================= //
// 添加DFA节点
void myLex::addDFANode(int fromId, string input, int toId)
{
    dfaVet[fromId].push_back(Node(fromId, input, toId));
}

// 生成DFA
void myLex::getDFA()
{
    // 对状态集合进行查重
    map<vector<int>, int> stateIdMap;           // 使用map存储状态集合和对应的标识符
    queue<pair<vector<int> *, int>> stateIdQue; // 使用队列进行广度优先搜索
    int id = 0;                                 // DFA状态标识符

    // 创建初始状态集合
    vector<int> *t = new vector<int>;
    t->push_back(0);
    vector<int> *startstateId = getState(t, ""); // 获取初始状态集合
    delete t;                                      // 释放临时向量的内存

    startstateId->push_back(0);                       // 添加额外状态（0）到初始状态集合
    sort(startstateId->begin(), startstateId->end()); // 对状态集合进行排序，确保唯一性

    stateIdMap.insert(pair<vector<int>, int>(*startstateId, id)); // 插入初始状态集合到映射表
    stateIdQue.push(pair<vector<int> *, int>(startstateId, id));  // 将初始状态集合加入队列
    id++;                                                         // 增加状态标识符

    // 广度优先搜索
    while (!stateIdQue.empty())
    {
        auto stateId = stateIdQue.front();
        stateIdQue.pop();

        // 遍历输入符号集合
        for (string x : inputSet)
        {
            // 获取新的状态集合
            vector<int> *newstateId = getState(stateId.first, x);

            // 如果新状态集合为空，继续下一轮循环
            if (newstateId->empty())
                continue;

            // 查找新状态集合是否已经存在于映射表中
            auto rel = stateIdMap.find(*newstateId);

            if (rel == stateIdMap.end())
            {
                // 如果新状态集合不存在，插入到映射表和队列中，同时添加DFA节点
                stateIdMap.insert(pair<vector<int>, int>(*newstateId, id));
                stateIdQue.push(pair<vector<int> *, int>(newstateId, id));
                addDFANode(stateId.second, x, id);
                id++;
            }
            else
            {
                // 如果新状态集合已经存在，直接添加DFA节点并连接到已有状态
                addDFANode(stateId.second, x, rel->second);
            }
        }
    }

    // 记录DFA状态总数
    this->dfaNum = id;
    // 获取DFA终态集合
    getVt(stateIdMap);
}

// 获取终态
void myLex::getVt(const map<vector<int>, int> &stateIdMap)
{

    for (auto x : stateIdMap)
    {
        for (auto i = x.first.begin(); i != x.first.end(); i++)
        {
            int num = (*i);
            if (nfaEnd[num] != -1)
            {
                dfaEnd[x.second] = nfaEnd[num];
                break;
            }
        }
    }
}
// ======================================================================================= //

// 生成目标词法分析器代码部分
// ======================================================================================= //
// 生成词法分析器代码的头部
void myLex::generate()
{
    cout << "#define MAXSIZE_token 120" << endl;
    cout << "#define MAXSIZE_BUFF 1024" << endl;
    cout << endl;
    cout << "char FILE_NAME[100];" << endl;
    cout << "char OUT_FILE_NAME[100];" << endl;
    cout << "int line = 0;" << endl;
    cout << "int stateId = 0;" << endl;
    cout << "int len = 0;" << endl;
    cout << "FILE *outputFile;" << endl;

    cout << "char token[MAXSIZE_token];" << endl;
    cout << "char buff[MAXSIZE_BUFF];" << endl;
    cout << endl;
    cout << "//扫描函数" << endl;
    cout << "void scanner(char *str, int line)" << endl;
    cout << "{" << endl;
    cout << "    int pos = 0;" << endl;
    cout << "    char ch = ' ';" << endl;
    cout << "    while(ch != '\\0')" << endl;
    cout << "    {" << endl;
    cout << "        switch(stateId) {" << endl;

    for (int i = 0; i < dfaNum; i++)
    {
        cout << "        case " << i << ":" << endl;
        cout << "        {" << endl;
        cout << "            ch = *str++;" << endl;
        cout << "            pos++;" << endl;
        cout << "            token[len++]=ch;" << endl;

        for (const auto &x : dfaVet[i])
        {
            cout << "            if(";
            auto func = funcMap.find(x.input);
            // 处理基本字符（如' " / +这些）
            if (func == funcMap.end())
            {
                cout << "ch == '";
                cout << x.input;
                cout << "'";
            }
            // 处理自定义的词（如letter digit等）
            else
            {
                cout << func->second;
                cout << "(ch)";
            }
            cout << ")" << endl;
            cout << "                stateId = " << x.toId << ";" << endl;
            cout << "            else" << endl;
        }
        cout << "            {" << endl;

        // dfa还没有遍历完
        if (dfaEnd[i] != -1)
        {
            cout << "                token[len-1] = '\\0';" << endl;
            cout << "                len=0;" << endl;
            cout << "                stateId=0;" << endl;
            cout << "                str--;" << endl;
            cout << "                pos--;" << endl;
            cout << funcVet[dfaEnd[i]] << endl;
        }
        // dfa遍历完还在转移，就是失败了
        else
        {
            cout << R"(printf("Error in line %d\n", line);)" << endl;
            cout << "exit(1);" << endl;
        }
        cout << "            }" << endl;
        cout << "            break;" << endl;
        cout << "        }" << endl;
    }

    cout << "        }" << endl;
    cout << "    }" << endl;
    cout << "}" << endl;
    cout << endl;
    cout << "int main(int argc, char **args)" << endl;
    cout << "{" << endl;
    cout << "    if(argc == 1)" << endl;
    cout << "    {" << endl;
    cout << "        printf(\"没有输入源文件名\");" << endl;
    cout << "        return 0;" << endl;
    cout << "    }" << endl;
    cout << "    else if(argc == 2)" << endl;
    cout << "    {" << endl;
    cout << "        strcpy(FILE_NAME, args[1]);" << endl;
    cout << "        sprintf(OUT_FILE_NAME, \"%s.out\", FILE_NAME);" << endl;
    cout << "    }" << endl;
    cout << "    else" << endl;
    cout << "    {" << endl;
    cout << "        strcpy(FILE_NAME, args[1]);" << endl;
    cout << "        strcpy(OUT_FILE_NAME, args[2]);" << endl;
    cout << "    }" << endl;
    cout << "    FILE* file = fopen(FILE_NAME, \"r\");" << endl;
    cout << R"(    initMap("/home/fwm/project/C_Lexer_Grammer/Lexer/mapping");)" << endl;
    //    cout << R"(    printf("种别码\t单词    \t行号\t列号\t长度\n");)" << endl;
    cout << R"( outputFile = fopen("./token", "w");)" << endl;
    cout << "    while(NULL != fgets(buff, MAXSIZE_BUFF, file))" << endl;
    cout << "    {" << endl;
    cout << "        ++line;" << endl;
    cout << "        scanner(buff, line);" << endl;
    cout << "    }" << endl;
    cout << "    return 0;" << endl;
    cout << "}" << endl;
}
// ======================================================================================= //

// 类运行主函数
void myLex::run()
{
    cerr << "1.Lex规则文本解析" << endl;
    scanner();

    cerr << "2.正规式转NFA" << endl;
    getNFA();

    cerr << "3.NFA转DFA" << endl;
    getDFA();

    cerr << "4.生成词法分析器代码" << endl;
    generate();
    
    cerr << "已生成词法分析器" << myLex::outCFile << endl;
}