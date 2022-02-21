#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_file(char names[2000][200], char genres[2000][20], char platforms[2000][20], float array[2000][7]); //for all options
int check_array(char name[20], char genres[2000][20]); //for option 0 and 1
int add_array(char name[20], char genres[2000][20]); //for option 0 and 1
void print_list(char array[2000][20]); //for option 0 and 1
void list_by_year(int year, char names[2000][200], float array[2000][7]); //for option 2
void list_by_year2(int year, char names[2000][200], float array[2000][7]); //for option 2
int informations_game(char name_of_game[200], char names[2000][200], float array[2000][7], char genres[2000][20], char platforms[2000][20]); //for option 3
void calculate_average(float array[2000][7]); //for option 4
int more_popular(char name_of_game_p[200], float array[2000][7], char names[2000][200]); //for option 5
void frequence_genres(float array[2000][7], char genres[2000][20]); //for option 6
int frequence_total(float array[2000][7], int id); //for option 6
void frequence_platforms(float array[2000][7], char genres[2000][20]); //for option 7
int frequence_total2(float array[2000][7], int id); //for option 7

int main() 
{
	char names[2000][200]; //The array where I keep the names
	char genres[2000][20]; //The array where I keep the genres
	char platforms[2000][20]; //The array where I keep the platforms
	float array[2000][7]; /*The array where I keep the genre, platform, year of release, NA sales, EU sales,
	global sales and user score*/
	char name_of_game[200]; //to list the information of the entered game
	char name_of_game_p[200]; //to compare the sales of the entered game
	read_file(names, genres, platforms, array); //Reading from the file into the array with function read_file
	int choice, loop = 1, year, year2; 
	
	do{
	printf("\n0: List of the Genres\n");
	printf("1: List of the Platforms\n");
	printf("2: List of the Games Through the Years\n");
	printf("3: All Informations of a Single Game\n");
	printf("4: Average of the User Scores\n");
	printf("5: Georographical Information of a Single Game\n");
	printf("6: Frequence of the Genres\n");
	printf("7: Frequence of the Platforms\n");
	printf("8: Exit\n");
	printf("Please select an operation: ");
	scanf("%d", &choice);
	
	if (choice == 0)
		print_list(genres);
	else if (choice == 1)
		print_list(platforms);
	else if (choice == 2)
	{
		printf("Enter a year: ");
		scanf("%d", &year);
		printf("Until (0) or Since (1) %d: ", year);
		scanf("%d", &year2);
		if(year2 == 1)
			list_by_year(year, names, array);
		else if(year2 == 0)
			list_by_year2(year, names, array);
	}
	else if (choice == 3)
	{
		printf("Please enter the name of the game: ");
		scanf("%s", name_of_game);
		informations_game(name_of_game, names, array, genres, platforms);
	}
	else if (choice == 4)
		calculate_average(array);
	else if (choice == 5)
	{
		printf("Please enter the name of the game: ");
		scanf("%s", name_of_game_p);
		more_popular(name_of_game_p, array, names);
	}
	else if (choice == 6)
		frequence_genres(array, genres);
   else if (choice == 7)
		frequence_platforms(array, platforms);
	else if (choice == 8)
		loop++; //Loop whose value is 1 is increased to exit the infinite loop
	else 
	{
		printf("\nTRY AGAIN.\n\n\n");
    	//When a number other than 0-8 is entered, it gives an error message and the menu comes back
	}
	}while(loop == 1);
}

void read_file(char names[2000][200], char genres[2000][20], char platforms[2000][20], float array[2000][7])
{
	char buffer[50000];
	char *row, *column;
	int i = 0, id;
	FILE *fstream = fopen("Video_Games.txt", "r");
	fgets(buffer, sizeof(buffer), fstream); 
	/*The first line in the text file is the title of the data, this line is not needed so I jumped 
	without reading the first line*/
	while((row = fgets(buffer, sizeof(buffer), fstream)) != NULL)
	{
		/* In the text file, the data is comma separated, and each comma is in the form of a column, 
		so by separating the data with the help of the strtok function, I assign each column to the second 
		dimension of a two-dimensional array, so that only the rows are the same, that is, one row for each
		game, but connected to each game. We had 7 columns.*/
    	column = strtok(row, ","); //Name 
    	strcpy(names[i], column);
    	/* Each column is separated by commas, so each comma is a column so I separated my columns by commas*/
    	column = strtok(NULL, ","); //Genres
    	id = check_array(column, genres);
    	if(id == -1)
    	{
			id = add_array(column, genres);
    	}
    	array[i][0] = id;
    	column = strtok(NULL, ","); //Platform
    	id = check_array(column, platforms);
    	if(id == -1)
    	{
			id = add_array(column, platforms);
    	}
    	array[i][1] = id;
    	column = strtok(NULL,","); //Year of Release 
    	array[i][2] = strtod(column, NULL);
    	column = strtok(NULL, ","); //NA Sales
    	array[i][3] = strtod(column, NULL);
    	column = strtok(NULL, ","); //EU Sales
    	array[i][4] = strtod(column, NULL);
    	column = strtok(NULL, ","); //Global Sales
    	if(strcmp(column, "not_available") == 0)
    	{
			/*I gave a random minus number, I used this part in the information_game function in the next 
			lines. If the sale is not_available, let's be equal to -99 (less than 0) and when the information 
    		of this column is written in the information_game function, the column is -99, so if it is less 
    		than 0, print not_available*/
    		array[i][5] = -99; 
    	}
    	else
    	{
			array[i][5] = strtod(column, NULL);
    	}
    	column = strtok(NULL, ","); //User Score
    	array[i][6] = strtod(column, NULL);
    	i++;
	}
	fclose(fstream);
}

