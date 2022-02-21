/*
** hw2_lib.c:
**
** The source file implementing library functions.
**
*/
#include<stdio.h>
#include<math.h>
#include"hw2_lib.h"

/*1: Monday, 2: Tuesday, ..., 7: Sunday. I used the first day of the calendar as a reference and then calculated the 
total days. I divided the number of days I have found by 7 to find out which day it equal. I used the date / time part of the 
computer to check the accuracy of the program. Many sites, for example, took the year 1800 as a leap year and
therefore does not give accurate results.*/
int find_weekday_of_data(int day, int month, int year)
{
	if(day > 31 || month > 12 || day < 0 || month < 0 || year < 0) /*It checks if a reasonable entry has been made*/
	{
		printf("Invalid date!\n");
		return 0;
	}
	else
	{
		int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /*Number of days in one year (if not a leap year)*/
		int days2[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /*Number of days in one year (if it is a leap year)*/
		int i, leap_year = 0, day_of_the_week, total_days;
		for(i = 0; i < year; i++) /*Calculates the number of leap years up to the year entered*/
		{
			if( (i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
			leap_year++;
		}
		total_days = (leap_year) * 366 + (year - leap_year) * 365; /*It calculates the sum of the days until that day. I multiplied 
		the leap years number by 366 and the remaining years by 365*/
		for(i = 1; i < month; i++) /*The number of days of the month up to that month is added*/
		{
			if( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) && i == 2) /*If it's a leap year and the month is equal to February*/
				total_days += days2[i-1]; /*29 is added*/
			else
				total_days += days[i-1]; /*28 is added*/
		}
		total_days += day;
		day_of_the_week = (total_days-2) % 7; /*To print the days as integer numbers*/
		if(day_of_the_week == 0) /*When the number of days is 7, it prints 0 because the remainder is 0. But it should print 7*/
			day_of_the_week = 7;
		
		return day_of_the_week;
	}
}
/*Many sites have incorrect results. (For example, since the year 1800, which is not a leap year, is calculated as a leap 
year, or the start or end dates are also calculated.) I calculated the result using the calendar on the computer for the 
accuracy of the program. ! I also did not count start and end dates! I ignored them because you said between two dates. 
(Near the end line) int result = end-start-1; If -1 is deleted in the line, the starting date is also taken into account.
Summary:I made the calculation by reference to the starting date. For example, if the user entered the year 2019, I added 
the days before 2019, and while doing this, I found the leap years before 2019 (multiplied by 366) and non-leap years 
(multiplied by 365). I applied the same procedure for the month. I collected the days up to that month in the entered year. 
(Because if the user entered the year 2019, I did not calculate the days in 2019 at all and used the month to calculate the
days in 2019) Then I added the day to the total number of days. Then I found the distances of the entered dates to the 
starting date, and finally I found the distance between these two dates by subtracting them from each other.*/

/*Calculates the distance of the entered date from the start date*/
int find_total_days(int day, int month, int year) 
{
	int leap_months[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /*Number of days in 1 year (if not a leap year)*/
	int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /*Number of days in 1 year (if it is a leap year)*/
	int i, leap_year = 0, total_days = 0, sum_of_months = 0;
	for(i = 1; i < year; i++) /*Total number of days up to the year entered*/
	{
		if( (i % 4 == 0 && i % 100 != 0) || i % 400 == 0) /*Is the year a leap year?* */
		leap_year++;
	}
	total_days = (leap_year) * 366 + ( (year) - leap_year) * 365; /*Finds the total number of days based on leap year.*/
	if( (year % 4 == 0 && year % 100 != 0) || year % 400 == 0) /*In leap years, February is 29 days, I have used two separate array for
	this and summed the months by year*/
	{
		for(i = 1; i < month; i++)
			sum_of_months += leap_months[i - 1];
	}
	else
	{
		for(i = 1; i < month; i++)
			sum_of_months += months[i - 1];
	}
	total_days = total_days + sum_of_months + day; 
	
	return total_days;
}

int count_day_between_dates(int start_day, int start_month, int start_year, int end_day, int end_month, int end_year)
{
	int start, end, result;
	if(start_day > 31 || start_month > 12 || end_day > 31 || end_month > 12 || start_day < 0 || start_month< 0 
					  || start_year < 0 || end_day < 0 || end_month < 0 || end_year < 0)
	{
		printf("Invalid date!\n");
		return 0;
	}
	start = find_total_days(start_day, start_month, start_year);
	end = find_total_days(end_day, end_month, end_year);
	result = end - start - 1;
	
	return result;
}

/*Summary: Let A be the angle opposite to length a, B be the angle opposite to length b and C be the angle opposite to length c.
The median drawn from point 'B' and intersecting on the edge of 'AC', I called 'D'.The median drawn from point 'C' 
and intersecting on the edge of 'AB', I called 'E'. These medians intersect at 'G'. 'G' divides the median into 2:1 ratio. 
I found the BGC angle by calculating the median lengths CE and BD. 
CE= 1/2*sqrt(2a^2+2b^2-c^2)
BD=1/2*sqrt(2a^2+2c^2-b^2).Since we know that G divides the median by the ratio of 2: 1, we can find the lengths CG and BG
BG=2/3*1/2*sqrt(2a^2+2c^2-b^2)=1/3*sqrt(2a^2+2c^2-b^2)
CG=2/3*1/2*sqrt(2a^2+2b^2-c^2)=1/3*sqrt(2a^2+2b^2-c^2). After,
cosBGC=CG^2+BG^2-a^2/2*CG*BG. Then I came up with this formula:
alpha=BGC=cos^-1(b^2+c^2-5a^2/2*sqrt*(2a^2+2b^2-c^2)*(2a^2+2c^2-b^2)).
I used Apollonius's and cos theorem.*/
double find_angle(double a, double b, double c)
{
	double first, second, third, last, result, end;
	first = (b * b) + (c * c) - (5 * a * a);
	second = (2 * a * a) + (2 * b * b) - (c * c);
	third = (2 * a * a) + (2 * c * c) - (b * b);
	last = 2 * sqrt(second * third);
	result = first / last;
	end = acos(result);
	
	return end;
}

/*Integers entered for the first column of the table can have a maximum of 5 digits*/
int find_digit(unsigned int number) 
{
	int i, digit = 1, temp;
	temp = number;
	while(temp >= 10)
	{
		temp = temp / 10;
		digit++;
	}
	if(digit > 5)
	{
		printf("The numbers for the first column cannot exceed 5 digits!\n");
		return 0;
	}
	else 
		return number;
}

/*Checks whether the number entered for the second column is written as an integer*/
int row2(double number) 
{
	int new_value = (int)number;
	double result = number - new_value;
	if(result == 0)
		return 1;
	else
		return 0;
}

/*I created a separate function for the repeating part of the table (for the * or - line)*/
void print_line(char c)
{
	int i;
	for(i = 0; i < 121; i++)
	{
		printf("%c",c);
	}
	printf("\n");
}

void print_tabulated(unsigned int r11, double r12, int r13, 
                     unsigned int r21, double r22, int r23, 
                     unsigned int r31, double r32, int r33, char border)
{
	int i, newr11, newr21, newr31, newr12, newr22, newr32; /*For first and second column checks*/
	/*5-digit control for the first column..*/
	newr11 = find_digit(r11); 
	newr21 = find_digit(r21);
	newr31 = find_digit(r31);
	
	/*Integer check for the second column*/
	newr12 = row2(r12); 
	newr22 = row2(r22);
	newr32 = row2(r32);
	
	/*For the header row.*/
	for(i = 0; i < 121; i++)
	{
		if(i == 0)
			printf("┌");
		else if(i == 120)
			printf("┐");
		else
			printf("%c",border);
	}
	printf("\n");
	printf("|\t\t%s\t\t\t|\t\t%s\t\t|\t\t%s\t\t|\n", "Row 101", "Row ABCDEFG", "Row XYZ123");
	print_line(border);
	
	if(r13 > 0) /*It is checked to write the sign of the number in the last column. If positive, adds +*/
	{
		if(newr12 == 1) /*If the number in the second column can be written as an integer, it converts it to an integer*/
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr11, (int)r12, r13);
			/*I did the first column check, can the number be written as an integer? In the main function, the parameter 
			is r11, I sent this parameter to the function I wrote(find_digit), and I met the result with newr11. That's why I used the variable
			newr11 when printing to the screen. So I checked all three columns*/
		else if(newr12 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr11, r12, r13);
	}
	else /*If the number in the last column is negative, it only writes the number(The number has a minus, no need to write a 
	minus again)*/
	{
		if(newr12 == 1)
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|%d\t\t\t\t\t|\n", newr11, (int)r12, r13);
		else if(newr12 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|%d\t\t\t\t\t|\n", newr11, r12, r13);
	}
	print_line(border); /*Writes the repeating part in the table*/
	
	if(r23 > 0) /*Third column control.*/
	{
		if(newr22 == 1) /*Second column control*/
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr21, (int)r22, r23);
		else if(newr22 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr21, r22, r23);
	}
	else
	{
		if(newr22 == 1)
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|%d\t\t\t\t\t|\n", newr21, (int)r22, r23);
		else if(newr22 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|%d\t\t\t\t\t|\n", newr21, r22, r23);
	}
	print_line(border);
	
	if(r33 > 0)/*Third column control*/
	{
		if(newr32 == 1) /*Second column control*/
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr31, (int)r32, r33);
		else if(newr32 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|+%d\t\t\t\t\t|\n", newr31, r32, r33);
	}
	else
	{
		if(newr32 == 1)
			printf("|\t\t%d\t\t\t|%d\t\t\t\t\t|%d\t\t\t\t\t|\n", newr31, (int)r32, r33);
		else if(newr32 == 0)
			printf("|\t\t%d\t\t\t|%.2f\t\t\t\t\t|%d\t\t\t\t\t|\n", newr31, r32, r33);
	}
	
	/*For the last row*/
	for(i = 0; i < 121; i++)
	{
		if(i == 0)
			printf("└");
		else if(i == 120)
			printf("┘");
		else 
			printf("%c",border);
	}
	printf("\n");
}
