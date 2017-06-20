#include<stdio.h>
#include<stdlib.h>
#include "node.h"

int main(void)
{
    // declare a struct with a node as head
	struct mynode *head = NULL;
	// here is the input taken from keyboard
	int  input;
	do{
	    printf("Enter input : ");
        scanf("%d", &input);
        push(&head,input);
    }
	while (input!=0);
	//print the unsorted linked list
    pop(head,0);
	// sort by merge sort and return the head of the new sorted LL
	head = mergeSort(head);
    //print the sorted linked list
	pop(head,1);
	return 0;
}
