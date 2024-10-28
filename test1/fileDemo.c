#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN  256
#define VLEN    256
#define TLEN    256
#define PLEN    256

struct G_st {
    char V[VLEN];       // 非终结符集合
    char T[TLEN];       // 终结符集合
    char P[PLEN];       // 产生式
    char S;             // 开始符号
    int type;           // 文法类型
} G;

static int Check_Gtype(struct G_st G);

int main(int argc, char* argv[]) {

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    char lineBuf[BUFLEN];
    FILE* source;
    int i = 1;
    
    const char* pgm = argv[1];
    source = fopen(pgm, "r");   // 以只读方式打开文件
    if(source == NULL) {
        perror("fopen()");
        exit(1);
    }

    // 按行读取文件内容并处理
    while (fgets(lineBuf, BUFLEN - 1, source)) {
        int lineBuf_len = strlen(lineBuf);
        if (lineBuf_len <= 2) continue;  // 跳过空行
        
        switch (i) {
            case 1:
                strcpy(G.V, lineBuf);
                printf("------------------------\n");
                printf("V: %s", G.V);
                i++;
                break;
            case 2:
                strcpy(G.T, lineBuf);
                printf("T: %s", G.T);
                i++;
                break;
            case 3:
                sprintf(G.P, "%s", lineBuf);
                printf("P: %s", G.P);
                i++;
                break;
            case 4:
                G.S = lineBuf[0];
                printf("S: %c\n\n", G.S);

                G.type = Check_Gtype(G);
                
                printf("G is type %d Grammar\n", G.type);
                printf("------------------------\n");

                i = 1;  // 重置，准备读取下一个文法
                break;
            default:
                break;
        }
    }

    fclose(source);  // 关闭文件
    return 0;
}

// 删除子字符串
static void del_SubStr(char *str, char *substr) {
    int len1 = strlen(str), len2 = strlen(substr);
    while (strstr(str, substr)) {
        int pos = strstr(str, substr) - str;
        for (int i = pos; i < len1 - len2; ++i) {
            str[i] = str[i + len2];
        }
        len1 -= len2;
        str[len1] = '\0';
    }
}

// 判断文法的类型
static int Check_Gtype(struct G_st G) {
    char* temp = NULL;
    char G_P[PLEN][PLEN];
    int cnt = 0;

    temp = strtok(G.P, ",");
    strcpy(G_P[cnt++], temp);
    while ((temp = strtok(NULL, ","))) {
        strcpy(G_P[cnt++], temp);
    }
    
    int isRegular = 1, isContextFree = 1, isContextSensitive = 1;
    
    
    // 检查每个产生式的左部和右部
    for (int i = 0; i < cnt; i++) {
        char *lstr, *rstr;
        int llen, rlen;

        llen = strlen(lstr = strtok(G_P[i], "->"));
        rlen = strlen(rstr = strtok(NULL, "->"));

        char sub_rstr[PLEN][PLEN];
        int sub_cnt = 0;
        temp = strtok(rstr,"|");
        
        if(temp != NULL){
            strcpy(sub_rstr[sub_cnt++],temp);
            while ((temp = strtok(NULL, "|"))) {
                strcpy(sub_rstr[sub_cnt++], temp);
            }
            //
            for (int j = 0; j < sub_cnt; j++)
            {
                rlen = strlen(sub_rstr[j]);
                strcpy(rstr,sub_rstr[j]);
                //最后一个规则的换行符
                if (i == cnt - 1 && j == sub_cnt - 1) {
                    rstr[strlen(rstr) - 2] = '\0';
                    rlen -= 2;
                }

                 printf("%s[%d] -> %s[%d]\n", lstr, llen, rstr, rlen);

                // 判断是否为正规文法
                if (llen != 1 || rlen > 2 || (rlen == 2 && (rstr[1] >= 'A' && rstr[1] <= 'Z') && (rstr[0] >= 'A' && rstr[0] <= 'Z')) ) {
                    isRegular = 0;
                }
                // 判断是否为上下文无关文法
                int Freecnt = 0;
                for(int i = 0; i < llen; i++){
                    if((lstr[i] >= 'A' && lstr[0] <= 'Z')) {
                        if(++Freecnt >= 2){
                            isContextFree = 0;
                            break;
                        }
                    }
                }
                // 判断是否为上下文相关文法
                if (llen > rlen) {
                    isContextSensitive = 0;
                    isContextFree = 0;
                    isRegular = 0;
                }
                
            }
            
        }
        else{
            //最后一个规则的换行符
            if (i == cnt - 1) {
                rstr[strlen(rstr) - 2] = '\0';
                rlen -= 2;
            }

            printf("%s[%d] -> %s[%d]\n", lstr, llen, rstr, rlen);

            // 判断是否为正规文法
            if (llen != 1 || rlen > 2 || (rlen == 2 && (rstr[1] >= 'A' && rstr[1] <= 'Z') && (rstr[0] >= 'A' && rstr[0] <= 'Z')) ) {
                isRegular = 0;
            }
            // 判断是否为上下文无关文法
            int Freecnt = 0;
            for(int i = 0; i < llen; i++){
                if((lstr[i] >= 'A' && lstr[0] <= 'Z')) {
                    if(++Freecnt >= 2){
                        isContextFree = 0;
                        break;
                    }
                }
            }
            // 判断是否为上下文相关文法
            if (llen > rlen) {
                isContextSensitive = 0;
                isContextFree = 0;
                isRegular = 0;
            }
        }
        
    }

    if (isRegular) {
        return 3;  // 正规文法
    } else if (isContextFree) {
        return 2;  // 上下文无关文法
    } else if (isContextSensitive) {
        return 1;  // 上下文相关文法
    } else {
        return 0;  // 无限制文法
    }
}
