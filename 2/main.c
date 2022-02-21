#include<stdio.h>
#define amount 10 //Array size
#define p1 0.5
#define p2 20

int main()
{
	float a = 0.0, b = 0.0, n = 0.0; 
	float numbers[amount];/*As the pdf says, I kept the numbers in the array because this assignment is 
	about file read/write and array usage*/
	int i = 0, j; //i, to assign numbers to the array; j is for the loop when writing numbers to the file
	
	FILE *fp; //to read the numbers
	fp = fopen("input.txt", "r");
	while(!feof(fp))
	{
		fscanf(fp, "%f", &numbers[i]); //I've assigned the numbers in the file to the array
		i++;		
	}
	fclose(fp);

	FILE *f; //to write the numbers
	f = fopen("output.txt", "w");
	double c = 2.0, atotal = 0.0;
	a = numbers[0]; //Starting number of chunk
	atotal = a; //Because the number of numbers and numbers added each time will change
	fprintf(f, "%.4f ", a); //Writes the first number to the file
	for(j = 1; j < amount; j++) 
	/*Controls numbers until the end of the array, but the control loop begins with the second
	element of the array because it writes the first number to the file*/
	{
		n = numbers[j]; //The number to add with a
		atotal += n; //for average
		b = (atotal) / c; //Average after n is added
		if(!(b > a * (1 + p1) || b < a * (1 - p1) || a > n * p2 || a < n / p2)) 
		{
			fprintf(f, "%.4f ", n); //n number is added if the condition is provided
			a = b; //Stores the average of the chunk before n is added for the next operation
			c++; //Because the number of added is increasing, 'c' should increase while taking averages
		}
		else
		{
			fprintf(f, "average: %.4f", a);
			fprintf(f, "\n");
			fprintf(f, "%.4f ", n);
			a = numbers[j]; 
			atotal = numbers[j];
			c = 2.0;
			/*The chunk is reset, so calculations will start over; c = 2 because a + n / 2 ==> a + n / 2 */
		}
	}
	fprintf(f, "average: %.4f", a);
	fclose(f);
}