/*Unique data required for genre and platform, checks it. If there is data, it will not write again, 
if not, if the result is -1, it writes the data*/
int check_array(char name[20], char genres[2000][20])
{
	int i, result = -1;
	for (i = 0; i < 2000; i++)
	{
		if(strcmp(genres[i], name) == 0)
		{
    		result = i;
			break;
		}
	}
	return result;
}

/* If the result returns -1 in the check_array function, that is, if the data was never written, 
this function is for writing this data.*/
int add_array(char name[20], char genres[2000][20])
{
	int i, result = -1;
	for (i = 0; i < 2000; i++)
	{
    	if(strlen(genres[i]) == 0)
    	{
    		result = i;
    		break;
		}
	}
	if (result != -1)
		strcpy(genres[result], name);
	
	return result;
}

void print_list(char array[2000][20]) //For genres and platforms lists
{
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(strlen(array[i]) > 0) 
			printf("%s\n", array[i]);
	}
}

void calculate_average(float array[2000][7]) //Calculates the user scores
{
	int i;
	float conc = 0, average = 0;
	for(i = 0; i < 2000; i++)
		conc = conc + array[i][6];
	
	average = conc / 2000;
	printf("Average: %f\n", average);
}

int informations_game(char name_of_game[200], char names[2000][200], float array[2000][7], char genres[2000][20], char platforms[2000][20])
{
	int i, result = -1;
	for(i = 0; i < 2000; i++)
	{
		if(strcmp(names[i], name_of_game) == 0)
			/*Compares the name entered by the user with the names in the 'names' array, if it matches,
			prints the columns of that row*/
		{
			result = 0;
			printf("Name:\t\t\t %s\n", names[i]);
			printf("Genre:\t\t\t %s\n", genres[(int)array[i][0]]);
			printf("Platform:\t\t %-15s\n", platforms[(int)array[i][1]]);
			printf("Year:\t\t\t %4.0f\n", array[i][2]);
			printf("Sales in NA:\t\t %f\n", array[i][3]);
			printf("Sales in EU:\t\t %f\n", array[i][4]);
			
			if (array[i][5] < 0)
				printf("Total Sales:\t\t Not Available\n");
			
			else
				printf("Total Sales:\t\t %f\n", array[i][5]);
			
			printf("User Score:\t\t %f\n", array[i][6]);
			break;
		}	
	}
	return result;
}

void frequence_genres(float array[2000][7], char genres[2000][20])
{
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(strlen(genres[i]) > 0)
			printf("%-15s\t%d\n", genres[i], frequence_total(array, i));
		else
			break;
	}
}

/*The genres are in column 0. I have defined a variable called id that is any type, if such types 
match any element of the array (there are 2000 lines, values), the counter increments and returns the
counter (to use in frequence_genres function)*/
int frequence_total(float array[2000][7], int id)
{
	int i, count = 0; 
	for(i = 0; i < 2000; i++)
	{
		if(array[i][0] == id)
			count++;
	}
	return count;
}

void frequence_platforms(float array[2000][7], char platforms[2000][20])
{
	int i;
	for(i = 0; i< 2000; i++)
	{
		if(strlen(platforms[i]) > 0)
			printf("%-15s\t%d\n", platforms[i], frequence_total2(array, i));
		else
			break;
	}
}

int frequence_total2(float array[2000][7], int id)
{
	int i, count = 0;
	for(i = 0; i < 2000; i++)
	{
		if(array[i][1] == id)
			count++;
	}
	return count;
}

int more_popular(char name_of_game_p[200], float array[2000][7], char names[2000][200])
{
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(strcmp(names[i],name_of_game_p) == 0) //Does the game name entered by the user match?
		{
			if (array[i][3] > array[i][4])
				printf("This game was more popular in North America.\n");
			else if (array[i][3] < array[i][4])
				printf("This game was more popular in Europe.\n");
			else 
				printf("Equal sales in North America and Europe.\n");
			break;
		}
	}
}

/*The user enters a year and the years are in column 2 in the array; A comparison is made between this 
year and the second column of the array*/
void list_by_year(int year, char names[2000][200], float array[2000][7])

{
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(array[i][2] > year)
			printf("%s\n", names[i]);
	}
	
}
void list_by_year2(int year, char names[2000][200], float array[2000][7])
{
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(array[i][2] < year)
			printf("%s\n", names[i]);
	}
}
/*I created the same function for the list by years option, because after the user has entered the year,
the function list_by_year2 works if it is before, because it again asks whether the listing wants before
or after that year.*/
