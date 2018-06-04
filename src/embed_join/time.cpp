#include "embedjoin.h"

struct timeval s,e;
float timeread = 0.0, timeembed = 0.0, timelsh = 0.0, timeedit = 0.0;

void recordtime()
{
	gettimeofday(&s, NULL);
}
void updatetime(int i)
{
	gettimeofday(&e, NULL);
	if(i == 1)
		timeread += e.tv_sec + e.tv_usec / 1000000.0 - s.tv_sec - s.tv_usec / 1000000.0;
	if(i == 2)
		timeembed += e.tv_sec + e.tv_usec / 1000000.0 - s.tv_sec - s.tv_usec / 1000000.0;
	if(i == 3)
		timelsh += e.tv_sec + e.tv_usec / 1000000.0 - s.tv_sec - s.tv_usec / 1000000.0;
	if(i == 4)
		timeedit += e.tv_sec + e.tv_usec / 1000000.0 - s.tv_sec - s.tv_usec / 1000000.0;
}

void outputtime()
{
	fprintf(stderr, "Time(read data):%f\nTime(embed data):%f\n", timeread, timeembed);
	fprintf(stderr, "Time(lsh):%f\nTime(verification):%f\n", timelsh, timeedit);
	fprintf(stderr, "Time(total):%f\n", timeread + timeembed + timelsh + timeedit);
}


