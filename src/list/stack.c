/**
 * 文件 stack 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */
#include <stdlib.h>
#include "../../inlclude/list/stack.h"

struct _stack_ele {
  StackValue data;
  struct _stack_ele *next;
};

struct _stack {
  int size;

  struct _stack_ele *head;
  void (*ele_free)(StackValue data);
};

bool stack_init(Stack **stack, void (*ele_free)(StackValue data)) {
  if (ele_free == NULL)
    return false;

  *stack = malloc(sizeof(struct _stack));
  if (*stack == NULL)
    return false;

  (*stack)->size = 0;
  (*stack)->head = NULL;
  (*stack)->ele_free = ele_free;

  return true;
}

void stack_destroy(Stack **stack) {
  while ((*stack)->size > 0)
    stack_pop(*stack, NULL);

  (*stack)->head = NULL;
  (*stack)->ele_free = NULL;

  free(*stack);
  *stack = NULL;
}

bool stack_push(Stack *stack, StackValue data) {
  struct _stack_ele *node = malloc(sizeof(struct _stack_ele));
  if (node == NULL)
    return false;

  node->data = data;
  node->next = stack->head;
  stack->head = node;
  stack->size++;

  return true;
}

bool stack_pop(Stack *stack, StackValue *data) {
  if (stack->size < 0)
    return false;

  struct _stack_ele *node = stack->head;
  stack->head = node->next;
  if (data == NULL)
    stack->ele_free(node->data);
  else
    *data = node->data;

  free(node);

  stack->size--;

  return true;
}

StackValue stack_peek(Stack *stack) {
  if (stack->size <= 0)
    return NULL;

  return stack->head->data;
}

int stack_size(const Stack *stack) {
  return stack->size;
}


