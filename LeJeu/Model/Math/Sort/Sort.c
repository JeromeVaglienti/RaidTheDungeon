#include "..\..\Enemy\Enemy.h"

Enemy* mergeSortedLists(Enemy* list1, Enemy* list2);
void splitLL(Enemy* head, Enemy** list1, Enemy** list2);

//Main function, which will sort the lists
void mergeSort(Enemy** head)
{
	Enemy* list1;
	Enemy* list2;
	/* Base case -- length 0 or 1 */
	if ((*head == NULL) || ((*head)->Next == NULL))
	{
		return;
	}

	// Split list(head) into 'list1' and 'list2' sublists 
	splitLL(*head, &list1, &list2);

	/* Recursively sort the sublists */
	mergeSort(&list1);
	mergeSort(&list2);
	//merge the two sorted lists together
	*head = mergeSortedLists(list1, list2);
}
//Merge the two sorted lists
Enemy* mergeSortedLists(Enemy* list1, Enemy* list2)
{
	//display(&list1);
	//cout<<(list1->data);
	Enemy* result = NULL;

	/* Base cases */
	if (list1 == NULL)
		return(list2);
	else if (list2 == NULL)
		return(list1);

	/* Pick either list1 or list2, and recur */
	if (list1->RigidBody.Position.y < list2->RigidBody.Position.y)
	{
		list1->Next = mergeSortedLists(list1->Next, list2);
		list1->Next->Previous = list1;
		list1->Previous = NULL;
		return list1;
	}
	else
	{
		list2->Next = mergeSortedLists(list1, list2->Next);
		list2->Next->Previous = list2;
		list2->Previous = NULL;
		return list2;
	}
}

//split the nodes into two halves 
void splitLL(Enemy* head,
	Enemy** list1, Enemy** list2)
{
	Enemy* fast;
	Enemy* slow;
	if (head == NULL || head->Next == NULL)
	{
		/* length < 2 cases */
		*list1 = head;
		*list2 = NULL;
	}
	else
	{
		slow = head;
		fast = head->Next;

		// Move 'fast' two nodes, and move 'slow' one node //
		while (fast != NULL)
		{
			fast = fast->Next;
			if (fast != NULL)
			{
				slow = slow->Next;
				fast = fast->Next;
			}
		}

		/* 'slow' is before the midpoint in the list, so split it in two
		at that point. */
		*list1 = head;
		*list2 = slow->Next;
		slow->Next = NULL;
	}
}