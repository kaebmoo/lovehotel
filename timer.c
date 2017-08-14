#include <time.h>
#include <stdio.h>

const int MIN = 60;
const int HOUR = 3600;

int main(void)
{
	time_t mytime;
	mytime = time(NULL);
	printf("%ld\n", mytime);
	printf(ctime(&mytime));

	printf("\n");
	mytime = mytime + HOUR;
	printf(ctime(&mytime));

	mytime = mytime + (HOUR * 2) + (MIN * 15);
	printf(ctime(&mytime));

	return 0;
}
