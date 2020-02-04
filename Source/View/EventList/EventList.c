#include "EventList.h"

void AddElement(EventList **First, sfEvent toAdd)
{
	EventList* temp = (EventList*)calloc(1, sizeof(EventList));
	temp->Event = toAdd;
	temp->Next = *First;
	*First = temp;
}

void CleanEventList(EventList **ToClean)
{
	EventList *tempEvent = *ToClean;
	while (tempEvent != NULL)
	{
		EventList* EventToDelete = tempEvent;
		tempEvent = tempEvent->Next;
		free(EventToDelete);
		EventToDelete = NULL;
	}
	*ToClean = NULL;
}