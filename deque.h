//
//  deque.h
//  C-library
//
//  Created by anxin on 2018/11/18.
//  Copyright © 2018 anxin. All rights reserved.
//

#ifndef deque_h
#define deque_h

#define offsetof(TYPE,MEMBER) ((size_t)&((TYPE*)0)->MEMBER)

#define container_of(ptr,type,member)({ \
const typeof(((type*)0)->member) *__mptr = (ptr); \
(type*)((char*)__mptr - offsetof(type,member));})

#define LIST_HEAD_INIT(name) {&(name),&(name)}

#define LIST_HEAD(name) ({ \
struct deque name = LIST_HEAD_INIT(name); })

#define list_entry(ptr,type,member) \
    container_of(ptr,type,member)


typedef struct deque {
    struct deque* prev, *next;
} deque;

static inline void __deque_push(deque *to_append, deque *prev, deque *next) {
    next->prev = to_append;
    to_append->next = next;
    prev->next = to_append;
    to_append->prev = prev;
}

static inline void __deque_delete(deque *prev, deque *next) {
    prev->next = next;
    next->prev = prev;
}

inline void deque_initHead(deque *head) {
    head->next = head;
    head->prev = head;
}

inline int deque_empty(deque *head) {
    return head->next == head;
}

inline void deque_delete(deque *head) {
    __deque_Delete(head->prev,head->next);
}

inline void deque_push(deque *to_append, deque *head) {
    __deque_Push(to_append,head, head->next);
}

inline void deque_pushFront(deque *to_append, deque *head) {
    __deque_Push(to_append,head->prev,head);
}

#endif /* deque_h */
