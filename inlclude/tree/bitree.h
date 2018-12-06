/**
 * 文件 bitree 创建于 2017/10/28
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef BITREE_H
#define BITREE_H

typedef struct BiTree_ BiTree;

bool bitree_init(BiTree **tree, void (*ele_free)(void *data), int (*compare)(const void *v1, const void *v2));

void bitree_destroy(BiTree **tree);

bool bitree_insert(BiTree *tree, void *data);

unsigned bitree_look_up(BiTree *tree, void *data, size_t size);

bool bitree_get(BiTree *tree, int index, void *data, size_t size);

bool bitree_delete(BiTree *tree, void **data);

void bitree_traverse(BiTree *tree, void (*action)(void *data));

unsigned int bitree_size(BiTree *tree);

#endif
