/**
 * 文件 testListHash 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inlclude/hashpjw.h"
#include "../inlclude/hash/list_hash.h"

int match(const void *key1, const void *key2) {
  return strcmp((char *) key1, (char *) key2);
}

void strings_show(void *data) {
  char *string = data;

  printf("%s\n", string);
}

int main(void) {
  CHTbl *hash;
  char string[4][10] = {
      {"hello"},
      {"world"},
      {"who you"},
      {"love love"}
  };

  chtbl_init(&hash, 8, hashpjw, match, NULL);

  chtbl_insert(hash, string[0]);
  chtbl_insert(hash, string[1]);
  chtbl_insert(hash, string[2]);
  chtbl_insert(hash, string[3]);

  chtbl_foreach(hash, strings_show);

  chtbl_destroy(&hash);

  return 0;
}