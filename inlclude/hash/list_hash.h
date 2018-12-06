/**
 * 文件 lish_hash 创建于 2017/10/20
 * 链式哈希表
 * @author 赵磊
 * @version 1.0
 */

#ifndef LIST_HASH_H
#define LIST_HASH_H

#include "../list/list.h"

/* chained hash tabls */
typedef struct CHTbl_ CHTbl;

bool chtbl_init(CHTbl **htbl,
                int buckets,
                unsigned int (*h)(const void *key),
                int (*match)(const void *key1, const void *key2),
                void (*destroy)(void *data)
);

void chtbl_destroy(CHTbl **htbl);

bool chtbl_insert(CHTbl *htbl, void *data);

bool chtbl_remove(CHTbl *htbl, void **data);

int chtbl_lookup(const CHTbl *htbl, void **data);

unsigned int chtbl_size(const CHTbl *htbl);

void chtbl_foreach(const CHTbl *htbl, void (*action)(void *data));

#endif
