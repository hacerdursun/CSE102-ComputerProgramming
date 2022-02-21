#include<stdio.h>
#include<string.h>
#define SIZE 250

int NewickTree(char array[], FILE *output); 
int i = 0, j = 0, count = 0; /*i, to assign the characters in the file to the array; j, to understand the end of the characters 
in the file; count is to hold '-' numbers*/

int main()
{
	char text[SIZE];
	FILE* finput = fopen("input.txt", "r");
    FILE* foutput = fopen("output.txt", "w");
	while(!feof(finput))
	{
		fscanf(finput, "%c", &text[i]); //I assigned the text in input.txt to the array
		i++;
	}
	fclose(finput);
    i--; //to ignore the character at the end
	NewickTree(text, foutput);
    fclose(foutput);
    
	return 0;
}

int NewickTree(char array[], FILE *output)
{
	int t;
	if(i == j) return 0; //Function ends at the end of the file
	else
	{
		if(array[j] == '(') //When encounters '(', writes '-' to the file
		{
			fprintf(output, "%c", '-');
			count++; //Since '(' as many as '-'
		}
		else if(array[j] == ')')
		{
			count--; /*When we open the parentheses, count variable increases its value and writes '-' as much as its value, 
			but its value should decrease when the parenthesis is closed*/
		} 
		else if(array[j] == ',') //Comma means new line
		{
			fprintf(output, "%c", '\n');
			for(t = 0; t < count; t++)
			{
				fprintf(output, "%c", '-');
			}
		}
		else
		{
			fprintf(output, "%c", array[j]); //Character should be written on the screen if there is no comma and parentheses
		}
	}
	j++; //Next character of array
	NewickTree(array, output); //Function calls itself again
}
