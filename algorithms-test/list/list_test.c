#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(void){
    char str[] = {'a', 'b', 'c', '\0'};
    printf("%s\n", str);         /* 打印字符串 */

    printf("%p\n", str[1]);      /* 打印b字符串的ASCII十六进制值 */
    printf("%p\n", str);         /* 打印字符串的起始地址（c语言中数组变量为指针） */

    int num = 10;
    printf("num->%p\n", num);   /* 打印num值的十六进制表示方式 */
    printf("num->%p\n", &num);  /* 打印存储num值的地址 */

    int *p = malloc(sizeof(int));           /* p为一个整型指针 */
    *p = 20;                                /* p指向的整型值 */
    
    printf("p存储的整型值为->%d\n", *p);      /* 打印p存储的值 */
    printf("p的值为->%p\n", p);              /* 打印p（指针）存储的值（即保存地址） */
    printf("p的地址为->%p\n", &p);           /* 打印p（指针）存储的位置 */
    
    *p = 30;
    printf("p存储的整型值为->%d\n", *p);      /* 打印p存储的值 */
    printf("p的值为->%p\n", p);              /* 打印p（指针）存储的值（即保存地址） */
    printf("p的地址为->%p\n", &p);           /* 打印p（指针）存储的位置 */

    free(p);

    int num1 = 20;
    printf("num1->%p\n", num1);
    printf("num1->%p\n", &num1);

    List *list;

    printf("指针的地址%p\n", &list);
    // printf("分配前地址->%p\n", list);
    // printf("是否为空%d\n", list == NULL);
    init(&list, free);
    // printf("分配后地址->%p\n", list);
    // printf("分配后指针地址->%p\n", &list);

    return 0;
}