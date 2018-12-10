//
//  llist.c
//  C-library
//
//  Created by anxin on 2018/11/19.
//  Copyright © 2018 anxin. All rights reserved.
//

#include "llist.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct llist_node {
    void *data;
    struct llist_node * next;
} llist_node;

llist *llist_create(void) {
    return calloc(1,sizeof(llist));
}

void llist_destroy(llist *llist){
    llist_node* cur = llist->head;
    while(cur){
        llist_node *next = cur->next;
        free(cur);
        cur = next;
    }
    free(llist);
}

void *llist_push_front(llist *llist, void *data) {
    llist_node* new_node = calloc(1,sizeof(llist_node));
    new_node->data = data;
    new_node->next = llist->head;
    llist->head = new_node;
    llist->count++;
    return new_node;
}

void *llist_push_back(llist *llist, void *data) {
    llist_node* new_node = calloc(1,sizeof(llist_node));
    llist_node* cur = llist->head;
    if(!cur) {
        llist->head = new_node;
    } else {
        while(cur->next) {
            cur = cur->next;
            cur->next = new_node;
        }
    }
    llist->count++;
    return new_node;
}

void *llist_head(llist *llist) {
    if(!llist->head) return NULL;
    return llist->head->data;
}

void *llist_tail(llist *llist) {
    llist_node *cur = llist->head;
    if(!cur) return NULL;
    while(cur->next) {
        cur=cur->next;
    }
    return cur->data;
}

void *llist_find(llist *llist, void *data, int (*cmpfn)(void *, void *)) {
    llist_node *cur = llist->head;
    while(cur) {
        if(cmpfn(cur->data,data)==0) {
            break;
        }
        cur = cur->next;
    }
    if(!cur) {
        return NULL;
    }
    return cur->data;
}

// do not delete data, only delete node;
void *llist_delete( llist *llist, void *data, int (*cmpfn)(void *, void *)) {
    llist_node *cur = llist->head;
    llist_node *pre = NULL;
    while(cur) {
        if(cmpfn(cur->data,data)==0) {
            void *data = cur->data;
            if(cur == llist->head) {
                llist->head = cur->next;
                free(cur);
            } else {
                pre->next = cur->next;
                free(cur);
            }
            llist->count--;
            return data;
        }
        pre = cur;
        cur = cur->next;
    }
    return NULL;
}

int llist_count( llist *llist) {
    return llist->count;
}

void llist_foreach(llist *llist, void (*f)(void *, void *), void *arg) {
    llist_node *cur = llist->head;
    while(cur) {
        f(cur->data,arg);
        cur = cur->next;
    }
}
