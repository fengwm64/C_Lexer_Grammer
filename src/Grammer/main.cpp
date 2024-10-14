#include <iomanip>
#include "Grammer.h"

using namespace std;

int main()
{
    Grammer G = Grammer("./Grammer/wenfa");

    // 获取词法分析部分输入
    G.scanner("./token");

    // 语法分析部分
    // G.printProc();
    // G.printTer();
    // G.printNonTer();

    // 计算first集
    G.getFirstSet();
    // G.printFirstSet();

    // 计算follow集
    G.getFollowSet();
    // G.printFollowSet();

    // 计算select集
    G.getSelectSet();
    // G.printSelectSet();

    // 构造分析表
    G.getTable();
    
    // 对token流进行语法分析
    G.runAnalysis();

    return 0;
}
