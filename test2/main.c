#define _CRT_SECURE_NO_WARNINGS
/****************************************************/
/* 文件: main.c                                   */
/* TINY 编译器的全局类型和变量                       */
/* 必须在其他 include 文件之前引入                    */
/* 编译器构造：原理与实践                             */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./Tiny Scanner/SCAN.H"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED 表示保留字的数量 */
#define MAXRESERVED 8

// TokenType 枚举类型表示 TINY 语言的所有 Token 类型
typedef enum
{
    ENDFILE,
    ERROR, // 文件结束、错误
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE, // 保留字
    ID,
    NUM, // 标识符、多字符数字
    ASSIGN,
    EQ,
    LT,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    LPAREN,
    RPAREN,
    SEMI // 特殊符号
} TokenType;

// 其他全局变量定义
extern FILE *source;  /* 源代码文件 */
extern FILE *listing; /* 列表输出文件 */
extern FILE *code;    /* 用于 TM 模拟器的代码文件 */
extern int lineno;    /* 列表文件中的行号 */

// 语法树的节点种类
typedef enum
{
    StmtK,
    ExpK
} NodeKind;
typedef enum
{
    IfK,
    RepeatK,
    AssignK,
    ReadK,
    WriteK
} StmtKind;
typedef enum
{
    OpK,
    ConstK,
    IdK
} ExpKind;

/* ExpType 用于类型检查 */
typedef enum
{
    Void,
    Integer,
    Boolean
} ExpType;

typedef enum
{
    START,     // 初始状态
    INNUM,     // 处理数字状态
    INID,      // 处理标识符状态
    INASSIGN,  // 处理赋值符 ":=" 状态
    INCOMMENT, // 处理注释状态
    DONE       // 处理完成
} StateType;

#define MAXCHILDREN 3

typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union
    {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union
    {
        TokenType op;
        int val;
        char *name;
    } attr;
    ExpType type; /* 用于表达式的类型检查 */
} TreeNode;

// 追踪标志
extern int EchoSource;   // 是否在解析时输出源代码
extern int TraceScan;    // 是否输出词法分析信息
extern int TraceParse;   // 是否输出语法树
extern int TraceAnalyze; // 是否在符号表插入和查找时输出信息
extern int TraceCode;    // 是否在生成代码时输出注释
extern int Error;        // 是否中止处理

#endif

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN 表示 token 的最大长度 */
#define MAXTOKENLEN 40

/* tokenString 数组存储每个 token 的词素 */
extern char tokenString[MAXTOKENLEN + 1];

/* 函数 getToken 返回源文件中的下一个 token */
TokenType getToken(void);

#endif

#ifndef _UTIL_H_
#define _UTIL_H_

// 函数声明
void printToken(TokenType, const char *); /* 打印 Token 和词素 */
TreeNode *newStmtNode(StmtKind);          /* 新建语句节点 */
TreeNode *newExpNode(ExpKind);            /* 新建表达式节点 */
char *copyString(char *);                 /* 复制字符串 */
void printTree(TreeNode *);               /* 打印语法树 */

#endif

/* printToken 函数打印 token 和词素到 listing 文件 */
void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
    case IF:
    case THEN:
    case ELSE:
    case END:
    case REPEAT:
    case UNTIL:
    case READ:
    case WRITE:
        fprintf(listing, "保留字: %s\n", tokenString);
        break;
    case ASSIGN:
        fprintf(listing, ":=\n");
        break;
    case LT:
        fprintf(listing, "<\n");
        break;
    case EQ:
        fprintf(listing, "=\n");
        break;
    case LPAREN:
        fprintf(listing, "(\n");
        break;
    case RPAREN:
        fprintf(listing, ")\n");
        break;
    case SEMI:
        fprintf(listing, ";\n");
        break;
    case PLUS:
        fprintf(listing, "+\n");
        break;
    case MINUS:
        fprintf(listing, "-\n");
        break;
    case TIMES:
        fprintf(listing, "*\n");
        break;
    case OVER:
        fprintf(listing, "/\n");
        break;
    case ENDFILE:
        fprintf(listing, "EOF\n");
        break;
    case NUM:
        fprintf(listing, "数字, 值= %s\n", tokenString);
        break;
    case ID:
        fprintf(listing, "标识符, 名称= %s\n", tokenString);
        break;
    case ERROR:
        fprintf(listing, "错误: %s\n", tokenString);
        break;
    default:
        fprintf(listing, "未知的 token: %d\n", token);
    }
}

/* ...（省略语法树相关函数实现）... */

/* getToken 函数实现了词法分析器，通过状态机识别 Token */
TokenType getToken(void)
{
    int tokenStringIndex = 0; // tokenString 索引
    TokenType currentToken;   // 当前识别到的 token
    StateType state = START;  // 初始状态
    int save;                 // 是否保存字符到 tokenString

    while (state != DONE)
    {
        int c = getNextChar(); // 读取下一个字符
        save = TRUE;
        switch (state)
        {
        case START:
            if (isdigit(c))
                state = INNUM; // 数字
            else if (isalpha(c))
                state = INID; // 标识符
            else if (c == ':')
                state = INASSIGN; // 赋值符
            else if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
                save = FALSE; // 跳过空白
            else if (c == '{')
            {
                save = FALSE;
                state = INCOMMENT;
            } // 注释
            else
            {
                state = DONE; // 处理特殊符号
                switch (c)
                {
                case EOF:
                    save = FALSE;
                    currentToken = ENDFILE;
                    break;
                case '=':
                    currentToken = EQ;
                    break;
                case '<':
                    currentToken = LT;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '/':
                    currentToken = OVER;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
            break;
        case INCOMMENT: // 注释处理
            save = FALSE;
            if (c == EOF)
            {
                state = DONE;
                currentToken = ENDFILE;
            }
            else if (c == '}')
                state = START;
            break;
        case INASSIGN: // 处理 ":=" 赋值符
            state = DONE;
            if (c == '=')
                currentToken = ASSIGN;
            else
            {
                ungetNextChar();
                save = FALSE;
                currentToken = ERROR;
            }
            break;
        case INNUM: // 处理数字
            if (!isdigit(c))
            {
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID: // 处理标识符
            if (!isalpha(c))
            {
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;
        default:
            state = DONE;
            currentToken = ERROR;
            break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
            tokenString[tokenStringIndex++] = (char)c;
        if (state == DONE)
        {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan)
    {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}

int main(int argc, char *argv[])
{
    source = fopen("tiny.txt", "r"); // 打开源文件

    listing = stdout; // 将输出设置为屏幕输出
    while (getToken() != ENDFILE)
        ; // 循环读取 Token，直到文件结束
    fclose(source);
    return 0;
}
