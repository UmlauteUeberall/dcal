#include <stdio.h>
#include <time.h>

// acitve months are yellow
#define SEASON_COL(x) (_date->season == x ? "\e[1;33m" : "\e[0m")
//normal
#define NORMAL_COL "\e[0m"
// inverted for active day
#define CURRENT_COL "\e[1;7m"
// hollidays are red
#define HOLLIDAY_COL "\e[1;31m"

struct disc_time 
{
    int season; /* 0-4 */
    int day; /* 0-72 */
    int yday; /* 0-365 */
    int year; /* 3066- */
};

struct disc_time convert();
void printDate(struct disc_time*,int);

int main(int argc, char *argv[])
{
	struct disc_time hastur;
	int pi;
	char* fnord = NULL;

	hastur=convert();

	for(pi=1; pi<argc; pi++) 
	{
		switch(argv[pi][0]) 
		{
			case '+': fnord=argv[pi]+1; break;
			case '-': 
			    	switch(argv[pi][1]) 
				{
	    				case 'f':
						printDate(&hastur,1);
						return 0;
	    				default: 
						printf("kaputt, try 'dcal' or 'dcal -f'");
						return 0;
	    			}
			default: 
				goto thud;
		}	
	}
	
	thud:
		printDate(&hastur,0);

	return 0;
} 

void printDate(struct disc_time* _date, int _longVersion)
{
	int column;
	int line;
	int weekday;
	int day;

	printf("%d YOLD\n",_date->year);
	
	if (_longVersion)	
		printf("%sCHAOS\t\t%sDISCORD\t\t%sCONFUSION\t%sBUREAUCRACY\t%sTHE AFTERMATH\e[0m\n",SEASON_COL(0), SEASON_COL(1), SEASON_COL(2), SEASON_COL(3), SEASON_COL(4));
	else
		switch(_date->season)
		{
			case 0:
				printf("%sCHAOS\e[0m\n", SEASON_COL(0));
				break;
			case 1:
			        printf("%sDISCORD\e[0m\n", SEASON_COL(1));
			        break;
			case 2:
			        printf("%sCONFUSION\e[0m\n", SEASON_COL(2));
			        break;
			case 3:
			        printf("%sBUREAUCRACY\e[0m\n", SEASON_COL(3));
			        break;
			case 4:
			        printf("%sTHE AFTERMATH\e[0m\n", SEASON_COL(4));
			        break;

		}
	
	for(column = 0; column < (_longVersion ? 5 : 1); column++)
	{
		printf("SM BT PD PP SO\t");
	}
	printf("\n");
	for(line = 0; line < 16; line++)
	{
		for(column = 0; column < 5; column++)
		{
			if (!_longVersion && column != _date->season)
				continue;

			for(weekday = 0; weekday < 5; weekday++)
			{
				day = 1 + weekday + line * 5;
				day -= (3 * column) % 5;
				
				if (day - 1 == _date->day && column == _date->season)
					printf(CURRENT_COL);
				else if (day == 5 || day == 50)
					printf(HOLLIDAY_COL);
				
				if (day < 1)
					printf("  ");
				else if (day < 10)
					printf(" %d", day);
				else if (day > 73)
					printf("  ");
				else
					printf("%d", day);
				printf(NORMAL_COL);
				if (day > 1 && day < 73);
				printf(" ");
			}
			printf("\t");
			
		}
		printf("\n");
	}
}

struct disc_time convert()
{     
	time_t day;
	struct tm* dday;
	struct disc_time funkychickens;

	day = time(NULL);
	dday =localtime(&day);
	
	funkychickens.year = dday->tm_year+3066;
	funkychickens.day=dday->tm_yday;
	funkychickens.season=0;
	if ((funkychickens.year%4)==2)
	{
		if (funkychickens.day==59)
			funkychickens.day=-1;
		else if (funkychickens.day >59)
			funkychickens.day-=1;
	}
	funkychickens.yday=funkychickens.day;
	while (funkychickens.day>=73)
	{ 
		funkychickens.season++;
		funkychickens.day-=73;
	}	
	return funkychickens;
  
}
