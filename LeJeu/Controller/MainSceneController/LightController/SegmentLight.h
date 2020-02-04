#ifndef _SEGMENTLIGHT_H
#define _SEGMENTLIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <SFML\Graphics.h>

#define rSegment (sfColor){239, 0, 35, 255}
#define lSegment (sfColor){59, 182, 0, 255}
#define dSegment (sfColor){14, 255, 255, 255}
#define uSegment (sfColor){255, 240, 0, 255}
#define ldSegment (sfColor){181, 96, 166, 255}
#define ldrSegment (sfColor){192, 121, 42, 255}
#define ldruSegment (sfColor){43, 53, 148, 255}
#define drSegment (sfColor){246, 53, 148, 255}
#define druSegment (sfColor){0, 45, 4, 255}
#define ruSegment (sfColor){245, 124, 0, 255}
#define rulSegment (sfColor){141, 63, 30, 255}
#define ulSegment (sfColor){0, 0, 0, 255}
#define uldSegment (sfColor){180, 181, 53, 255}
#define lrSegment (sfColor){40, 215, 134, 255}
#define udSegment (sfColor){209, 177, 119, 255}



typedef struct SEGMENTL
{
	sfVector2f P1;
	sfVector2f P2;
}SegmentL;

typedef struct SEGMENTSLIGHT
{
	int numberSegment;
	SegmentL* Array;
}SegmentsLight;

void InitSegmentsLight(SegmentsLight* segments);

void AddSegmentsLight(SegmentsLight* segments, const sfVector2f p1, const sfVector2f p2);

sfBool IsEqualColor(const sfColor color1, const sfColor color2);

void ClearAllSegmentsLight(SegmentsLight* segments);

#endif