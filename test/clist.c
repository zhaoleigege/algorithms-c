/**
 * 文件 clist 创建于 2017/10/20
 * @author 赵磊
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../inlclude/list/clist.h"

#define LENGTH 0

#define SIZE 6

typedef struct _page {
  int access;
  int modify;
  int page_num;
  int block_num;
} Page;

int main(void) {
  CList *page_list;
  Page *page;

  clist_init(&page_list, free);

  for (int i = 0; i < SIZE; i++) {
    page = malloc(sizeof(struct _page));
    printf("页面%d的信息: ", i + 1);
    scanf("%d %d", &page->access, &page->modify);
    page->page_num = i;
    page->block_num = (i + 1) * 2;
    clist_insert(page_list, i, page);
  }

  printf("开始置换:\n置换顺序为:\n");

  while (clist_size(page_list) > 0) {
    int index = clist_next_data(page_list, (void **) &page);
    if (!page->modify && !page->access) {
      printf("置换page->%d, block->%d\n", page->page_num, page->block_num);
      clist_remove(page_list, index, NULL);
    } else if (page->modify && !page->access) {
      page->modify = false;
    } else if (!page->modify && page->access) {
      page->modify = true;
      page->access = false;
    } else {
      page->modify = false;
    }
  }

  clist_destroy(&page_list);

  CList *list;
  int *data;

  clist_init(&list, free);

  for (int i = 0; i < LENGTH; i++) {
    data = malloc(sizeof(int));
    *data = i;
    clist_insert(list, i, data);
  }

  clist_reverse(list);

  for (int i = 0; i < LENGTH; i++) {
    clist_get(list, i, (void **) &data);
    printf("data-> %d\n", *data);
  }

  clist_destroy(&list);

  return 0;
}