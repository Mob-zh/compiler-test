﻿#define _CRT_SECURE_NO_WARNINGS
/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* 保留字数量 */
#define MAXRESERVED 20

typedef enum
/* book-keeping tokens */
{
    ENDFILE,
    ERROR,
    /* reserved words */
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    /* multicharacter tokens */
    ID,
    NUM,
    /* special symbols */
    ASSIGN,
    EQ,
    LT,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    LPAREN,
    RPAREN,
    SEMI,
    /*add reserved words*/
    TRUE1,
    FALSE1,
    OR,
    AND,
    NOT,
    INT,
    BOOL,
    FLOAT,
    DOUBLE,
    STRING,
    DO,
    WHILE,
    /*新的特殊符号*/
    STRING_S,
    GT,
    LE,
    GE,
    NEQ,
    COMMA,
    QUOTE

} TokenType;

extern FILE *source;  /* source code text file 源文件 */
extern FILE *listing; /* listing output text file 输出文件*/
extern FILE *code;    /* code text file for TM simulator 目标代码文件*/

extern int lineno; /* source line number for listing 记录当前源文件的行号*/

int Printerror = 1;
/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

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

/* ExpType is used for type checking */
typedef enum
{
    Void,
    Integer,
    Boolean
} ExpType;

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
    ExpType type; /* for type checking of exps */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void);

#endif

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType, const char *);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *);

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *);

