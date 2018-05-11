/*

	VIVA DISCORDIA
	parts stolen from https://github.com/bo0ts/ddate stolen from util-linux stolen from Five tons of flax. stolen from Lee H:. O:. Smith, KYTP stolen from Druel the Chaotic
	
	54-2-3184 YOLD
	lct
*/



#include <stdio.h>
#include <time.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
//#include <string.h>

// active months are yellow
#define SEASON_COL(x) (_date->season == x ? "\e[1;33m" : "\e[0m")
//normal
#define NORMAL_COL "\e[0m"
// inverted for active day
#define CURRENT_COL "\e[1;7m"
// holydays are red
#define HOLYDAY_COL "\e[1;31m"
// custom holyday
#define CUSTOM_COL "\e[1;36m"
// fnord
#define ERROR_MSG "The answer to your question is FIVE TONS OF MAN DCAL!\n"

char* path = "/.holydays";

struct disc_time 
{
	int season; /* 0-4 */
	int day; /* 0-72 */
	int yday; /* 0-365 */
	int year; /* 3066- */
};

struct special_day
{
	int season;
	int day;
	char* desc;
};

struct disc_time convert();
void printDate(struct disc_time*,int);

// Custom Hollidays
int getNumberFileEntries();
struct special_day* getSpecialDays(int);
int isSpecialDay(int,struct special_day*, int, int);
void printSpecialDay(int,int);

int main(int argc, char *argv[])
{
	struct disc_time hastur;
	int pi;
	char* fnord = NULL;
	
	struct passwd *pw = getpwuid(getuid());
	
	char* tmpPath =  pw->pw_dir;
	strcat(tmpPath,path);
	path = tmpPath;
	
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
					case 'd':
						if (argc != 3 + pi)
							printf(ERROR_MSG);
						else
							printSpecialDay(atoi(argv[pi+1]),atoi(argv[pi+2]));
						return 0;
					default: 
						printf(ERROR_MSG);
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
	struct special_day* specialDays;
	int specialDayCount;
	int specialDay;
	
	specialDayCount = getNumberFileEntries();
	specialDays = getSpecialDays(specialDayCount);
	
	if(specialDay = isSpecialDay(specialDayCount, specialDays, _date->season + 1, _date->day + 1))
		printf("%d YOLD - %s\n", _date->year, specialDays[specialDay - 1].desc);
	else
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
	
	printf(NORMAL_COL);
	
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
					printf(HOLYDAY_COL);
				else if (isSpecialDay(specialDayCount, specialDays, column + 1, day))
					printf(CUSTOM_COL);
				
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
	
	// Cleanup
	for(specialDay = 0; specialDay < specialDayCount; specialDay++)
		free(specialDays[specialDay].desc);
}

void printSpecialDay(int _day, int _month)
{
	struct special_day* specialDays;
	int specialDayCount;
	int specialDay;
	
	specialDayCount = getNumberFileEntries();
	specialDays = getSpecialDays(specialDayCount);
	
	if(specialDay = isSpecialDay(specialDayCount, specialDays, _month, _day))
		printf("%s\n",  specialDays[specialDay - 1].desc);
	else
		printf("a new day and nothing has changed\n");
	
	// Cleanup
	for(specialDay = 0; specialDay < specialDayCount; specialDay++)
		free(specialDays[specialDay].desc);
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

int getNumberFileEntries()
{
	int count = 0;
	FILE* fp;
	char* line;
	size_t len = 0;
	ssize_t read;
	
	fp = fopen(path, "r");
	if (fp == NULL)
	{
		return 0;
	}
	
	while((read = getline(&line, &len, fp)) != -1)
	{
		count++;
	}
	fclose(fp);

	return count;
}

struct special_day* getSpecialDays(int _count)
{
	struct special_day* specialDays;
	FILE* fp;
	char* line;
	size_t len = 0;
	ssize_t read;
	int counter = 0;
	int currentWordLength;
	int wordIndex;
	int tmpWordIndex;

	specialDays = (struct special_day*) malloc(_count * sizeof(struct special_day));

	fp = fopen(path, "r");
	if (fp == NULL)
	{
        	return NULL;
	}

	while((read = getline(&line, &len, fp)) != -1)
	{
		char* currentWord;
		
		wordIndex = 0;
		currentWord = (char*) malloc(read * sizeof(char));
		if(!currentWord)
		{
			printf("malloc has the curse of greyface");
			return NULL;
		}
		memset(currentWord,0, read);
		for(tmpWordIndex = 0; line[wordIndex] != ',' && line[wordIndex] != '\n'; wordIndex++, tmpWordIndex++)
		{
			currentWord[tmpWordIndex] = line[wordIndex];
		}
		specialDays[counter].day = atoi(currentWord);
		
		memset(currentWord,0, read);
		tmpWordIndex = 0;
		wordIndex++;
		for(tmpWordIndex = 0; line[wordIndex] != ',' && line[wordIndex] != '\n'; wordIndex++, tmpWordIndex++)
		{
		        currentWord[tmpWordIndex] = line[wordIndex];
		}
		specialDays[counter].season = atoi(currentWord);
		
		memset(currentWord, 0, read);
		tmpWordIndex = 0;
		wordIndex++;
		for(tmpWordIndex = 0; line[wordIndex] != ',' && line[wordIndex] != '\n'; wordIndex++, tmpWordIndex++)
		{
		        currentWord[tmpWordIndex] = line[wordIndex];
		}
		specialDays[counter++].desc = currentWord;
	}
	fclose(fp);
	
	return specialDays;
}

int isSpecialDay(int _entryCount, struct special_day* _specialDays, int _season, int _day)
{
	int i;

	if (_entryCount == 0 || !_specialDays)
	{
		return 0;
	}

	for(i = 0; i < _entryCount; i++)
	{
		if (_specialDays[i].season == _season && _specialDays[i].day == _day)
		{
			return i + 1;
		}
	}
	return 0;
}
