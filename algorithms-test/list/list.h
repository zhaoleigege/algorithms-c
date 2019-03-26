#ifndef LIST
#define LIST

typedef void* Element;
typedef struct _list List;
typedef void (*Ele_free)(void *data);

void init(List** list,Ele_free ele_free);

void insert(List list, int index, Element value);

void get(List list, int index, Element value);

int size(List list);

void update(List list, int index, int value);

void for_each(List list, void(*action(Element ele)));
#endif