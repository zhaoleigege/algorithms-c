#ifndef LIST
#define LIST

typedef void (*data_free)(void *data);

typedef struct _list List;

void list_init(List **list, void (*data_free)(void *));

void list_destroy(List **list);

void list_insert(List *list, void *data);

int list_is_circle(List *list);

#endif