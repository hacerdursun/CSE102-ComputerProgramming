#include<stdio.h>
#include<string.h>
#define WORD_LENGTH 10 //for input1.txt, maximum word length is 10 characters
#define LENGTH_EACH_ROW 250 //for input2.txt, maximum length of each row is 250
#define LENGTH_NUMBER_OF_ROW 100 //for input2.txt, maximum number of rows is 100
#define CHECK_HORIZONTAL 1 //Return value of horizontal search 
#define CHECK_VERTICAL 2 //Return value of vertical search 

int check_horizontal(char* words, int words_size, int* v_coord, int* h_coord); //Horizontal search 
int check_vertical(char* words, int words_size, int* v_coord, int* h_coord); //Vertical search 

int main()
{
	char words[WORD_LENGTH]; //for words in input1.txt
	int words_size, v_coord, h_coord; //Vertical and horizontal coordinates
	FILE* fin = fopen("input1.txt", "r"); //for input1.txt
	FILE* fout = fopen("output.txt", "w"); //for output.txt
	while(fscanf (fin, "%s", words) != EOF) //Assigns words in input1 to words array
	{ 
		words_size = strlen(words);
		if(check_horizontal(words, words_size, &v_coord, &h_coord) == CHECK_HORIZONTAL)
			fprintf(fout, "%s (%d,%d) Horizontal\n", words, h_coord, v_coord);
		else if(check_vertical(words, words_size, &v_coord, &h_coord) == CHECK_VERTICAL)
			fprintf(fout, "%s (%d,%d) Vertical\n", words, v_coord, h_coord);
	}
	fclose(fin);
	fclose(fout);
	
	return 0; 
}

/*Horizontal search: as parameters; the array I have stored in input1 and the size of this array; 
the horizontal and vertical initial position*/
int check_horizontal(char* words, int words_size, int* v_coord, int* h_coord) 
{
	char row[LENGTH_EACH_ROW], total[LENGTH_NUMBER_OF_ROW][LENGTH_EACH_ROW];
	int row_size[LENGTH_EACH_ROW], i, j, rows_number = 0, size = 0, temp = 0, counter = 0;
	FILE* fin2 = fopen("input2.txt", "r");
	while(fscanf(fin2, "%s", row) != EOF) //to read from input2.txt and assigned to the total
	{ 
		row_size[rows_number] = strlen(row); //All row's size
		for(i = 0; i < row_size[rows_number]; i++) 
			total[rows_number][i] = row[i]; 
		rows_number++; //to holds number of rows
	}
	
	for(i = 0; i < rows_number; i++)
	{
		for(j = 0; j < row_size[i]; j++)
		{
			if(total[i][j] == words[temp]) //to check the character at the location and the first character of the word
			{ 
				temp = j;
				while(total[i][temp] == words[counter] && size != words_size) //Checks the length of the word horizontally
				{ 
					size++;
					temp++;
					counter++;
				}
				if(size == words_size) //if same *
				{ 
					*v_coord = j + 1; //Position indices start from 1
					*h_coord = i + 1; 
					return CHECK_HORIZONTAL; //* returns the true value which CHECK_HORIZONTAL
				}
				else //If it is not true, it continues checking, and in this case the variables must have initial values
				{
					counter = 0;
					size = 0;
					temp = 0; 
				}
			}
		}
	}
	fclose(fin2);
}

int check_vertical(char* words, int words_size, int* v_coord, int* h_coord)
{
	char rows[LENGTH_EACH_ROW], total[LENGTH_NUMBER_OF_ROW][LENGTH_EACH_ROW];
	int row_size[LENGTH_EACH_ROW], i, j, smallest_row, rows_number = 0, temp = 0, counter = 0, size = 0;
	int min_number = row_size[0];
	FILE* fin2 = fopen("input2.txt", "r");
	while(fscanf(fin2, "%s", rows) != EOF)
	{
		row_size[rows_number] = strlen(rows);
		for(i = 0; i < row_size[rows_number]; i++)
			total[rows_number][i] = rows[i];
		rows_number++;
	}
	for(i = 1; i < rows_number; i++)
		if(row_size[i] < min_number)
			min_number = row_size[i];
		smallest_row = min_number;
		
	for(i = 0; i < smallest_row; i++)
	{
		for(j = 0; j < rows_number; j++)
		{
			if(total[j][i] == words[temp])
			{
				temp = j;
				while(total[temp][i] == words[counter] && size != words_size)
				{
					size++;
					temp++;
					counter++;
				}
				if(size == words_size)
				{
					*v_coord = j + 1; 
					*h_coord = i + 1;
					return CHECK_VERTICAL;
				}
				else
				{
					counter = 0;
					size = 0;
					temp = 0;
				}
			}
		}
	}
	fclose(fin2);
}
