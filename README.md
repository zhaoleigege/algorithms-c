# algorithms-c

项目记录了用 c 语言实现的算法和数据结构，实现的算法包括排序算法，链表操作算法，实现的数据结构有链表、堆、哈希表和树。
其中数据结构的实现体现了 c 语言面向对象编程的一些特性。

**指针详讲**

在c语言中指针代表了它所指定的数据类型的值的存储地址，该存储地址就是该数据类型的值。

`%p`以十六进制的形式打印参数存储的值。

```c
char str[] = {'a', 'b', 'c', '\0'};
printf("%s\n", str);         /* 打印字符串 */

printf("%p\n", str[1]);      /* 打印b字符串的ASCII十六进制值 */
rintf("%p\n", str);         /* 打印字符串的起始地址（c语言中数组变量为指针） */

/**--->
abc
0x62
0x7fff4d06992c
*/
```

```c
int num = 10;
printf("num->%p\n", num);   /* 打印num值的十六进制表示方式 */
printf("num地址->%p\n", &num);  /* 打印存储num值的地址 */

/**--->
num->0xa
num地址->0x7fff4d069928
*/
```

```c
int *p = malloc(sizeof(int));           /* p为一个整型指针。这里相当于在内存中找一段空闲地址，并把地址的值存储在p中 */
*p = 20;                                /* p指向的整型值 */
    
printf("p存储的整型值为->%d\n", *p);      /* 打印p存储的值 */
printf("p的值为->%p\n", p);              /* 打印p（指针）存储的值（即保存地址） */
printf("p的地址为->%p\n", &p);           /* 打印p（指针）存储的位置 */
    
*p = 30;
printf("p存储的整型值为->%d\n", *p);      /* 打印p存储的值 */
printf("p的值为->%p\n", p);              /* 打印p（指针）存储的值（即保存地址） */
printf("p的地址为->%p\n", &p);           /* 打印p（指针）存储的位置 */

free(p);

/**--->
p存储的整型值为->20
p的值为->0x5566a4dd8670
p的地址为->0x7fff4d069920
p存储的整型值为->30
p的值为->0x5566a4dd8670
p的地址为->0x7fff4d069920
*/
```

