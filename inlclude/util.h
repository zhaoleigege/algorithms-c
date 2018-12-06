/**
 * 文件 util 创建于 2017/11/3
 * @author 赵磊
 * @version 1.0
 */
#ifndef UTIL_H
#define UTIL_H

typedef int (*compare_func)(const void *v1, const void *v2);

typedef void (*ele_free)(void *data);

typedef void (*key_free)(void *key);
typedef void (*value_free)(void *value);

#endif
