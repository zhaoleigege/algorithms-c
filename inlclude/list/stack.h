/**
 * 文件 stack 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef STACK_H
#define STACK_H

typedef void *StackValue;

typedef struct _stack Stack;

bool stack_init(Stack **stack, void (*ele_free)(StackValue data));

void stack_destroy(Stack **stack);

bool stack_push(Stack *stack, StackValue data);

bool stack_pop(Stack *stack, StackValue *data);

StackValue stack_peek(Stack *stack);

int stack_size(const Stack *stack);

#endif
