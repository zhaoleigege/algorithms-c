/**
 * 文件 hashpjw 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include "../inlclude/hashpjw.h"

#define PRIME_TBLSIZ 1024

unsigned int hashpjw(const void *key) {
  const char *ptr;
  unsigned int val;

  val = 0;
  ptr = key;

  while (*ptr != '\0') {
    unsigned int tmp;
    val = (val << 4) + *ptr;

    if ((tmp = (val & 0xF0000000))) {
      val = val ^ (tmp >> 24);
      val = val ^ tmp;
    }

    ptr++;
  }

  return val % PRIME_TBLSIZ;
}