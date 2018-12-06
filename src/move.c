/**
 * 文件 test 创建于 2017/11/10
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void num_exch(int *num1, int *num2) {
  int *temp = num1;
  *num1 = *num2;
  *num2 = *temp;
}

static int Gcd(int num1, int num2) {
  int mod = num1 % num2;
  return !mod ? num2 : Gcd(num2, mod);
}

static inline int GCD(int num1, int num2) {
  if (num2 > num1)
    num_exch(&num1, &num2);

  int mod = num1 % num2;

  while (mod != 0) {
    num1 = num2;
    num2 = mod;
    mod = num1 % num2;
  }

  return num2;
}

void move(void **array, int start, int end, int mnum, size_t size) {
//  for (int i = 0; i < 10; i++)
//    printf("%d\n", *(int *) (*array + i * size));

  int length = end - start + 1;
  int gcd = Gcd(length, mnum);

  int count = 0;
  void *temp = malloc(size);
  void *replace = malloc(size);

  printf("%p\n", *array);
  printf("%p\n", *array + 1 * size);

  while (count < gcd) {
    int index = start + count;
    memcpy(temp, *array + index * size, size);
    int current = (index + mnum) % length;
    int next = current;

    while (current != index) {
      memcpy(replace, *array + next * size, size);
      memcpy(*array + next * size, temp, size);
      current = next;
      next = (next + mnum) % length;
      memcpy(temp, replace, size);
    }
    count++;
  }

  free(temp);
  free(replace);
}
int main(void) {
  int *a = malloc(10 * sizeof(int));

  for (int i = 0; i < 10; i++)
    a[i] = i + 1;

  move((void **) &a, 0, 9, 6, sizeof(int));

  for (int i = 0; i < 10; i++)
    printf("%d ", a[i]);

  printf("\n");

  free(a);

  return 0;
}
