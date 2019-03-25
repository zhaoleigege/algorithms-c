/**
 * 文件 testStack 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "../inlclude/list/stack.h"

#define LENGTH 100

int main(void) {
  Stack *stack;
  stack_init(&stack, free);
  int *data;

  for (int i = 0; i < LENGTH; i++) {
    data = malloc(sizeof(int));
    *data = i;
    stack_push(stack, data);
  }

  printf("peek-> %d\n", *(int *) stack_peek(stack));

  printf("size-> %d\n", stack_size(stack));

  for (int i = 0; i < LENGTH; i++) {
    stack_pop(stack, (StackValue *) &data);
    printf("data-> %d\n", *data);
    free(data);
  }

  printf("size-> %d\n", stack_size(stack));

  stack_destroy(&stack);

  return 0;
}