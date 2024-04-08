#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // sentinel로 쓸 nil노드 초기화
  node_t* nil_node = (node_t*)calloc(1,sizeof(node_t));
  nil_node->parent=nil_node;
  nil_node->left=nil_node;
  nil_node->right=nil_node;
  nil_node->color=RBTREE_BLACK;
  // 루트노드 초기화
  p->root=nil_node;
  p->nil=nil_node;
  return p;
}

node_t *new_red_node(rbtree* t,const key_t key) {
  node_t* n = (node_t*)malloc(sizeof(node_t));
  n->key = key;
  n->left= t->nil;
  n->right= t->nil;
  n->parent= t->nil;
  n->color=RBTREE_RED;
  return n;
}

void delete_node(node_t* start,rbtree *t){    
    //nil_node면
    if(start == t->nil){
        return;
    }
    // 리프노드일 경우
    else if(start->left==t->nil && start->right==t->nil) free(start);
    // 후위순회로 삭제
    else{
        if(start->left!=t->nil)delete_node(start->left,t);
        if(start->right!=t->nil)delete_node(start->right,t);
        free(start);
    }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // 포함된 노드들 제거
  delete_node(t->root,t);
  // sentinel한 nil노드 제거
  free(t->nil);
  free(t);
}

void rotation_left(rbtree* t ,node_t* target){
  // left_child 설정
  node_t* y= target->right;
  
  // y의 왼쪽 서브트리를 오른쪽 서브트리로 붙인다.-------------
  target->right = y->left;
  // 만약 서브트리가 nil 노드가 아니라면
  // 서브트리의 부모를 target 노드로 갱신해준다.
  if(y->left != t->nil)y->left->parent=target;
  
  // target의 부모를 y로 연결한다.------------------------
  y->parent=target->parent;
  // 만약 target이 루트노드라면
  if(target->parent==t->nil)t->root = y;
  // 루트노드가 아니라면 부모의 어느쪽 자식 노드인지
  // 왼쪽이라면
  else if(target == target->parent->left) target->parent->left=y;
  // 오른쪽이라면
  else target->parent->right=y;
  
  // target을 y의 왼쪽으로 연결한다.----------------------
  y->left=target;
  target->parent=y;
}
void rotation_right(rbtree* t ,node_t* target ){
  
  node_t* y= target->left;
  
  target->left = y->right;
  if(y->right != t->nil)y->right->parent=target;
  
  y->parent=target->parent;
  if(target->parent==t->nil)t->root = y;
  else if(target == target->parent->left) target->parent->left=y;
  else target->parent->right=y;
  
  y->right=target;
  target->parent=y;
}

// DONE ----------------upper
node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  node_t* new_node = new_red_node(t,key);
  
  // 노드 추가될 자리 찾기
  node_t* x = t->root; // key 비교용 노드
  node_t* y = t->nil;  // 추가된 노드의 부모가 될 노드
  while(x!=t->nil){
    y=x;
    if (key < x->key) x=x->left;
    else x->right;
  }
  new_node->parent=y;
  
  // 만약 비어있는 트리였다면 루트로 설정
  if(y==t->nil)t->root = new_node;
  // 아니라면 key값을 통해 이동한 위치에 연결
  else if(y->key<new_node->key)y->left=new_node;
  else y->right=new_node;
  
  //추가 후 balancing
  node_t *rbtree_insert_fixup(t,new_node);
  
  return new_node;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t* target){
  


}


// TODO ---------------- under
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
