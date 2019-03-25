/**
 * 文件 testOhtbl 创建于 2017/10/27
 * @author 赵磊
 * @version 1.0
 */
#include <stdlib.h>
#include <stdio.h>
#include "../inlclude/hashpjw.h"
#include "../inlclude/hash/ohtbl.h"
#include "../inlclude/compare.h"

#define WORD_LENGTH 20

void printf_ele(const void *k, const void *v) {

  printf("key-> %s, word-> %d\n", (char *) k, *(int *) v);
}

int main(void) {
  OHTbl *ohTbl;
  ohtbl_init(&ohTbl, 16, hashpjw, string_compare, free, free);

  int *key;
  char *word;

  for (int i = 0; i < 20; i++) {
    key = malloc(sizeof(int));
    *key = i;

    printf("请输入字符串(20字以内): ");
    word = malloc(WORD_LENGTH * sizeof(char));
    scanf("%s", word);
    ohtbl_insert(ohTbl, word, key);
  }

  int index = 12;

  ohtbl_remove(ohTbl, &index, NULL);

  ohtbl_foreach(ohTbl, printf_ele);

  printf("哈希表大小-> %d\n", ohtbl_size(ohTbl));

  ohtbl_destroy(&ohTbl);

  return 0;
}
