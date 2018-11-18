//
//  Deque.h
//  C-library
//
//  Created by anxin on 2018/11/18.
//  Copyright © 2018 anxin. All rights reserved.
//

#ifndef Deque_h
#define Deque_h

#define offsetof(TYPE,MEMBER) ((size_t)&((TYPE*)0)->MEMBER)

#define container_of(ptr,type,member)({ \
const typeof(((type*)0)->member) *__mptr = (ptr); \
(type*)((char*)__mptr - offsetof(type,member));})

#define LIST_HEAD_INIT(name) {&(name),&(name)}

#define LIST_HEAD(name) ({ \
struct Deque name = LIST_HEAD_INIT(name); })

#define list_entry(ptr,type,member) \
    container_of(ptr,type,member)


typedef struct Deque {
    struct Deque* prev, *next;
} Deque;

static inline void __Deque_Push(Deque *to_append, Deque *prev, Deque *next) {
    next->prev = to_append;
    to_append->next = next;
    prev->next = to_append;
    to_append->prev = prev;
}

static inline void __Deque_Delete(Deque *prev, Deque *next) {
    prev->next = next;
    next->prev = prev;
}

inline void Deque_InitHead(Deque *head) {
    head->next = head;
    head->prev = head;
}

inline int Deque_Empty(Deque *head) {
    return head->next == head;
}

inline void Deque_Delete(Deque *head) {
    __Deque_Delete(head->prev,head->next);
}

inline void Deque_Push(Deque *to_append, Deque *head) {
    __Deque_Push(to_append,head, head->next);
}

inline void Deque_PushFront(Deque *to_append, Deque *head) {
    __Deque_Push(to_append,head->prev,head);
}

#endif /* Deque_h */
