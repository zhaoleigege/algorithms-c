/**
 * 文件 uf_test 创建于 2017/11/12
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include "../inlclude/uf.h"

void uf_printf(int index, int count) {
  printf("下标->%d, 连通量->%d\n", index, count);
}

int main(void) {
  FILE *file;
  file = fopen("/Users/BUSE/Documents/IntellijCode/algorithms/algs4-data/largeUF.txt", "r");

  UF *uf;
  int num1, num2;

  fscanf(file, "%d", &num1);

  uf_init(&uf, num1);

  while (fscanf(file, "%d", &num1) > 0) {
    fscanf(file, "%d", &num2);
    uf_union(uf, num1, num2);
  }

//  uf_traverse(uf, uf_printf);

  printf("连通量%d\n", uf_count(uf));

  fclose(file);
  uf_destroy(&uf);

  return 0;
}