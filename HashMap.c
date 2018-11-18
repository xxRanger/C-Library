//
//  HashMap.c
//  C-library
//
//  Created by anxin on 2018/11/17.
//  Copyright © 2018 anxin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "HashMap.h"
#define MIN(A,B) ({ __typeof__(A) __a=(A); __typeof__(B) __b=(B); __a<__b ? __a:__b;})

//HashTable function
HashMap* HashMap_New(void) {
    HashMap*ht = (HashMap*) malloc(sizeof(HashMap));
    ht->cap = DEFAULT_INITIAL_CAPACITY;
    ht->load_factor = DEFAULT_LOAD_FACTOR;
    ht->threshold = ht->cap * ht->load_factor;
    ht->len = 0;
    ht->tab = (HashNode **)malloc(ht->cap*sizeof(HashNode*));
    memset(ht->tab, 0 ,ht->cap*sizeof(HashNode*));
    return ht;
};

void HashMap_Destroy(HashMap* self) {
    for(int i=0; i<self->cap; ++i) {
        if(self->tab[i]){
            HashNode_Destroy(self->tab[i]);
        }
    }
    free(self->tab);
    free(self);
}

int HashMap_MapSizeFor(int cap){
    int n = cap - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n<0?1:n>=MAXIMUM_CAPACITY?MAXIMUM_CAPACITY:n+1;
};

void HashMap_Resize(HashMap* self) {
    if(!self) {
        self = HashMap_New();
    } else if(self->len>=self->threshold){
        int new_cap = self->cap << 1;
        if(new_cap <= MAXIMUM_CAPACITY){
            HashNode** new_tab =(HashNode **)malloc(new_cap*sizeof(HashNode*));
            memset(new_tab,0,new_cap*sizeof(HashNode*));
            for(int i=0;i<self->cap;++i) {
                if(!self->tab[i]) {
                    continue;
                }
                HashNode *lo = new_tab[i];
                HashNode *hi = new_tab[i+self->cap];
                while(lo&&lo->next){
                    lo = lo->next;
                }
                while(hi &&hi->next) {
                    hi = hi->next;
                }
                
                HashNode *cur = self->tab[i];
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
                    HashNode* pre = cur;
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

void HashMap_Put(HashMap* self, char* key, int value) {
    // if not enough room for new key, resize
    if(!self) {
        HashMap_Resize(self);
    } else if(self->len+1>self->threshold) {
        HashMap_Resize(self);
    }
    
    int loc = (self->cap - 1) & HashNode_Hash(key);
    HashNode* cur = self->tab[loc];
    HashNode* to_append = HashNode_New(key,value);
    
    if(!cur) {  //empty list
        self->tab[loc] = to_append;
    } else {
        HashNode* pre=NULL;
        while(cur) {
            if(!HashNode_KeyCmp(cur->key,key)){
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

void HashMap_Delete(HashMap* self, char* key) {
    if(!self) {
        abort();
    }
    int loc = (self->cap - 1) & HashNode_Hash(key);
    if(!self->tab[loc]){
        abort();
    }
    HashNode* cur = self->tab[loc];
    
    if(!HashNode_KeyCmp(cur->key,key)) {
        self->tab[loc] = cur->next;
    } else {
        HashNode* pre = cur;
        cur = cur->next;
        while(cur){
            if(!HashNode_KeyCmp(cur->key,key)) {
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

int HashMap_ContainsKey(HashMap* self, char* key) {
    if(!self) {
        return -1;
    }
    int loc = (self->cap - 1) & HashNode_Hash(key);
    if(!self->tab[loc]){
        return -1;
    }
    HashNode* cur = self->tab[loc];
    while(cur){
        if(!HashNode_KeyCmp(cur->key,key)) {
            return 0;
        }
        cur = cur->next;
    }
    return -1;
};

int HashMap_GetValue(HashMap*self, char*key) {
    if(!self) {
        abort();
    }
    int loc = (self->cap - 1) & HashNode_Hash(key);
    if(!self->tab[loc]){
        abort();
    }
    HashNode* cur = self->tab[loc];
    while(cur){
        if(!HashNode_KeyCmp(cur->key,key)) {
            return cur->value;
        }
        cur = cur->next;
    }
    abort();
}

//Node function
HashNode* HashNode_New(char *key,int value) {
    HashNode* node = (HashNode*) malloc(sizeof(HashNode));
    size_t len = strlen(key);
    node->key = (char*)malloc(sizeof(char)*len);
    strcpy(node->key,key);
    node->value = value;
    node->hash = HashNode_Hash(key);
    node->next = NULL;
    return node;
}

void HashNode_Destroy(HashNode *self) {
    while(self) {
        HashNode *next = self->next;
        free(self);
        self = next;
    }
}

int HashNode_Hash(char *key){
    int hash = 0;
    char *src = key;
    while (*src) {
        hash = (hash<<5)-hash + *src;
        ++src;
    }
    return hash^(hash>>16);
}

int HashNode_KeyCmp(char*p1, char*p2){
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
