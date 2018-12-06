/**
 * 文件 avltree 创建于 2017/11/3
 * @author 赵磊
 * @version 1.0
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../../inlclude/tree/avltree.h"

typedef enum {
  AVL_LFT_HEAVY = 2,
  AVL_RGT_HEAVY = -2
} AvlTreeNodeFactor;

struct node {
  AvlTreeEntry *data;
  unsigned int size;

  struct node *left;
  struct node *right;

  int height;                                         //此节点到叶子节点的高度(从1开始)
};

struct AvlTree_ {
  struct node *root;

  ele_free kfree;
  ele_free vfree;

  compare_func keyCompare;
};

#define HEIGHT(node)                                                                      \
        (((node)->left == NULL && (node)->right == NULL) ? ((node)->height++): ((node)->height))

/**
 * 计算此节点的平衡因子
 * @param node
 * @return
 */
static inline int get_factor(struct node *node) {
  return (node->left == NULL ? 0 : node->left->height) - (node->right == NULL ? 0 : node->right->height);
}

/**
 * 返回节点个数
 * @param node
 * @return
 */
static inline unsigned int node_size(struct node *node) {
  if (node == NULL)
    return 0;

  return node->size;
}

/**
 * 返回此节点的高度
 * @param node
 * @return
 */
static inline int node_height(struct node *node) {
  return node == NULL ? 0 : node->height;
}

/**
 * 计算此节点的高度
 * @param node
 * @return
 */
static inline int calcu_height(struct node *node) {
  int lheight = node_height(node->left);
  int rheight = node_height(node->right);

  return lheight > rheight ? lheight + 1 : rheight + 1;
}

bool AvlTree_init(AvlTree **tree, ele_free kfree, ele_free vfree, compare_func kcompare) {
  if (kfree == NULL || vfree == NULL || kcompare == NULL)
    return false;

  *tree = malloc(sizeof(struct AvlTree_));
  if ((*tree) == NULL)
    return false;

  (*tree)->kfree = kfree;
  (*tree)->vfree = vfree;
  (*tree)->keyCompare = kcompare;

  (*tree)->root = NULL;

  return true;
}

void AvlTree_destroy(AvlTree **tree) {

}

/**
 * LL旋转
 * @param node
 * @return
 */
static inline struct node *left_left_rotate(struct node *node) {
  struct node *lnode = node->left;

  node->left = lnode->right;
  lnode->right = node;

  node->height = lnode->left->height;

  return lnode;
}

/**
 * LR旋转
 * @param node
 * @return
 */
static inline struct node *left_right_rotate(struct node *node) {
  struct node *lnode = node->left;
  struct node *grnode = lnode->right;

  lnode->right = grnode->left;
  grnode->left = lnode;
  grnode->height = lnode->height + 1;

  node->left = grnode->right;
  grnode->right = node;
  node->height = lnode->height;

  return grnode;
}

/**
 * RR旋转
 * @param node
 * @return
 */
static inline struct node *right_right_rotate(struct node *node) {
  struct node *rnode = node->right;

  node->right = rnode->left;
  rnode->left = node;

  node->height = rnode->right->height;

  return rnode;
}

/**
 * RL旋转
 * @param node
 * @return
 */
static inline struct node *right_left_rotate(struct node *node) {
  struct node *rnode = node->right;
  struct node *grnode = rnode->left;

  rnode->left = rnode->right;
  grnode->right = rnode;
  grnode->height = rnode->height + 1;

  node->right = grnode->left;
  grnode->left = node;
  node->height = rnode->height;

  return rnode;
}

/**
 * 添加节点，节点添加完后先改变高度值再进行旋转判断
 * @param tree
 * @param node
 * @param entry
 * @return
 */
static struct node *insert(AvlTree *tree, struct node *node, AvlTreeEntry *entry) {
  if (node == NULL) {
    node = malloc(sizeof(struct node));
    if (node == NULL)
      return NULL;

    node->data = entry;
    node->size = 0;

    node->right = NULL;
    node->left = NULL;

    node->height = 1;

    return node;
  }

  struct node *temp = NULL;

  int cmp = tree->keyCompare(entry->key, node->data->key);

