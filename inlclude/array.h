/**
 * 文件 array 创建于 2017/10/19
 * @author 赵磊
 * @version 1.0
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
typedef void *arrType;

typedef struct _array {
  int length;
  void (*arr_free)(arrType *value);
  arrType value[1];
} Array;

bool array_init(Array **array, int length, void (*arr_free)(arrType *value));

void rem_aval(Array array, int index);

#endif
