#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) 
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->right){
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void del_node(rbtree *t, node_t *n){
  if(n != t->nil){
    del_node(t, n->left);
    del_node(t, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t){
  del_node(t, t->root);
  free(t->nil);
  free(t);
}

void RB_INSERT_FIXUP(rbtree *t, node_t *new_node){
  node_t *tmp;
  while (new_node->parent->color == RBTREE_RED){
    if (new_node->parent == new_node->parent->parent->left){
      tmp = new_node->parent->parent->right; // uncle 노드

      // case 1
      if (tmp->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      // case 2
      else { 
        if (new_node == new_node->parent->right){
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
      // case 3
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    }
    else {
      tmp = new_node->parent->parent->left;

        // case 1
        if (tmp->color == RBTREE_RED){
          new_node->parent->color = RBTREE_BLACK;
          tmp->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          new_node = new_node->parent->parent;
        }
        // case 2
        else {
          if (new_node == new_node->parent->left){
            new_node = new_node->parent;
            right_rotate(t, new_node);
          }
        // case 3
          new_node->parent->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          left_rotate(t, new_node->parent->parent);
        }
      }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = malloc(sizeof(node_t));
  node_t *x = t->root;
  node_t *y = t->nil;
  new_node->key = key;
 
  while (x != t->nil){
    y = x;
    if (new_node->key < y->key){
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  new_node->parent = y;
  if (y == t->nil){
    t->root = new_node;
  }
  else if (new_node->key < y->key){
    y->left = new_node;
  }
  else {
    y->right = new_node;
    }
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  RB_INSERT_FIXUP(t, new_node);
return new_node;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *tmp = t->root; 
    while(tmp != t->nil && tmp->key != key) { 
      if (tmp->key > key) { 
        tmp = tmp->left; 
      }
      else{
        tmp = tmp->right;
      }
    }
    
    if (tmp->key == key){ 
        return tmp;
        }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *node_min(const rbtree *t, node_t *n) {
  node_t *ptr;
  ptr = n;
  while(ptr->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while (x->right != t->nil){
    x = x->right;
  }
  return x;
}

void RB_TRNASPLANT(rbtree *t, node_t *u, node_t *v){
  v->parent = u->parent;
  // root
  if (u->parent == t->nil){
    t->root = v;
  }
  // left
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  // right (u->parent->right == u 생략)
  else {
    u->parent->right = v;
  }
  // v->parent = u->parent;
}

void RB_ERASE_FIXUP(rbtree *t, node_t *target){
  node_t *u;
  while(target != t->root && target->color == RBTREE_BLACK){
    if(target->parent->left == target){
      u = target->parent->right;
      //case1: uncle is red
      if(u->color == RBTREE_RED){
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_rotate(t, target->parent);
        u = target->parent->right;
      }
      //case2: uncle is black with two black child
      if(u->left->color == RBTREE_BLACK && u->right->color == RBTREE_BLACK){
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      else { 
        //case3: uncle is black with red color left child and black color right child
        if(u->right->color == RBTREE_BLACK){
          u->color = RBTREE_RED;
          u->left->color = RBTREE_BLACK;
          right_rotate(t, u);
          u = target->parent->right;
        }
        //case4: uncle is black with red color right child
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }
    else{
      u = target->parent->left;
      //case1: uncle is red
      if(u->color == RBTREE_RED){
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
        u = target->parent->left;
      }
      //case2: uncle is black with two black child
      if(u->right->color == RBTREE_BLACK && u->left->color == RBTREE_BLACK){
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      else { 
        //case3: uncle is black with red color right child and black color left child
        if(u->left->color == RBTREE_BLACK){
          u->color = RBTREE_RED;
          u->right->color = RBTREE_BLACK;
          left_rotate(t, u);
          u = target->parent->left;
        }
        //case4: uncle is black with red color left child
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(p == NULL){
    return 0;
  }
  node_t *y= p;
  node_t *ptr;
  color_t y_original_color = p->color;

  // case1 : 왼쪽 노드가 nil일 때
  if (p->left == t->nil){
    ptr = p->right;
    RB_TRNASPLANT(t, p, p->right);
  }
  // case2 : 오른쪽 노드가 nil일 때
  else if (p->right == t->nil){
    ptr = p->left;
    RB_TRNASPLANT(t, p, p->left);
  }
  // case3 : 왼쪽, 오른쪽 둘 다 노드일 때 (not nil)
  else {
    y = node_min(t, p->right);
    y_original_color = y->color;
    ptr = y->right;
    // case1 : p의 자식이 y일 때
    if (y->parent == p){
      ptr->parent = y;
    }
    else {
      // 그 외 케이스 : y의 부모노드를 y->right와 연결시킨다.
        RB_TRNASPLANT(t, y, y->right);
        // p의 오른쪽 노드와 y를 연결시킨다.
        p->right->parent = y;
        y->right = p->right;
      }
      // p의 부모노드를 y와 연결시킨다.
      RB_TRNASPLANT(t, p, y);
      // p의 왼쪽노드를 y와 연결시킨다.
      y->left = p->left;
      p->left->parent = y;
      y->color = p->color;
    }
  if (y_original_color == RBTREE_BLACK){
    RB_ERASE_FIXUP(t, ptr);
  }
  free(p);
  // p = NULL;
  return 0;
}

int inorder_array(node_t *p, key_t *arr, const rbtree *t, int i){
  if (p == t->nil) {
    return i;
  }
  i = inorder_array(p->left, arr, t, i);
  arr[i++] = p->key;
  i = inorder_array(p->right, arr, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  // int i = 0;
  node_t *tmp = t->root;
  inorder_array(tmp, arr, t, 0);
  return 0;
}





