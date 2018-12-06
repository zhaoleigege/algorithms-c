/**
 * 文件 avltree 创建于 2017/11/3
 * @author 赵磊
 * @version 1.0
 */
#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdbool.h>
#include <lzma.h>
#include "../util.h"

typedef struct AvlTree_ AvlTree;

typedef void *AvlKey;

typedef void *AvlValue;

typedef struct AvlTreeEntry_ {
  AvlKey key;
  AvlValue value;
} AvlTreeEntry;

bool AvlTree_init(AvlTree **tree, ele_free kfree, ele_free vfree, compare_func key_compare);

void AvlTree_destroy(AvlTree **tree);

bool AvlTree_insert(AvlTree *tree, AvlTreeEntry *entry);

unsigned AvlTree_look_up(AvlTree *tree, AvlKey key, AvlValue *value);

bool AvlTree_get(AvlTree *tree, int index, AvlTreeEntry *entry);

bool AvlTree_delete(AvlTree *tree, AvlKey key, AvlValue *value);

void AvlTree_traverse(AvlTree *tree, void (*action)(AvlTreeEntry *entry));

unsigned int AvlTree_size(AvlTree *tree);

#endif
