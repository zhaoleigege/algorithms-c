/**
 * 文件 bitree 创建于 2017/10/28
 * @author 赵磊
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "../../inlclude/tree/bitree.h"
#include "../../inlclude/list/stack.h"

struct node_ {
  void *data;

  struct node_ *lnode;
  struct node_ *rnode;

  unsigned int size;
};

struct BiTree_ {
  struct node_ *root;

  int (*compare)(const void *v1, const void *v2);
  void (*ele_free)(void *ele);
};

bool bitree_init(BiTree **tree, void (*ele_free)(void *data), int (*compare)(const void *v1, const void *v2)) {
  if (ele_free == NULL || compare == NULL) {
    return false;
  }

  *tree = malloc(sizeof(struct BiTree_));

  if ((*tree) == NULL)
    return false;

  (*tree)->root = NULL;
  (*tree)->ele_free = ele_free;
  (*tree)->compare = compare;

  return true;
}

static void post_delete(struct node_ *node, void (*ele_free)(void *data)) {
  if (node == NULL)
    return;

  post_delete(node->lnode, ele_free);
  post_delete(node->rnode, ele_free);
  ele_free(node->data);
  node->lnode = NULL;
  node->rnode = NULL;
  free(node);
}

void bitree_destroy(BiTree **tree) {
  post_delete((*tree)->root, (*tree)->ele_free);
  (*tree)->ele_free = NULL;
  (*tree)->compare = NULL;
  free(*tree);
}

static inline unsigned int bit_node_size(struct node_ *node) {
  if (node == NULL)
    return 0;

  return node->size;
}

static struct node_ *tree_ins(struct node_ *node, void *data, BiTree *tree) {
  if (node == NULL) {
    node = malloc(sizeof(struct node_));
    node->size = 1;
    node->data = data;
    node->lnode = NULL;
    node->rnode = NULL;

    return node;
  }

  int com = tree->compare(data, node->data);

  if (com == 0) {
    tree->ele_free(node->data);
    node->data = data;
  } else if (com > 0) {
    node->rnode = tree_ins(node->rnode, data, tree);
    node->size = bit_node_size(node->lnode) + bit_node_size(node->rnode) + 1;
  } else if (com < 0) {
    node->lnode = tree_ins(node->lnode, data, tree);
    node->size = bit_node_size(node->lnode) + bit_node_size(node->rnode) + 1;
  }

  return node;
}

bool bitree_insert(BiTree *tree, void *data) {
  tree->root = tree_ins(tree->root, data, tree);

  return true;
}

/**
 * 先序遍历
 * @param node
 * @param action
 */
static void pre_order(struct node_ *node, void (*action)(void *data)) {
  if (node == NULL)
    return;

  action(node->data);
  pre_order(node->lnode, action);
  pre_order(node->rnode, action);
}

/**
 * 中序遍历
 * @param node
 * @param action
 */
static void in_order(struct node_ *node, void (*action)(void *data)) {
  if (node == NULL)
    return;

  in_order(node->lnode, action);
  action(node->data);
  in_order(node->rnode, action);
}

/**
 * 后序遍历
 * @param node
 * @param action
 */
static void post_order(struct node_ *node, void (*action)(void *data)) {
  if (node == NULL)
    return;

  post_order(node->lnode, action);
  post_order(node->rnode, action);
  action(node->data);
}

/**
 * 层次遍历
 * @param node
 * @param action
 */
static void level_order(struct node_ *node, void (*action)(void *data)) {
  if (node == NULL)
    return;

  struct node_ *temp;
  Stack *stack;

  stack_init(&stack, free);
  stack_push(stack, node);

  while (stack_size(stack) > 0) {
    stack_pop(stack, (void **) &temp);
    action(temp->data);

    if (temp->lnode != NULL)
      stack_push(stack, temp->lnode);

    if (temp->rnode != NULL)
      stack_push(stack, temp->rnode);
  }

  stack_destroy(&stack);
}

void bitree_traverse(BiTree *tree, void (*action)(void *data)) {
  if (action == NULL)
    return;

  in_order(tree->root, action);
}

static unsigned tree_node_look_up(struct node_ *node, void *data, BiTree *tree, size_t size) {
  if (node == NULL)
    return 0;

  int com = tree->compare(data, node->data);

  if (com == 0) {
    memcpy(data, node->data, size);
    return bit_node_size(node->lnode) + 1;
  } else if (com > 0) {
    unsigned index = tree_node_look_up(node->rnode, data, tree, size);

    if (index != 0)
      return index + bit_node_size(node->lnode) + 1;
  } else if (com < 0) {
    return tree_node_look_up(node->lnode, data, tree, size);
  }

  return 0;
}

unsigned bitree_look_up(BiTree *tree, void *data, size_t size) {
  return tree_node_look_up(tree->root, data, tree, size);
}

bool bitree_get(BiTree *tree, int index, void *data, size_t size) {
  if (index < 1 || index > tree->root->size)
    return false;

  struct node_ *node = tree->root;
  while (1) {
    int gap = index - bit_node_size(node->lnode) - 1;
    if (gap == 0) {
      memcpy(data, node->data, size);
      return true;
    } else if (gap > 0) {
      index = gap;
      node = node->rnode;
    } else {
      node = node->lnode;
    }
  }
}

static struct node_ *delete_min(struct node_ *node, struct node_ **min) {
  if (node->lnode != NULL) {
    delete_min(node->lnode, min);
    node->size = bit_node_size(node->lnode) + bit_node_size(node->rnode) + 1;
    return node;
  }

  *min = node;
  node->size = 1;
  return node->rnode;
}

static struct node_ *delete(struct node_ *node,
                            void **data,
                            int (*compare)(const void *v1, const void *v2),
                            struct node_ **dnode) {
  if (node == NULL)
    return NULL;

  int com = compare(*data, node->data);

  if (com == 0) {
    *dnode = node;

    if (node->rnode != NULL) {
      struct node_ *replace;
      node->rnode = delete_min(node->rnode, &replace);
      replace->rnode = node->rnode;
      replace->lnode = node->lnode;
      replace->size = bit_node_size(replace->rnode) + bit_node_size(replace->lnode) + 1;
      return replace;
    } else {
      return node->lnode;
    }
  } else if (com > 0) {
    node->rnode = delete(node->rnode, data, compare, dnode);
    node->size = bit_node_size(node->rnode) + bit_node_size(node->lnode) + 1;
    return node;
  } else {
    node->lnode = delete(node->lnode, data, compare, dnode);
    node->size = bit_node_size(node->rnode) + bit_node_size(node->lnode) + 1;
    return node;
  }
}

bool bitree_delete(BiTree *tree, void **data) {
  struct node_ *dnode = NULL;
  tree->root = delete(tree->root, data, tree->compare, &dnode);
  if (dnode == NULL)
    return false;

  void *temp = dnode->data;
  dnode->data = *data;
  *data = temp;

  tree->ele_free(dnode->data);
  dnode->rnode = NULL;
  dnode->lnode = NULL;
  free(dnode);

  return true;
}

unsigned int bitree_size(BiTree *tree) {
  return tree->root->size;
}