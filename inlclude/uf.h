/**
 * 文件 uf 创建于 2017/11/12
 * @author 赵磊
 * @version 1.0
 */

#include <stdbool.h>
#ifndef UF_H
#define UF_H

typedef struct uf_ UF;

bool uf_init(UF **uf, int size);

void uf_union(UF *uf, int p, int q);

bool uf_connected(UF *uf, int p, int q);

int uf_count(UF *uf);

void uf_destroy(UF **uf);

void uf_traverse(UF *uf, void (*action)(int index, int count));
#endif
