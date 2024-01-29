//%{ start
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

//%} end
//%@ start
//%@ end
//%% start
//%% end
#define MAXSIZE_token 120
#define MAXSIZE_BUFF 1024

char FILE_NAME[100];
char OUT_FILE_NAME[100];
int line = 0;
int stateId = 0;
int len = 0;
FILE *outputFile;
char token[MAXSIZE_token];
char buff[MAXSIZE_BUFF];

//扫描函数
void scanner(char *str, int line)
{
    int pos = 0;
    char ch = ' ';
    while(ch != '\0')
    {
        switch(stateId) {
        case 0:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == ' ')
                stateId = 1;
            else
            if(ch == '&')
                stateId = 2;
            else
            if(ch == '(')
                stateId = 3;
            else
            if(ch == ')')
                stateId = 4;
            else
            if(ch == '*')
                stateId = 5;
            else
            if(ch == '+')
                stateId = 6;
            else
            if(ch == ',')
                stateId = 7;
            else
            if(ch == '-')
                stateId = 8;
            else
            if(ch == '.')
                stateId = 9;
            else
            if(ch == '/')
                stateId = 10;
            else
            if(ch == ';')
                stateId = 11;
            else
            if(ch == '<')
                stateId = 12;
            else
            if(ch == '=')
                stateId = 13;
            else
            if(ch == '>')
                stateId = 14;
            else
            if(ch == '[')
                stateId = 15;
            else
            if(ch == '\"')
                stateId = 16;
            else
            if(ch == '\'')
                stateId = 17;
            else
            if(ch == '\n')
                stateId = 18;
            else
            if(ch == '\t')
                stateId = 19;
            else
            if(ch == ']')
                stateId = 20;
            else
            if(ch == '^')
                stateId = 21;
            else
            if(ch == '_')
                stateId = 22;
            else
            if(isDigit(ch))
                stateId = 23;
            else
            if(isLetter(ch))
                stateId = 24;
            else
            if(ch == '{')
                stateId = 25;
            else
            if(ch == '|')
                stateId = 26;
            else
            if(ch == '}')
                stateId = 27;
            else
            if(ch == '~')
                stateId = 28;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 1:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{}
            }
            break;
        }
        case 2:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '&')
                stateId = 29;
            else
            if(ch == '=')
                stateId = 30;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 3:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 4:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 5:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 6:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '+')
                stateId = 31;
            else
            if(ch == '=')
                stateId = 32;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 7:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 8:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '-')
                stateId = 33;
            else
            if(ch == '=')
                stateId = 34;
            else
            if(isDigit(ch))
                stateId = 23;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 9:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 10:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 35;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 11:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 12:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '<')
                stateId = 36;
            else
            if(ch == '=')
                stateId = 37;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 13:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 38;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 14:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 39;
            else
            if(ch == '>')
                stateId = 40;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 15:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 16:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '\"')
                stateId = 41;
            else
            if(isNoTow(ch))
                stateId = 16;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 17:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '\'')
                stateId = 42;
            else
            if(isNoOne(ch))
                stateId = 17;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 18:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{}
            }
            break;
        }
        case 19:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{}
            }
            break;
        }
        case 20:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 21:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 43;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 22:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '_')
                stateId = 44;
            else
            if(isDigit(ch))
                stateId = 45;
            else
            if(isLetter(ch))
                stateId = 46;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	int id = getKeyId(token);	if(id != 0)		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));	else		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 23:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '.')
                stateId = 47;
            else
            if(ch == 'E')
                stateId = 48;
            else
            if(isDigit(ch))
                stateId = 23;
            else
            if(ch == 'e')
                stateId = 49;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("digit"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 24:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '_')
                stateId = 44;
            else
            if(isDigit(ch))
                stateId = 45;
            else
            if(isLetter(ch))
                stateId = 46;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	int id = getKeyId(token);	if(id != 0)		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));	else		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 25:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 26:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 50;
            else
            if(ch == '|')
                stateId = 51;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 27:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 28:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '=')
                stateId = 52;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 29:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 30:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 31:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 32:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 33:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 34:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 35:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 36:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 37:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 38:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 39:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 40:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 41:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("string"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 42:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("string"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 43:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 44:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '_')
                stateId = 44;
            else
            if(isDigit(ch))
                stateId = 45;
            else
            if(isLetter(ch))
                stateId = 46;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	int id = getKeyId(token);	if(id != 0)		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));	else		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 45:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '_')
                stateId = 44;
            else
            if(isDigit(ch))
                stateId = 45;
            else
            if(isLetter(ch))
                stateId = 46;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	int id = getKeyId(token);	if(id != 0)		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));	else		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 46:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == '_')
                stateId = 44;
            else
            if(isDigit(ch))
                stateId = 45;
            else
            if(isLetter(ch))
                stateId = 46;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	int id = getKeyId(token);	if(id != 0)		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));	else		fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("id"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 47:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(isDigit(ch))
                stateId = 53;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 48:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(isDigit(ch))
                stateId = 54;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 49:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(isDigit(ch))
                stateId = 54;
            else
            {
printf("Error in line %d\n", line);
exit(1);
            }
            break;
        }
        case 50:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 51:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 52:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping(token),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 53:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(ch == 'E')
                stateId = 48;
            else
            if(isDigit(ch))
                stateId = 53;
            else
            if(ch == 'e')
                stateId = 49;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("digit"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        case 54:
        {
            ch = *str++;
            pos++;
            token[len++]=ch;
            if(isDigit(ch))
                stateId = 54;
            else
            {
                token[len-1] = '\0';
                len=0;
                stateId=0;
                str--;
                pos--;
{	fprintf(outputFile,"%d\t%-15s\t%d\t%d\t%d\n", searchMapping("digit"),token,line,pos-strlen(token),strlen(token));}
            }
            break;
        }
        }
    }
}

int main(int argc, char **args)
{
    if(argc == 1)
    {
        printf("没有输入源文件名");
        return 0;
    }
    else if(argc == 2)
    {
        strcpy(FILE_NAME, args[1]);
        sprintf(OUT_FILE_NAME, "%s.out", FILE_NAME);
    }
    else
    {
        strcpy(FILE_NAME, args[1]);
        strcpy(OUT_FILE_NAME, args[2]);
    }
    FILE* file = fopen(FILE_NAME, "r");
    initMap("/home/fwm/project/C_Lexer_Grammer/Lexer/mapping");
 outputFile = fopen("./token", "w");
    while(NULL != fgets(buff, MAXSIZE_BUFF, file))
    {
        ++line;
        scanner(buff, line);
    }
    return 0;
}
