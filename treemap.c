#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)){
  TreeMap* map=(TreeMap*)malloc(sizeof(TreeMap));

  map->lower_than=lower_than;
  map->root=NULL;
  //new->lower_than = lower_than;
  return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value){
  TreeNode* current=tree->root;
  TreeNode* newNode=createTreeNode(key, value);

  while(current!=NULL){
    if(is_equal(tree, key, current->pair->key)){
      return;
    }else if(tree->lower_than(key, current->pair->key)){
      if (current->left==NULL){
        current->left=newNode;
        newNode->parent=current;
        tree->current=newNode;
        return;
      }else{
        current=current->left;
      }
    }else{
      if (current->right==NULL){
        current->right=newNode;
        newNode->parent=current;
        tree->current=newNode;
        return;
      }else{
        current=current->right;
      }
    }
  }
}

TreeNode * minimum(TreeNode * x){
  while (x->left!=NULL){
    x=x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node){

  if(node->left==NULL && node->right==NULL){
    if(node->parent->left==node){
      node->parent->left=NULL;
    }else if(node->parent->right==node){
      node->parent->right=NULL;
    }

  }else if(node->left==NULL || node->right==NULL){
    TreeNode* child;
    if(node->left!=NULL){
      child=node->left;
    }else{
      child=node->right;
    }

    if(node->parent->left==node){
      node->parent->left=child;
    }else{
      node->parent->right=child;
    }
    child->parent=node->parent;
  }else{
    TreeNode* succesor=minimum(node->right);
    node->pair->key=succesor->pair->key;
    node->pair->value=succesor->pair->value;
    removeNode(tree, succesor);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key){
  if (tree==NULL || tree->root==NULL){
    return NULL;
  }
  TreeNode* current=tree->root;
  while (current!=NULL){
    if (is_equal(tree, key, current->pair->key)){
      tree->current=current;
      return tree->current->pair; 
    } else if(tree->lower_than(key, current->pair->key)){
      current=current->left;
    } else{
      current=current->right;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key){
  if(tree==NULL || tree->root==NULL){
    return NULL;
  }

  TreeNode* current=tree->root;
  TreeNode* upperBound=NULL;

  while(current!=NULL){
    if(is_equal(tree, key, current->pair->key)){
      return current->pair;
    }else if(tree->lower_than(key, current->pair->key)){
      upperBound=current;
      current=current->left;
    }else{
      current=current->right;
    }
  }
  if (upperBound!=NULL){
    return upperBound->pair;
  }else{
    return NULL;
  }
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree){
  TreeNode *current = minimum(tree->root);
  tree->current = current;
  return current->pair;
}

Pair * nextTreeMap(TreeMap * tree){
  TreeNode* current = tree->current;
  if (current == NULL) return NULL;
  
  if (current->right != NULL){
    current = minimum(current->right);
  } 
  else{
    TreeNode* parent = current->parent;
    while (current->parent != NULL && current ==  current->parent->right){
      current = parent;
      parent = parent->parent;
    }
    current =  current->parent;
  }
  tree->current = current;
  
  if (current != NULL) return current->pair;
  return NULL;
}
