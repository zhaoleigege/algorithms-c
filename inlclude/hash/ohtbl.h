/**
 * 文件 ohtbl 创建于 2017/10/27
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef OHTBL_H
#define OHTBL_H

typedef struct OHTbl_ OHTbl;

bool ohtbl_init(
    OHTbl **ohtbl, unsigned int capacity                                     //m代表表中槽位的个数
    , unsigned int (*hash)(const void *key)                                  //hash分别代表哈希函数
    , int (*match)(const void *k1, const void *k2)                  //match用来判断两个健是否相等
    , void (*key_free)(void *key)                                   //用来释放健的方法
    , void (*value_free)(void *value)                               //用来释放值的方法
);

void ohtbl_destroy(OHTbl **ohtbl);

bool ohtbl_insert(OHTbl *ohTbl, void *key, void *value);

bool ohtbl_remove(OHTbl *ohTbl, void *key, void **value);

bool ohtbl_lookup(OHTbl *ohTbl, void *key, void **value);

unsigned int ohtbl_size(const OHTbl *ohTbl);

void ohtbl_foreach(const OHTbl *ohTbl, void (*action)(const void *key, const void *value));

#endif
