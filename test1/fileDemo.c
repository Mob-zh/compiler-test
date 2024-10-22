//demo 1   文件输入：文件操作的读取
#define BUFLEN  256
#define VLEN    256
#define TLEN    256
#define PLEN    256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct G_st
{
    char V[VLEN];
    char T[TLEN];
    char P[PLEN];
    char S;
    int type;
}G;

static int Check_Gtype(struct G_st G);

int main(int argc,char* argv[]) {

    if(argc < 2){
        fprintf(stderr,"Usage:%s <filepath>\n",argv[0]);
        exit(1);
    }

    char lineBuf[BUFLEN];
    FILE* source;
    int i = 1;
    
    const char* pgm = argv[1];   
    source = fopen(pgm, "r");    ////获得文件指针，"r"表示只读方式打开
    if(source == NULL){
        perror("fopen()");
        exit(1);
    }
    while (1) {

        if (fgets(lineBuf, BUFLEN - 1, source)) {  ///逐行读入文件内容，每次读入的内容都存放在lineBuf中
            int lineBuf_len;
            lineBuf_len = strlen(lineBuf);
            if((lineBuf_len = strlen(lineBuf)) <= 2){
                continue;
            }
            switch (i)
            {
            case 1:
                /* code */
                strcpy(G.V,lineBuf);
                printf("------------------------\n");
                printf("V:%s",G.V);
                i++;
                break;
            case 2:
                /* code */
                strcpy(G.T,lineBuf);
                printf("T:%s",G.T);
                i++;
                break;
            case 3:
                /* code */
                sprintf(G.P,"%s",lineBuf);
                printf("P:%s",G.P);
                i++;
                break;
            case 4:
                /* code */
                G.S = lineBuf[0];
                printf("S:%c\n\n",G.S);

                G.type = Check_Gtype(G);
                
                printf("G Is type %d Grammar\n",G.type);
                printf("------------------------\n");

                i = 1;  
                break;

            default:
                break;
            }
            ///这里按照乔姆斯基的分类store每行content

            // printf("[%ld]%s", strlen(lineBuf),lineBuf);
        }
        else {   ////已经到了文件末尾，可以直接退出
            ////end of file
            break;
        }
    }


    fclose(source);   ///用完后要关闭文件指针
    return 0;
}

static void del_SubStr(char *str,char *substr)
{
    int len1 = strlen(str),len2 = strlen(substr);
    while(strstr(str,substr)){
        int pos = strstr(str,substr)-str;
        for(int i = pos;i<len1-len2;++i){
            str[i] = str[i+len2];
        }
        len1 -= len2;
        str[len1] = '\0';
    }

}
static int Check_Gtype(struct G_st G){

    char* temp = NULL;
    char G_P[PLEN][PLEN];
    int cnt = 0;
    printf("%s", G.P);
    temp = strtok(G.P, ",");  ////strtok的第一个参数不能为指向常量的指针，我们使用字符数组就好
    // printf("temp:%s\n", temp);
    strcpy(G_P[cnt++],temp);
    while (1) {
        // printf("len:%ld\n", strlen(temp));    ///strlen可以获得已\0结尾的字符串的长度，需要包含头文件string.h
        temp = strtok(NULL, ",");   ////strtok里面有个静态变量会记录上次分割的地方，
        if(temp == NULL) break;

        strcpy(G_P[cnt++],temp);
    }
    
    // printf("%d\n",cnt);
    for (int i = 0; i < cnt; i++)
    {
        char *lstr, *rstr;
        int llen = 0,rlen = 0;
        llen = strlen(lstr = strtok(G_P[i],"->"));
        rlen = strlen(rstr = strtok(NULL,"->"));
        
        if(i == cnt - 1){
            rstr[strlen(rstr)-2] = 0;
            rlen -= 2;
        }
        printf("%s[%d]->%s[%d]\n",lstr,llen,rstr,rlen);        

    }
    

    return 0;
}

