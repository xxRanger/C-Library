//
//  HashMap.h
//  C-library
//
//  Created by anxin on 2018/11/17.
//  Copyright © 2018 anxin. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h

const static int MAXIMUM_CAPACITY = 1<<30;
const static float DEFAULT_LOAD_FACTOR = 0.75;
const static int DEFAULT_INITIAL_CAPACITY = 16;

struct HashNode {
    char *key;
    int value;
    int hash;
    struct HashNode *next;
};

struct HashMap {
    int cap;
    float load_factor;
    int threshold;
    int len;
    struct HashNode **tab;
};

typedef struct HashNode HashNode;
typedef struct HashMap HashMap;

//HashMap function
HashMap* HashMap_New(void);
int HashMap_MapSizeFor(int cap);
void HashMap_Resize(HashMap* self);
void HashMap_Put(HashMap* self, char* key, int value);
void HashMap_Delete(HashMap* self, char* key);
int HashMap_ContainsKey(HashMap* self, char* key);
int HashMap_GetValue(HashMap*self, char*key);
void HashMap_Destroy(HashMap* self);
//Node function
HashNode* HashNode_New(char *,int);
void HashNode_Destroy(HashNode *self);
int HashNode_Hash(char *key);
int HashNode_KeyCmp(char*p1, char*p2);
#endif /* HashMap */
