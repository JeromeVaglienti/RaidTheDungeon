#include "SegmentLight.h"

void InitSegmentsLight(SegmentsLight* segments)
{
	segments->numberSegment = 0;
	segments->Array = NULL;
}

void AddSegmentsLight(SegmentsLight* segments, const sfVector2f p1, const sfVector2f p2)
{
	segments->numberSegment++;
	segments->Array = (SegmentL*)realloc(segments->Array, sizeof(SegmentL)*segments->numberSegment);
	segments->Array[segments->numberSegment - 1].P1 = p1;
	segments->Array[segments->numberSegment - 1].P2 = p2;
}

/*sfBool IsEqualColor(const sfColor color1, const sfColor color2)
{
	return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}*/

void ClearAllSegmentsLight(SegmentsLight* segments)
{
	if (segments->Array)
	{
		free(segments->Array);
	}
}