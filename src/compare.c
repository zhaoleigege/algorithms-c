/**
 * 文件 compare 创建于 2017/10/17
 * @author 赵磊
 * @version 1.0
 */
#include <string.h>
#include "../inlclude/compare.h"

int int_compare(const void *v1, const void *v2) {
  return *(int *) v1 - *(int *) v2;
}

int string_compare(const void *v1, const void *v2) {
  return strcmp(v1, v2);
}