  if (cmp == 0) {
    if (entry->value == node->data->value)
      return node;

    tree->vfree(node->data->value);
    node->data->value = entry->value;

    return node;
  } else if (cmp > 0) {
    temp = insert(tree, node->right, entry);
    if (temp == NULL)
      return NULL;

    node->right = temp;
    node->size = node_size(node->left) + node_size(node->right) + 1;

    if (temp->height == 1)
      HEIGHT(node);
    else
      node->height++;

    switch (get_factor(node)) {
      case AVL_LFT_HEAVY:                 //RL旋转
        node = right_left_rotate(node);
        break;
      case AVL_RGT_HEAVY:                 //RR旋转
        node = right_right_rotate(node);
        break;
      default:break;
    }
  } else {
    temp = insert(tree, node->left, entry);
    if (temp == NULL)
      return NULL;

    node->left = temp;
    node->size = node_size(node->left) + node_size(node->right) + 1;

    if (temp->height == 1)
      HEIGHT(node);
    else
      node->height++;

    switch (get_factor(node)) {
      case AVL_LFT_HEAVY:                   //LL旋转
        node = left_left_rotate(node);
        break;
      case AVL_RGT_HEAVY:                   //LR旋转
        node = left_right_rotate(node);
        break;
      default:break;
    }
  }

  return node;
}

bool AvlTree_insert(AvlTree *tree, AvlTreeEntry *entry) {
  struct node *node = insert(tree, tree->root, entry);
  if (node == NULL)
    return false;

  tree->root = node;

  return true;
}

static inline struct node *left_delete(struct node *node) {
  int factor = get_factor(node);

  if (factor == AVL_RGT_HEAVY) {
    if (get_factor(node->right) != 1)
      return right_right_rotate(node);
    else
      return right_left_rotate(node);
  }

  return node;
}

static inline struct node *right_delete(struct node *node) {
  int factor = get_factor(node);

  if (factor == AVL_LFT_HEAVY) {
    if (get_factor(node->left) != -1)
      return left_left_rotate(node);
    else
      return left_right_rotate(node);
  }

  return node;
}

static struct node *delete_min(struct node *node, struct node **dnode) {
  if (node->left != NULL) {
    node->left = delete_min(node->left, dnode);
    node->size = node_size(node->left) + node_size(node->right) + 1;

    return left_delete(node);
  }

  *dnode = node;
  return node->right;
}

static struct node *delete(struct node *node, AvlKey key, compare_func compare, struct node **dnode) {
  if (node == NULL)
    return NULL;

  int cmp = compare(key, node->data->key);

  if (cmp == 0) {
    *dnode = node;

    if (node->right != NULL) {
      struct node *replace;
      node->right = delete_min(node->right, &replace);

      replace->left = node->left;
      replace->right = node->right;

      replace->size = node_size(node->left) + node_size(node->right) + 1;

      return right_delete(replace);
    }

    return node->left;
  } else if (cmp > 0) {
    node->right = delete(node->right, key, compare, dnode);
    node->size = node_size(node->left) + node_size(node->right) + 1;

    return right_delete(node);
  } else {
    node->left = delete(node->left, key, compare, dnode);

    node->size = node_size(node->left) + node_size(node->right) + 1;

    return left_delete(node);
  }
}

bool AvlTree_delete(AvlTree *tree, AvlKey key, AvlValue *value) {
  if (key == NULL)
    return false;

  struct node *node = NULL;

  tree->root = delete(tree->root, key, tree->keyCompare, &node);

  if (node == NULL)
    return false;

  if (key != node->data->key)
    tree->kfree(node->data->key);

  if (value == NULL)
    tree->vfree(node->data->value);
  else
    *value = node->data->value;

  node->right = NULL;
  node->left = NULL;
//  free(node->data);

  return true;
}

static void in_order(struct node *node, void (*action)(AvlTreeEntry *entry)) {
  if (node == NULL)
    return;

  in_order(node->left, action);
  action(node->data);
  in_order(node->right, action);
}

void AvlTree_traverse(AvlTree *tree, void (*action)(AvlTreeEntry *entry)) {
  in_order(tree->root, action);
}
