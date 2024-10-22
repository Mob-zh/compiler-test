///demo 2:通过分隔符来分隔字符串demo
#include <stdlib.h>
#include "stdio.h"
#include "string.h"

int strtokErr() {
    char* temp = NULL;
    char input[20] = "A=a,B=b,C=c,D=d";

    printf("%s\n", input);
    temp = strtok(input, ",");  ////strtok的第一个参数不能为指向常量的指针，我们使用字符数组就好
    printf("temp:%s\n", temp);

    while (temp != NULL) {     ////第一轮strtok循环
        temp = strtok(NULL, ",");   ////分隔符为字符串时，则其中的每个字符都是分隔符.strtok里面有个静态变量会记录上次分割的地方，
                                      //// 如果第一个参数为NULL，则会使用静态变量作为起始点,方便外面写循环。因此strtok函数需要避免嵌套调用
        /// 调用会出错
        if (temp != NULL) {
            char* equal = temp;
            equal = strtok(temp, "=");    ////在外层strtok的循环中不能调用strtok解析其他的字段，解决方法可以先把while中调用的结果先存起来，
            printf("equal:%s\n", equal);     //////然后再调用strtok解析其他的字段。
            while (equal != NULL) {
                equal = strtok(NULL, "=");
               printf("equal:%s\n", equal);
            }
        }
        
        printf("temp:%s\n", temp);
    }
    return 0;
}


int strtokOk() {
    char* temp[5] = { NULL };
    int i = 0;
    char input[20] = "A=a,B=b,C=c,D=d";

    printf("%s\n", input);
    temp[0] = strtok(input, ",");  ////strtok的第一个参数不能为指向常量的指针，我们使用字符数组就好
    printf("temp:%s\n", temp);

    while (temp[i] != NULL) {
        i++;
        temp[i] = strtok(NULL, ",");   ////先存起来，避免的strtok的嵌套调用
        printf("i:%d,temp:%s\n",i,temp[i]);
    }

    int j = 0;
    while (j < i-1) {
        if (temp[j] != NULL) {
            char* left = temp[j];
            left = strtok(temp[j], "=");
            printf("left:%s\n", left);
            while (left != NULL) {
                left = strtok(NULL, "=");
                printf("left:%s\n", left);
            }
        }
        j++;
    }
    return 0;
}

int strtokSingle() {
    char* temp = NULL;
    char input[128] = "A=a,B=b,C=c,D=d";

    printf("%s\n", input);
    temp = strtok(input, ",");  ////strtok的第一个参数不能为指向常量的指针，我们使用字符数组就好
    printf("temp:%s\n", temp);

    while (temp != NULL) {
        printf("len:%d\n", strlen(temp));    ///strlen可以获得已\0结尾的字符串的长度，需要包含头文件string.h
        temp = strtok(NULL, ",");   ////strtok里面有个静态变量会记录上次分割的地方，
        //// 如果第一个参数为NULL，则会使用静态变量作为起始点,方便外面写循环。因此strtok函数需要避免嵌套调用
        printf("temp:%s\n", temp);
    }
    return 0;
}


int main() {
    strtokSingle();

 //   strtokErr();

//    strtokOk();
}