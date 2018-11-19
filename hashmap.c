//
//  hashmap.c
//  C-library
//
//  Created by anxin on 2018/11/17.
//  Copyright © 2018 anxin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "hashmap.h"
#define MIN(A,B) ({ __typeof__(A) __a=(A); __typeof__(B) __b=(B); __a<__b ? __a:__b;})

//HashTable function
hashmap* hashmap_new(void) {
    hashmap*ht = (hashmap*) malloc(sizeof(hashmap));
    ht->cap = DEFAULT_INITIAL_CAPACITY;
    ht->load_factor = DEFAULT_LOAD_FACTOR;
    ht->threshold = ht->cap * ht->load_factor;
    ht->len = 0;
    ht->tab = (hashnode **)malloc(ht->cap*sizeof(hashnode*));
    memset(ht->tab, 0 ,ht->cap*sizeof(hashnode*));
    return ht;
};

void hashmap_destroy(hashmap* self) {
    for(int i=0; i<self->cap; ++i) {
        if(self->tab[i]){
            hashnode_destroy(self->tab[i]);
        }
    }
    free(self->tab);
    free(self);
}

int hashmap_map_size_for(int cap){
    int n = cap - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n<0?1:n>=MAXIMUM_CAPACITY?MAXIMUM_CAPACITY:n+1;
};

void hashmap_resize(hashmap* self) {
    if(!self) {
        self = hashmap_new();
    } else if(self->len>=self->threshold){
        int new_cap = self->cap << 1;
        if(new_cap <= MAXIMUM_CAPACITY){
            hashnode** new_tab =(hashnode **)malloc(new_cap*sizeof(hashnode*));
            memset(new_tab,0,new_cap*sizeof(hashnode*));
            for(int i=0;i<self->cap;++i) {
                if(!self->tab[i]) {
                    continue;
                }
                hashnode *lo = new_tab[i];
                hashnode *hi = new_tab[i+self->cap];
                while(lo&&lo->next){
                    lo = lo->next;
                }
                while(hi &&hi->next) {
                    hi = hi->next;
                }
                
                hashnode *cur = self->tab[i];
                do {
                    int new_loc = cur->hash & (new_cap - 1);
                    if(new_loc == i) {  //low location
                        if(!lo){
                            new_tab[new_loc] = cur;
                        } else {
                            lo->next = cur;
                        }
                        lo = cur;
                    } else { //high location
                        if(!hi){
                            new_tab[new_loc] = cur;
                        } else {
                            hi->next = cur;
                        }
                        hi = cur;
                    }
                    hashnode* pre = cur;
                    cur = cur->next;
                    pre->next = NULL;
                } while(cur);
            }
            self->tab = new_tab;
            self->cap = new_cap;
            self->threshold = new_cap*self->load_factor;
        }
    }
};

void hashmap_put(hashmap* self, char* key, int value) {
    // if not enough room for new key, resize
    if(!self) {
        hashmap_resize(self);
    } else if(self->len+1>self->threshold) {
        hashmap_resize(self);
    }
    
    int loc = (self->cap - 1) & hashnode_hash(key);
    hashnode* cur = self->tab[loc];
    hashnode* to_append = hashnode_new(key,value);
    
    if(!cur) {  //empty list
        self->tab[loc] = to_append;
    } else {
        hashnode* pre=NULL;
        while(cur) {
            if(!hashnode_cmp(cur->key,key)){
                cur->value = value;
                return;
            }
            pre = cur;
            cur = cur->next;
        }
        pre->next = to_append;
    }
    self->len +=1;
};

void hashmap_delete(hashmap* self, char* key) {
    if(!self) {
        abort();
    }
    int loc = (self->cap - 1) & hashnode_hash(key);
    if(!self->tab[loc]){
        abort();
    }
    hashnode* cur = self->tab[loc];
    
    if(!hashnode_cmp(cur->key,key)) {
        self->tab[loc] = cur->next;
    } else {
        hashnode* pre = cur;
        cur = cur->next;
        while(cur){
            if(!hashnode_cmp(cur->key,key)) {
                pre->next = cur->next;
                break;
            }
            pre = cur;
            cur = cur->next;
        }
    }
    
    if(cur) {  //find
        free(cur);
        self->len-=1;
    } else {
        abort();
    }
};

int hashmap_contains(hashmap* self, char* key) {
    if(!self) {
        return -1;
    }
    int loc = (self->cap - 1) & hashnode_hash(key);
    if(!self->tab[loc]){
        return -1;
    }
    hashnode* cur = self->tab[loc];
    while(cur){
        if(!hashnode_cmp(cur->key,key)) {
            return 0;
        }
        cur = cur->next;
    }
    return -1;
};

int hashmap_get(hashmap*self, char*key) {
    if(!self) {
        abort();
    }
    int loc = (self->cap - 1) & hashnode_hash(key);
    if(!self->tab[loc]){
        abort();
    }
    hashnode* cur = self->tab[loc];
    while(cur){
        if(!hashnode_cmp(cur->key,key)) {
            return cur->value;
        }
        cur = cur->next;
    }
    abort();
}

//Node function
hashnode* hashnode_new(char *key,int value) {
    hashnode* node = (hashnode*) malloc(sizeof(hashnode));
    size_t len = strlen(key);
    node->key = (char*)malloc(sizeof(char)*len);
    strcpy(node->key,key);
    node->value = value;
    node->hash = hashnode_hash(key);
    node->next = NULL;
    return node;
}

void hashnode_destroy(hashnode *self) {
    while(self) {
        hashnode *next = self->next;
        free(self);
        self = next;
    }
}

int hashnode_hash(char *key){
    int hash = 0;
    char *src = key;
    while (*src) {
        hash = (hash<<5)-hash + *src;
        ++src;
    }
    return hash^(hash>>16);
}

int hashnode_cmp(char*p1, char*p2){
    char *s1 = p1;
    char *s2 = p2;
    char c1,c2;
    do {
        c1 = *s1++;
        c2 = *s2++;
        if('\0'==c1) {
            return c1-c2;
        }
    } while(c1==c2);
    return c1-c2;
}
