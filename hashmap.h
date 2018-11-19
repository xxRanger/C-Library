//
//  hashmap.h
//  C-library
//
//  Created by anxin on 2018/11/17.
//  Copyright © 2018 anxin. All rights reserved.
//

#ifndef hashmap_h
#define hashmap_h

const static int MAXIMUM_CAPACITY = 1<<30;
const static float DEFAULT_LOAD_FACTOR = 0.75;
const static int DEFAULT_INITIAL_CAPACITY = 16;

struct hashnode {
    char *key;
    int value;
    int hash;
    struct hashnode *next;
};

struct hashmap {
    int cap;
    float load_factor;
    int threshold;
    int len;
    struct hashnode **tab;
};

typedef struct hashnode hashnode;
typedef struct hashmap hashmap;

//hashmap function
extern hashmap* hashmap_new(void);
extern int hashmap_map_size_for(int cap);
extern void hashmap_resize(hashmap* self);
extern void hashmap_put(hashmap* self, char* key, int value);
extern void hashmap_delete(hashmap* self, char* key);
extern int hashmap_contains(hashmap* self, char* key);
extern int hashmap_get(hashmap*self, char*key);
extern void hashmap_destroy(hashmap* self);
//Node function
extern hashnode* hashnode_new(char *,int);
extern void hashnode_destroy(hashnode *self);
extern int hashnode_hash(char *key);
extern int hashnode_cmp(char*p1, char*p2);
#endif /* hashmap */
