#include<stdio.h>
#include<stdlib.h>

struct mynode
{
	int const inputVal;
	struct mynode *next;
	struct mynode *prev;
};

// Function to compare and merge two linked lists
struct mynode *mergelist(struct mynode *left, struct mynode *right)
{
	// If the left linked list is empty then return the right one
	if (left==NULL){
        return right;

	// If the right linked list is empty then return the left one
	} else if (right==NULL){
		return left;

	// if the left linked list value is smaller than the right linked list value
	} else if ((*left).inputVal < (*right).inputVal){
	    // if the value of the first element of the left LL is smaller than the first element of
	    // the right LL then we should compare the second element of the left LL to the first element of
	    // the right LL.
	    // whatever value is returned is the smallest value after the first element of the left LL.
		left->next = mergelist(left->next,right);
		// once the next element of the left half is returned, the first element of the left half (the smallest value)
		// should be referenced to the previous of the returned value above
		(left->next)->prev = left;
		left->prev = NULL;
		return left;
	} else {
	    // if the value of the first element of the right LL is smaller than the first element of
	    // the left LL then we should compare the second element of the right LL to the first element of
	    // the left LL.
	    // whatever value is returned is the smallest value after the first element of the right LL.
		right->next = mergelist(left,right->next);
		(right->next)->prev = right;
		right->prev = NULL;
		return right;
	}
}

// Here is the split method. In this method the linked list is split into two halves by
// introducing two nodes: single and double. Single traverse one by one and double traverse
// two by two. Once double reaches the end of the linked list, single is at the middle of it.
// Then, the next node is the beginning of the next half
struct mynode *split(struct mynode *head)
{
    // defining single and double and initializing with the head
	struct mynode *Double = head;
	struct mynode *Single = head;
	// since double jumps two by two both double.next and double.next.next must be checked.
	// checking double.next.next is not sufficient since double might be the last node,
	// then double.next is NULL and double.next.next does not makes sense at all.
	// It causes the program to crash

	while (Double->next != NULL && Double->next->next != NULL){
	    // Jump twice for double and jump once for single
		Double = Double->next->next;
		Single = Single->next;
	}

	// Alternatively I can check the first statement and if correct I can increment double and then
	// check the second statement and if correct increment both.  Incrementing both first and then double
	// makes the program to crash
	/*
	while (Double->next != NULL){
	    Double = Double->next;
		if (Double->next != NULL){
            Double = Double->next;
           	Single = Single->next;
		}
	}
	*/


	// once it double reaches the end place single.next into the head of the second half of
	// the program linked list
	struct mynode *secondHalfHead = Single->next;
	// NULL the next node of the last node of the first half and return the head of second half
	Single->next = NULL;
	return secondHalfHead;

	// this part is a developing section.  It works sometimes but it crashed multiple time.
	// It gets stuck in a while loop.  It is based on traversing from the head to the
	// end of the linked list and then counting. Then traversing again from the beginning to
	// the half of the counter and capture the last node and then return the value of the
	// starting point of the next node
	/*
	struct mynode *temp1 = head;
    struct mynode *temp2 = head;
    int count1 = 1;
    while (temp1->next!=NULL){
        temp1 = temp1->next;
        count1++;
    }
    //printf("\n%d ", count1);
    int count2 = count1/2;
    printf("\n%d ", count2);
    int i = 0;
    while (i<count2){
        temp2 = temp2->next;
        i++;
    }
    struct mynode *temp = temp2->next; //->next;
	temp2->next = NULL;
	return temp;
	*/
}

// This is the main merge sort function
struct mynode *mergeSort(struct mynode *head)
{
    // if the linked list is empty OR the node next to head is NULL (which means it has
    // a single value, then return the head and it is DONE!
	if (head == NULL || (*head).next == NULL){
		return head;
		}
    // head of the first list is preserved and the linked list is
    // split into twp parts and the head of the second half is assigned
    // to  rightHead
	struct mynode *rightHead = split(head);

	// mergeSort recursively which means that for each half of the linked list run the same thing
	head = mergeSort(head);
	rightHead = mergeSort(rightHead);

	// now merge the two halves
	return mergelist(head,rightHead);
}

// This is a function to push values from scanf into the linked list.  The input values are the data which is
// inputVal and a pointer to the address of the head.  It is a pointer to pointer (**head).  The first pointer is
// to the address of the input value and the second pointer is the pointer to the address of the pointer
void push(struct mynode **head, int inputVal)
{
   	struct mynode *temp = *head;
    // here a node is made and a memory is allocated to it.
	struct mynode *current = (struct mynode *)malloc(sizeof(struct mynode));
	// since inputVal in the mynode struct is a constant, it is impossible to initialize that.  in order to initialize and already
	// const-defined value we have to use a pointer.  this is the structure used to cast away the const to initialize the fields of
	// a malloc'ed structure instead of &temp->data = data.  we use the address of a and define a pointer which point to the address and
	// changes the value manually
	*(int *)&current->inputVal = inputVal;
	// initialize the next and previous nodes of the current node to NULL.  In other words, it is just a node and no connection.
	current->next = NULL;
	current->prev = NULL;
	// if the LL is empty assign a to head
	if (*head == NULL) {
        (*head) = current;

	} else {
        // otherwise traverse from the head to the last node and then assign the current node to the next of the last node in the LL
        // and assign the previous of the current node to the last node of the old struct
		while (temp->next != NULL){
                temp = temp->next;
		}
        // the next element of the temp which is the tail of the old LL is linked to current
        temp->next = current;
        //  current is set to tail
        current->next = NULL;
        // current is linked back to previous node which is temp
        current->prev = temp;
	}
}

// a method to print the linked list with an input (head)
// if status == 0 it prints it before sorting
// if status == 1 it prints it after sorting

void pop(struct mynode *head, int status)
{
    // declare a new pointer to traverse and pop
    struct mynode *temp = head;
    if (status == 0) 	printf("\nData before sorting : ");
    if (status == 1)	printf("\nData after sorting  : ");
    while (head != NULL) {
		printf("%d ",temp->inputVal);
		temp = temp->next;
	}
}
