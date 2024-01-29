%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_MAP_SIZE 100

struct tokenMap
{
    char* token; // 内容
    int num;     // 种别码
}mapArray[MAX_MAP_SIZE];

void initMap(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL)
    {
        fprintf(stderr, "错误：无法打开文件\n");
        return;
    }

    int index = 0;
    char buffer[256]; // 假设每行最大长度为256，你可以根据实际情况调整

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char* token = strtok(buffer, " ");
        char* numStr = strtok(NULL, " ");

        if (token != NULL && numStr != NULL)
        {
            mapArray[index].token = strdup(token);
            mapArray[index].num = atoi(numStr);
            ++index;
        }
    }

    fclose(file);
}

int searchMapping(const char* input)
{
    for (int i = 0; i < MAX_MAP_SIZE && mapArray[i].token != NULL; ++i)
        if (strcmp(mapArray[i].token, input) == 0)
            return mapArray[i].num;
    return -1;
}


const int KEY_NUM = 23;
const char* KEY_SET[] = {
  "int", "char", "if", "else", "for",
  "while", "return", "void", "break", "case",
  "const", "continue", "default", "do", "double",
  "float", "goto", "register", "static", "struct",
  "switch", "typedef", "unsigned"
};

int isDigit(char ch)
{
	if(ch <= '9' && ch >= '0')
		return 1;
	return 0;
}

int isLetter(char ch)
{
	if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	return 0;
}

int getKeyId(char *str)
{
	for(int i=0; i<KEY_NUM; i++)
	{
		if(strcmp(KEY_SET[i], str) == 0)
			return i+1;
	}
	return 0;
}

int isNoOne(char ch)
{
	if(ch != '\'' && ch != '\0')
		return 1;
	return 0;
}

int isNoTow(char ch)
{
	if(ch != '\"' && ch != '\0')
		return 1;
	return 0;
}

%}

%@
noone=isNoOne
notow=isNoTow
letter=isLetter
digit=isDigit
%@


%%
({letter}|_)({letter}|_|{digit})* {
	int id = getKeyId(token);
	if(id != 0)
		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));
	else
		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));
}
%$
(-|%$){digit}{digit}*(.{digit}{digit}*|%$)((E|e){digit}{digit}*|%$) {
	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("digit"),token,line,pos-strlen(token),strlen(token));
}
%$
%(|%)|%{|%}|[|]|;|,|. {
	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));
}
%$
% |{\t}|{\n} {

}
%$
(%*(%=|%$))|(/(=|%$))|(+(+|=|%$))|(-(-|=|%$))|(<(<|=|%$))|(>(>|=|%$))|(=(=|%$))|(&(&|=|%$))|(%|(%||=|%$))|(^(=|%$))|(~(=|%$)) {
	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));
}
%$
({\'}{noone}*{\'})|({\"}{notow}*{\"}) {
	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("string"),token,line,pos-strlen(token),strlen(token));
}
%%