#endif

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
    case TRUE1:
    case FALSE1:
    case OR:
    case AND:
    case NOT:
    case INT:
    case BOOL:
    case STRING:
    case FLOAT:
    case DOUBLE:
    case DO:
    case WHILE:
    case IF:
    case THEN:
    case ELSE:
    case END:
    case REPEAT:
    case UNTIL:
    case READ:
    case WRITE:
        fprintf(listing,
                "reserved word: %s\n", tokenString);
        break;
    case STRING_S:
        fprintf(listing,
                "string: %s\n", tokenString);
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
    case GT:
        fprintf(listing, ">\n");
        break;
    case LE:
        fprintf(listing, "<=\n");
        break;
    case GE:
        fprintf(listing, ">=\n");
        break;
    case NEQ:
        fprintf(listing, "!=\n");
        break;
    case COMMA:
        fprintf(listing, ",\n");
        break;
    case QUOTE:
        fprintf(listing, "\'\n");
        break;
    case ENDFILE:
        fprintf(listing, "EOF\n");
        break;
    case NUM:
        fprintf(listing,
                "NUM, val= %s\n", tokenString);
        break;
    case ID:
        fprintf(listing,
                "ID, name= %s\n", tokenString);
        break;
    case ERROR:
        if (Printerror)
            fprintf(listing,
                    "ERROR: %s\n", tokenString);
        break;
    default: /* should never happen */
        fprintf(listing, "Unknown token: %d\n", token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *s)
{
    int n;
    char *t;
    if (s == NULL)
        return NULL;
    n = strlen(s) + 1;
    t = (char *)malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
        strcpy(t, s);
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while (tree != NULL)
    {
        printSpaces();
        if (tree->nodekind == StmtK)
        {
            switch (tree->kind.stmt)
            {
            case IfK:
                fprintf(listing, "If\n");
                break;
            case RepeatK:
                fprintf(listing, "Repeat\n");
                break;
            case AssignK:
                fprintf(listing, "Assign to: %s\n", tree->attr.name);
                break;
            case ReadK:
                fprintf(listing, "Read: %s\n", tree->attr.name);
                break;
            case WriteK:
                fprintf(listing, "Write\n");
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        }
        else if (tree->nodekind == ExpK)
        {
            switch (tree->kind.exp)
            {
            case OpK:
                fprintf(listing, "Op: ");
                printToken(tree->attr.op, "\0");
                break;
            case ConstK:
                fprintf(listing, "Const: %d\n", tree->attr.val);
                break;
            case IdK:
                fprintf(listing, "Id: %s\n", tree->attr.name);
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        }
        else
            fprintf(listing, "Unknown node kind\n");
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

/* states in scanner DFA */
typedef enum
{
    START,
    INASSIGN,
    INCOMMENT,
    INNUM,
    INID,
    INSTRING,
    DONE
} StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0;      /* current position in LineBuf */
static int bufsize = 0;      /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void)
{
    if (!(linepos < bufsize))
    {
        lineno++;
        if (fgets(lineBuf, BUFLEN - 1, source))
        {
            if (EchoSource)
                fprintf(listing, "%d: %s", lineno, lineBuf);
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        }
        else
        {
            EOF_flag = TRUE;
            return EOF;
        }
    }
    else
        return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{
    if (!EOF_flag)
        linepos--;
}

/* lookup table of reserved words */
static struct
{
    const char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"if", IF}, {"then", THEN}, {"else", ELSE}, {"end", END}, {"repeat", REPEAT}, {"until", UNTIL}, {"read", READ}, {"write", WRITE}, {"true", TRUE1}, {"false", FALSE1}, {"or", OR}, {"and", AND}, {"not", NOT}, {"int", INT}, {"bool", BOOL}, {"float", FLOAT}, {"double", DOUBLE}, {"string", STRING}, {"do", DO}, {"while", WHILE}};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup(char *s)
{
    int i;
    for (i = 0; i < MAXRESERVED; i++)
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{ /* index for storing into tokenString */
    int tokenStringIndex = 0;
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state - always begins at START */
    StateType state = START;
    /* flag to indicate save to tokenString */
    int save;
    Printerror = TRUE;

    while (state != DONE)
    {
        int c = getNextChar();
        save = TRUE;
        switch (state)
        {
        case START:
            if (isdigit(c))
                state = INNUM;
            else if (isalpha(c))
                state = INID;
            else if (c == ':')
                state = INASSIGN;
            else if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
                save = FALSE;
            else if (c == '{')
            {
                save = FALSE;
                state = INCOMMENT;
            }
            else if (c == '\'')
            {
                // 切换到字符串识别状态，初始化 tokenString 保存
                state = INSTRING;
            }
            else
            {
                state = DONE;
                switch (c)
                {
                case EOF:
                    save = FALSE;
                    currentToken = ENDFILE;
                    break;
                case '=':
                    currentToken = EQ;
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
                case '>':
                    if (getNextChar() == '=')
                    {
                        currentToken = GE;
                    }
                    else
                    {
                        ungetNextChar();
                        currentToken = GT;
                    }
                    break;
                case '<':
                    if (getNextChar() == '=')
                    {
                        currentToken = LE;
                    }
                    else
                    {
                        ungetNextChar();
                        currentToken = LT;
                    }
                    break;
                case '!':
                    if (getNextChar() == '=')
                    {
                        currentToken = NEQ;
                    }
                    else
                    {
                        ungetNextChar();
                        currentToken = ERROR; // 非法字符处理
                    }
                    break;
                case ',':
                    currentToken = COMMA;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
            break;
        case INCOMMENT:
            save = FALSE; // 忽略注释内容
            if (c == EOF)
            {
                // 注释没有闭合的错误处理
                Printerror = FALSE;
                state = DONE;
                currentToken = ERROR;
                fprintf(listing, "\t%d: Error: Comment block opened with '{' was not closed with '}'\n", lineno);
            }
            else if (c == '}')
            {
                // 注释闭合，恢复到开始状态
                state = START;
            }
            break;
        case INASSIGN:
            state = DONE;
            if (c == '=')
                currentToken = ASSIGN;
            else
            { /* backup in the input */
                ungetNextChar();
                save = FALSE;
                currentToken = ERROR;
            }
            break;
        case INNUM:
            if (!isdigit(c))
            { /* backup in the input */
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(c) && !isdigit(c))
            { /* backup in the input */
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;
        case INSTRING:
            if (c != '\'' && c != EOF && c != '\n')
            {
                // 保存字符串中的字符
                // tokenString[tokenStringIndex++] = c;
            }
            else if (c == '\'')
            {
                // 遇到结束符号单引号，字符串结束
                currentToken = STRING_S;
                state = DONE;
            }
            else
            {
                // 如果遇到文件结尾未闭合，引发错误
                // '没有闭合的错误处理
                Printerror = FALSE;
                state = DONE;
                currentToken = ERROR;
                fprintf(listing, "\t%d: Error: string block opened with \' was not closed with \'\n", lineno);
            }
            break;
        case DONE:
        default: /* should never happen */
            fprintf(listing, "Scanner Bug: state= %d\n", state);
            state = DONE;
            currentToken = ERROR;
            break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
        {
            // fprintf(listing, "[%c]%s\n", c, tokenString);
            tokenString[tokenStringIndex++] = (char)c;
        }
        if (state == DONE)
        {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
            {
                currentToken = reservedLookup(tokenString);
            }
        }
    }
    if (TraceScan && Printerror)
    {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
} /* end getToken */

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE TRUE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE TRUE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

int main(int argc, char *argv[])
{
    source = fopen(argv[1], "r"); ////stdin;

    listing = stdout; /* send listing to screen */

    while (getToken() != ENDFILE)
        ;

    fclose(source);

    return 0;
}
