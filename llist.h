//
//  llist.h
//  C-library
//
//  Created by anxin on 2018/11/19.
//  Copyright © 2018 anxin. All rights reserved.
//

#ifndef llist_h
#define llist_h

typedef struct llist {
    struct llist_node *head;
    int count;
} llist;

extern llist *llist_create(void);
extern void llist_destroy(llist *llist);
extern void *llist_push_front(llist *llist, void *data);
extern void *llist_push_back(llist *llist, void *data);
extern void *llist_head(llist *llist);
extern void *llist_tail(llist *llist);
extern void *llist_find(llist *llist, void *data, int (*cmpfn)(void *, void *));
extern void *llist_delete( llist *llist, void *data, int (*cmpfn)(void *, void *));
extern int llist_count( llist *llist);
extern void llist_foreach( llist *llist, void (*f)(void *, void *), void *arg);

#endif /* llist_h */
