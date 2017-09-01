#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

struct mynode;
struct mynode *split(struct mynode *head);
struct mynode *mergelist(struct mynode *first, struct mynode *second);
struct mynode *mergeSort(struct mynode *head);
void push(struct mynode **head, int data);
void pop(struct mynode *head, int status);
struct mynode *split(struct mynode *head);

#endif // NODE_H_INCLUDED

