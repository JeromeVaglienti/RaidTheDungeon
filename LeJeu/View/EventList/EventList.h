#ifndef _EVENTLIST_H
#define _EVENTLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>


typedef struct EVENTLIST
{
	sfEvent Event;
	struct EVENTLIST* Next;
}EventList;


void AddElement(EventList **First, sfEvent toAdd);
void CleanEventList(EventList **ToClean);



#endif // !_EVENTLIST_H
