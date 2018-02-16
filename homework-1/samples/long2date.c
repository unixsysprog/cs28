#include	<time.h>


main()
{
	long	ttt;
	char	*ctime();

	while( scanf("%ld", &ttt) == 1 )
		printf("that time is %s", ctime(&ttt));
}
