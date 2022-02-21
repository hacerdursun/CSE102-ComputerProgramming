#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILE_LINE_MAX_LENGTH 200 //Maximum length for one line in the Movies.txt 

/* 
 * main function is at the end
 *
 */
 
int numberOfGenres = 0; //Number of different movie genres
char **allGenres = NULL; //Array of movie genres
//Self explanatory movie structure
typedef struct movie_s
{
	double budget; 
    int genre;
    char *name;
    double score;
    int year;
}Movie;

//Updates given movie's properties to given values 
void UpdateMovie(Movie *movieP, double budget, int genre, const char *name, double score, int year)
{
    movieP->budget = budget;
    movieP->genre = genre;
    free(movieP->name);
    movieP->name = (char *) calloc(strlen(name) + 1, sizeof(char));
    strcpy(movieP->name, name);
    movieP->score = score;
    movieP->year = year;
}

//Create a Blank Movie object 
Movie * CreateBlankMovie()
{
    Movie *newBlankMovie = (Movie *) malloc(sizeof(Movie));
    newBlankMovie->budget = 0;
    newBlankMovie->genre = 0;
    newBlankMovie->name = NULL;
    newBlankMovie->score = 0;
    newBlankMovie->year = 0;

    return newBlankMovie;
}

//Create a Movie object with given properties
Movie * CreateMovie(double budget, int genre, const char *name, double score, int year)
{
    Movie *newMovie = (Movie *) malloc(sizeof(Movie));
    newMovie->name = NULL;
    UpdateMovie(newMovie, budget, genre, name, score, year);

    return newMovie;
}

//Get the Movie object from array of Movie objects
Movie * GetMovie(Movie **movieArray, int movieCount, const char *movieName)
{
    Movie *foundMovie = NULL; //Found movie
    while(movieCount--)
        if(!strcmp(movieArray[movieCount]->name, movieName)) //If names are identical
		{   
            foundMovie = movieArray[movieCount]; //Set found movie 
            break; //Break, no need further check
        }

    return foundMovie;
}

//Converts character to integer
int CharToInt(char c)
{
    int i = c - '0'; //Get the difference from '0'
    return i; 
}

//Returns lower of given char
char LowerChar(char c)
{
    if(c < 'a') //If it's before 'a' then it's uppercase
        return c + 32; //'A' + 32 == 'a'
    return c;
}

//Checks if w2 comes after w1 in dictionary, returns 1 if so, 0 otherwise
int IsAfter(const char *w1, const char *w2)
{
    int charIndex; //Character index
    char l1, l2; //Lower chars
    for(charIndex = 0; w1[charIndex] && w2[charIndex]; ++charIndex) //As long as neither word is all checked
	{    
        l1 = LowerChar(w1[charIndex]); //Get lower
        l2 = LowerChar(w2[charIndex]); //Get lower
        if(l1 < l2) //If given first word's current character comes before
            return 1; //then given second word is after
        else if(l1 > l2) //If given first word's current character comes after
            return 0; //then given second word is before if they are the same character, continue
    }
    /*Here can only be reached if all checked characters are the same there's only one remaining condition for w2 to be 
	after, that is w1 being exempted of some of last characters of w2 so '\0' of w1 should be reached if that's the case, 
	if it's so return 1, 0 otherwise*/
	
    return !w1[charIndex];
}

//Resizes movie pointer array to given size
void ResizeMoviePointerArray(Movie ***movieArrayPointer, int size, int newSize)
{
    Movie **newMovieArray = (Movie **) calloc(newSize, sizeof(Movie *)); //Allocate new array
    Movie **movieArray = *movieArrayPointer; //Current array
    int i;
    if(newSize > size) //If new size is greater
	{                         
        for(i = 0; i < size; ++i) //Copy existing pointers
            newMovieArray[i] = movieArray[i];
    }
    else if(newSize < size) //If new size is lesser
	{                    
        for(i = 0; i < newSize; ++i) 
            newMovieArray[i] = movieArray[i]; //Copy existing pointers
        for(i = newSize; i < size; ++i) 
            free(movieArray[i]); //Free left over pointers
    }
    free(movieArray); //Free pointer array
    *movieArrayPointer = newMovieArray; //Set to new pointer array address
}

//Removes given movie pointer from given movie pointer array
void RemoveMovie(Movie ***movieArrayPointer, int size, Movie *movie)
{
	int i;
    Movie **newMovieArray = (Movie **) calloc(size - 1, sizeof(Movie *)); //Allocate new array
    Movie **movieArray = *movieArrayPointer; //Old array
    int displacement = 0; //Displacement of copies of old pointers
    for(i = 0; i < size; ++i)
	{
        if(!strcmp(movieArray[i]->name, movie->name)) //If checked movie is searched movie
            displacement = -1; //Next pointers shall be copied to one before
        else
            newMovieArray[i + displacement] = movieArray[i]; //Copy existing pointers
    }
    free(movieArray); //Free old array
    *movieArrayPointer = newMovieArray; //Set to new address
    //Removed movie is not freed, it's only needed to be removed
}

//Inserts given movie pointer to given movie pointer array
void InsertMovie(Movie ***movieArrayPointer, int size, Movie *newMovie, int insertIndex)
{
	int i;
    ResizeMoviePointerArray(movieArrayPointer, size, size + 1); ++size; //Resize array
    Movie **movieArray = *movieArrayPointer;
    
    for(i = size - 2; i >= insertIndex; --i) //Discplacing old pointers to open space at given index
        movieArray[i + 1] = movieArray[i];
    movieArray[insertIndex] = newMovie; //Put new movie pointer
}

//Adds given movie pointer to given movie pointer array
void AddMovie(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
    ResizeMoviePointerArray(movieArrayPointer, size, size + 1); //Resize
    (*movieArrayPointer)[size] = newMovie; //Put
}

//Binary search movie pointer with it's budget, return right spot for the movie
int MovieBudgetBinarySearch(Movie **movieArrayPointer, int l, int r, Movie *newMovie)
{
    int mid = l + (r - l) / 2; //Mid point
    if(l == r)
        return r; //Return index
    if(movieArrayPointer[mid]->budget > newMovie->budget) //If lesser
        return MovieBudgetBinarySearch(movieArrayPointer, l, mid, newMovie);
    else //If greater
        return MovieBudgetBinarySearch(movieArrayPointer, mid + 1, r, newMovie);
}

//Add movie to given array at correct index due  to its budget
void AddMovieBudgetSorted(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
    int insertIndex = size ? MovieBudgetBinarySearch(*movieArrayPointer, 0, size - 1, newMovie) : 0; 
    InsertMovie(movieArrayPointer, size, newMovie, insertIndex);
}

//Binary search movie pointer with it's budget, return right spot for the movie
int MovieGenreBinarySearch(Movie **movieArrayPointer, int l, int r, Movie *newMovie)
{
    int mid = l + (r - l) / 2;
    if(l == r)
        return r;
    if(movieArrayPointer[mid]->genre > newMovie->genre)
        return MovieGenreBinarySearch(movieArrayPointer, l, mid, newMovie);
    else
        return MovieGenreBinarySearch(movieArrayPointer, mid + 1, r, newMovie);
}

//Add movie to given array at correct index due  to its budget
void AddMovieGenreSorted(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
	// Insert at 0 if size is 0
    int insertIndex = size ? MovieGenreBinarySearch(*movieArrayPointer, 0, size - 1, newMovie) : 0; 
    InsertMovie(movieArrayPointer, size, newMovie, insertIndex);
}

//Binary search movie pointer with it's budget, return right spot for the movie
int MovieNameBinarySearch(Movie **movieArrayPointer, int l, int r, Movie *newMovie)
{
    int mid = l + (r - l) / 2;
    if(l == r)
        return r;
    if(IsAfter(movieArrayPointer[mid]->name, newMovie->name))
        return MovieNameBinarySearch(movieArrayPointer, mid + 1, r, newMovie);
    else
        return MovieNameBinarySearch(movieArrayPointer, l, mid, newMovie);
}

//Add movie to given array at correct index due  to its budget
void AddMovieNameSorted(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
	//Insert at 0 if size is 0
    int insertIndex = size ? MovieNameBinarySearch(*movieArrayPointer, 0, size - 1, newMovie) : 0;    
    InsertMovie(movieArrayPointer, size, newMovie, insertIndex);
}

//Binary search movie pointer with it's budget, return right spot for the movie
int MovieScoreBinarySearch(Movie **movieArrayPointer, int l, int r, Movie *newMovie)
{
    int mid = l + (r - l) / 2;
    if(l == r)
        return r;
    if(movieArrayPointer[mid]->score > newMovie->score)
        return MovieScoreBinarySearch(movieArrayPointer, l, mid, newMovie);
    else
        return MovieScoreBinarySearch(movieArrayPointer, mid + 1, r, newMovie);
}

//Add movie to given array at correct index due  to its budget
void AddMovieScoreSorted(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
    int insertIndex = size ? MovieScoreBinarySearch(*movieArrayPointer, 0, size - 1, newMovie) : 0;    
    InsertMovie(movieArrayPointer, size, newMovie, insertIndex);
}

//Binary search movie pointer with it's budget, return right spot for the movie
int MovieYearBinarySearch(Movie **movieArrayPointer, int l, int r, Movie *newMovie)
{
    int mid = l + (r - l) / 2;
    if(l == r)
        return r;
    if(movieArrayPointer[mid]->year > newMovie->year)
        return MovieYearBinarySearch(movieArrayPointer, l, mid, newMovie);
    else
        return MovieYearBinarySearch(movieArrayPointer, mid + 1, r, newMovie);
}

//Add movie to given array at correct index due  to its budget
void AddMovieYearSorted(Movie ***movieArrayPointer, int size, Movie *newMovie)
{
    int insertIndex = size ? MovieYearBinarySearch(*movieArrayPointer, 0, size - 1, newMovie) : 0;  
    InsertMovie(movieArrayPointer, size, newMovie, insertIndex);
}

//Reads budget from line, returns index of next comma
int ReadBudget(const char *line, double *budget)
{
    double readBudget = 0;
    int commaIndex;
    for(commaIndex = 0; line[commaIndex] != ','; ++commaIndex); //Get comma index
    if(line[0] != 'u') //If starting character is not 'u', thus it's not "unknown"
        sscanf(line, "%lf", &readBudget);
    *budget = readBudget; //Update budget
    return commaIndex;
}

//Reads genre from line, return index of next comma
int ReadGenre(const char *line, int *genre)
{
    int i, commaIndex, genreIndex;
    for(commaIndex = 0; line[commaIndex] != ','; ++commaIndex); //Get comma index
    for(genreIndex = 0; genreIndex < numberOfGenres; ++genreIndex) //Check if any existing genre is identical to genre in given line
	{ 
        if(!strncmp(allGenres[genreIndex], line, commaIndex)) //If identical
		{      
            *genre  = genreIndex; //Set genre to genre's index
            return commaIndex; //Return comma index
        }
    }
    //If genre is new
    //Temp array to store existing genres
    char **temp = (char **) calloc(numberOfGenres + 1, sizeof(char *));
    //Copy existing genres to temp and free actual array
    for(i = 0; i < numberOfGenres; ++i)
	{
    	temp[i] = (char *) calloc(strlen(allGenres[i]) + 1, sizeof(char));
        strcpy(temp[i], allGenres[i]);
        free(allGenres[i]);
    }
    free(allGenres); //Free array of pointers
    allGenres = (char **) calloc(numberOfGenres + 1, sizeof(char *)); //Allocate with one greater size for new genre
    allGenres[numberOfGenres] = (char *) calloc(commaIndex + 1, sizeof(char)); //Allocate last pointer directly for new genre
    //Copy existing genres from temp and free temp
    for(i = 0; i < numberOfGenres; ++i)
	{
        allGenres[i] = (char *) calloc(strlen(temp[i]) + 1, sizeof(char));
        strcpy(allGenres[i], temp[i]);
        free(temp[i]);
    }
    free(temp); //Free temp array
    strncpy(allGenres[numberOfGenres], line, commaIndex); //Copy new genre
    *genre = numberOfGenres; //Set genre
    ++numberOfGenres; //Increase number of genres by one
    return commaIndex; //Return comma index
}

//Read name from line, returns next comma index
int ReadName(const char *line, char **name)
{
    int commaIndex;
    for(commaIndex = 0; line[commaIndex] != ','; ++commaIndex); //Get comma index
    free(*name); //Deallocate old name just in case
    *name = (char *) calloc(commaIndex + 1, sizeof(char)); //Allocate for the read name
    strncpy(*name, line, commaIndex); //Copy read name
    return commaIndex; //Return comma index
}

//Read score from line, returns next comma index
int ReadScore(char *line, double *score)
{
    int commaIndex;
    for(commaIndex = 0; line[commaIndex] != ','; ++commaIndex); //Get comma index
    sscanf(line, "%lf", score); //Set score
    return commaIndex; //Return comma index
}

//Reads year from line
void ReadYear(char *line, int *year)
{
    sscanf(line, "%d", year);
}

void PrintMovie(Movie *movie); //Forward declaration of PrintMovie to use ine ReadMovieData

//Reads given file ("movie.txt" in this case) and handles data, sorts the data in corresponding arrays that are given
int ReadMovieData(char *fileName, Movie ***movieArrayPointer, Movie ***sortedByBudget, Movie ***sortedByGenre, 
				    					 Movie ***sortedByName, Movie ***sortedByScore, Movie ***sortedByYear)
{
    FILE *file = fopen(fileName, "r"); 
    char line[FILE_LINE_MAX_LENGTH]; //Read line from file
    //Properties of a movie
    double budget;
    int genre;
    char *name = NULL;
    double score;
    int year;
    int commaIndex; //Index of next comma in the line
    int movieCount = 0; //Number of movies there
    Movie *newMovie = NULL; //New movie 
    fgets(line, FILE_LINE_MAX_LENGTH, file); //Not a real data line, just an indicator
    
    //Until file is fully read
    while(fgets(line, FILE_LINE_MAX_LENGTH, file))
	{
        //Read properties
        commaIndex = 0;
        commaIndex += ReadBudget(line, &budget);
        commaIndex += ReadGenre(line + commaIndex + 1, &genre) + 1;
        commaIndex += ReadName(line + commaIndex + 1, &name) + 1;
        commaIndex += ReadScore(line + commaIndex + 1, &score) + 1;
        ReadYear(line + commaIndex + 1, &year);
        //Check if movie exists
        newMovie = GetMovie(*movieArrayPointer, movieCount, name);
        //If it does not
        if(newMovie == NULL)
		{
            //Create movie and add to movie array
            newMovie = CreateMovie(budget, genre, name, score, year);   
            AddMovie(movieArrayPointer, movieCount, newMovie);
            //Add to sorted arrays
            AddMovieBudgetSorted(sortedByBudget, movieCount, newMovie);
            AddMovieGenreSorted(sortedByGenre, movieCount, newMovie);
            AddMovieNameSorted(sortedByName, movieCount, newMovie);
            AddMovieScoreSorted(sortedByScore, movieCount, newMovie);
            AddMovieYearSorted(sortedByYear, movieCount, newMovie);
            //Increase movie count
            ++movieCount;
        }
        //If there's already a movie named that
        else
		{
            //to maintain order in sorted arrays old one will be removed
            RemoveMovie(sortedByBudget, movieCount, newMovie);
            RemoveMovie(sortedByGenre, movieCount, newMovie);
            RemoveMovie(sortedByName, movieCount, newMovie);
            RemoveMovie(sortedByScore, movieCount, newMovie);
            RemoveMovie(sortedByYear, movieCount, newMovie);
            --movieCount;
            //It will be updated to new properties
            UpdateMovie(newMovie, budget, genre, name, score, year);
            //And will be added back properly
            AddMovieBudgetSorted(sortedByBudget, movieCount, newMovie);
            AddMovieGenreSorted(sortedByGenre, movieCount, newMovie);
            AddMovieNameSorted(sortedByName, movieCount, newMovie);
            AddMovieScoreSorted(sortedByScore, movieCount, newMovie);
            AddMovieYearSorted(sortedByYear, movieCount, newMovie);
            ++movieCount;
        }
    }
    /*It should because of something about binary search functions but since I'm unable to put my finger on it, instead 
	I solved the "first added movie is at last index in every sorted array" problem this way*/
	
    //movieArrayPointer is not sorted, instead in order of addition so get first added movie
    Movie *misplacedMovie = (*movieArrayPointer)[0];
    //Remove from sorted arrays
    RemoveMovie(sortedByBudget, movieCount, misplacedMovie);
    RemoveMovie(sortedByGenre, movieCount, misplacedMovie);
    RemoveMovie(sortedByName, movieCount, misplacedMovie);
    RemoveMovie(sortedByScore, movieCount, misplacedMovie);
    RemoveMovie(sortedByYear, movieCount, misplacedMovie);
    --movieCount;
    //Add to sorted arrays properly
    AddMovieBudgetSorted(sortedByBudget, movieCount, misplacedMovie);
    AddMovieGenreSorted(sortedByGenre, movieCount, misplacedMovie);
    AddMovieNameSorted(sortedByName, movieCount, misplacedMovie);
    AddMovieScoreSorted(sortedByScore, movieCount, misplacedMovie);
    AddMovieYearSorted(sortedByYear, movieCount, misplacedMovie);
    ++movieCount;
    fclose(file);       
    return movieCount; //Return number of movies
}

//Prints properties of given movie
void PrintMovie(Movie *movie)
{
    //Just some basic pretty printings, no explanation needed
    int numberOfPrintedChars;
    if(movie->budget)
        numberOfPrintedChars = printf("%.0lf", movie->budget);
    else
        numberOfPrintedChars = printf("unknown");
    printf("%*c", 15 - numberOfPrintedChars, ' ');
    numberOfPrintedChars = printf("%s", allGenres[movie->genre]);
    printf("%*c", 15 - numberOfPrintedChars, ' ');
    numberOfPrintedChars = printf("%s", movie->name);
    printf("%*c", 75 - numberOfPrintedChars, ' ');
    numberOfPrintedChars = printf("%.1lf", movie->score);
    printf("%*c", 10 - numberOfPrintedChars, ' ');
    numberOfPrintedChars = printf("%d\n", movie->year);
}

//Menu for the first operation for the main menu
void SortedMenu(int numberOfMovies, Movie **sortedByBudget, Movie **sortedByGenre, Movie **sortedByName, 
															Movie **sortedByScore, Movie **sortedByYear)
{   
    int i, userCommand = 0;
    while(1)
	{
        printf("\n");
        printf("1. Budget\n");
        printf("2. Genre\n");
        printf("3. Name\n");
        printf("4. Score\n");
        printf("5. Year\n");
        printf("\n");
        printf("Select an operation: ");
        scanf("%d", &userCommand);
        int tryAgain = 0;           
        int startIndex, endIndex; //Start and end indexes for movie prints
        Movie **chosenMovieArray = NULL; //Corresponding array to be printed
        //Set corresponding sorted array
        switch(userCommand)
		{
            case 1:
                chosenMovieArray = sortedByBudget;
                break;
            case 2:
                chosenMovieArray = sortedByGenre;
                break;
            case 3:
                chosenMovieArray = sortedByName;
                break;
            case 4:
                chosenMovieArray = sortedByScore;
                break;
            case 5:
                chosenMovieArray = sortedByYear;
                break;
            default:
                printf("\nTRY AGAIN.\n");
                tryAgain = 1;
        }
        //If successfully selected
        if(!tryAgain)
		{
            tryAgain = 0;
            printf("1. Single Selection\n");
            printf("2. Multiple Selection\n");
            printf("\n");
            printf("Select an operation: ");
            scanf("%d", &userCommand);
            //Set start and end indexes to user's desires
            switch(userCommand)
			{
                case 1:
                    printf("Enter value: ");
                    scanf("%d", &startIndex);
                    endIndex = startIndex + 1; 
                    break;
                case 2:
                    printf("Enter start value: ");
                    scanf("%d", &startIndex);
                    printf("Enter end value: ");
                    scanf("%d", &endIndex); 
                    break;
                default:
                    printf("\nTRY AGAIN.\n");
                    tryAgain = 1;
            }
            //If successfully selected
            if(!tryAgain)
			{
                --startIndex; //Array indexes start from 0 not 1
                if(startIndex < 0 || startIndex >= numberOfMovies || endIndex < 0 || endIndex > numberOfMovies){
                    printf("Invalid indexes. There's only %d number of movies and start from 1.\n", numberOfMovies);
                    tryAgain = 1;
                    continue;
                }
                for(i = startIndex; i < endIndex; ++i) //Print every movie throughout start and index
                    PrintMovie(chosenMovieArray[i]);
                printf("\n");
                break;
            }
        }
    }
}

//Menu for the third operation for the main menu
void ListThroughYears(Movie **sortedByYear, int size)
{
    int i, year, since, isPrinted = 0;
    while(!isPrinted)
	{
        printf("Enter a year:\n");
        scanf("%d", &year);
        printf("Until (0) or Since (1) %d:", year);
        scanf("%d", &since);
        if(since) //If since is selected
		{                                  
            int i;
            for(i = 0; i < size; ++i) //Get to entered year
                if(sortedByYear[i]->year >= year)
                    break;
            for(; i < size; ++i) //Print starting from there
			{                   
                PrintMovie(sortedByYear[i]);
                isPrinted = 1;
            }
        }
        else //If until is selected
		{                                       
            for(i = 0; i < size; ++i)
			{
                if(sortedByYear[i]->year >= year) //If a year greater or equal to entered year is reached break
                    break;
                PrintMovie(sortedByYear[i]);        
                isPrinted = 1;
            }
        }
		if(!isPrinted)
			printf("\nTRY AGAIN\n");
    }
}

//Menu for the forth operation for the main menu
void ListThroughScores(Movie **sortedByScore, int size)
{
    double score;
    int i, greater, isPrinted = 0;
    while(!isPrinted)
	{
        printf("Enter a score:\n");
        scanf("%lf", &score);
        printf("Less (0) or Greater (1) %.1lf:", score);
        scanf("%d", &greater);
        if(greater)
		{
            int i;
            for(i = 0; i < size; ++i)
                if(sortedByScore[i]->score >= score)
                    break;
            for(; i < size; ++i)
			{
                PrintMovie(sortedByScore[i]);
                isPrinted = 1;
            }
        }
        else
		{
            for(i = 0; i < size; ++i)
			{
                if(sortedByScore[i]->score >= score)
                    break;
                PrintMovie(sortedByScore[i]);
                isPrinted = 1;
            }
        }
        if(!isPrinted)
            printf("\nTRY AGAIN\n");
    }
}

//Main menu of the program
void Menu(int numberOfMovies, Movie **movieArrayPointer, Movie **sortedByBudget, Movie **sortedByGenre, 
									 Movie **sortedByName, Movie **sortedByScore, Movie **sortedByYear)
{
    int i, userCommand = 0;
    double avgScore = 0.0;
    char userStringInput[100];
    Movie *searchedMovie = NULL;
    int *frequencyOfGenres = (int *) calloc(numberOfGenres, sizeof(int));
    *frequencyOfGenres = 0;
    //Until user enters 8
    while(userCommand != 8)
	{
    	printf("\n");
        printf("1. List of the Sorted Data\n");
        printf("2. List of the Genres\n");
        printf("3. List of the Movie Through the Years\n");
        printf("4. List of the Movie Through the Scores\n");
        printf("5. All informations of a Single Movie\n");
        printf("6. Average of IMDB Scores\n");
        printf("7. Frequence of the Genres\n");
        printf("8. Exit\n");
        printf("\n");
        printf("Select an operation: ");
        scanf("%d", &userCommand);
        printf("\n");
        //Start corresponding process
        switch(userCommand)
        {
            case 1:
                SortedMenu(numberOfMovies, sortedByBudget, sortedByGenre, sortedByName, sortedByScore, sortedByYear);
                break;
            case 2:
                for(i = 0; i < numberOfGenres; ++i)
                    printf("%s\n", allGenres[i]);
                break;
            case 3:
                ListThroughYears(sortedByYear, numberOfMovies);
                break;
            case 4:
                ListThroughScores(sortedByScore, numberOfMovies);
                break;
            case 5:
                //Get movie name
                printf("Please enter the name of the movie:\n");
                getchar();
                scanf("%[^\n]", userStringInput);
                printf("\n");
                //Get movie
                searchedMovie = GetMovie(movieArrayPointer, numberOfMovies, userStringInput);
                //Inform
                if(searchedMovie != NULL)
				{
                    if(searchedMovie->budget) 
                        printf("Budget: %.0lf\n", searchedMovie->budget);
                    else 
                        printf("Budget: unknown\n");
                    printf("Genre:  %s\n", allGenres[searchedMovie->genre]);
                    printf("Name:   %s\n", searchedMovie->name);
                    printf("Score:  %.1lf\n", searchedMovie->score);
                    printf("Year:   %d\n", searchedMovie->year);
                }
                else 
                    printf("Not found.\n");
                break;
            case 6:
                //If average score yet to be calculated
                if(!avgScore){
                    for(i = 0; i < numberOfMovies; ++i) 
                        avgScore += movieArrayPointer[i]->score;
                    avgScore /= numberOfMovies;
                }
                printf("Average: %0.6lf\n", avgScore);
                break;
            case 7:
                //If frequency of genres yet to be calculated
                if(!*frequencyOfGenres){
                    for(i = 0; i < numberOfMovies; ++i)
                        ++frequencyOfGenres[movieArrayPointer[i]->genre];
                }
                //Print
                for(i = 0; i < numberOfGenres; ++i){
                    int printedCharCount = printf("%s", allGenres[i]);
                    printf("%*c%d\n", 20 - printedCharCount, ' ', frequencyOfGenres[i]);
                }
                break;
            case 8:
                break;
            default:
                printf("\nTRY AGAIN.\n");
        }
    }
}

int main()
{
    //Arrays
    Movie **movieArray = NULL;
    Movie **movieArraySortedByBudget = NULL;
    Movie **movieArraySortedByGenre = NULL;
    Movie **movieArraySortedByName = NULL;
    Movie **movieArraySortedByScore = NULL;
    Movie **movieArraySortedByYear = NULL;
    //Read and handle data
    int numberOfMovies = ReadMovieData("Movies.txt", &movieArray, &movieArraySortedByBudget, &movieArraySortedByGenre, &movieArraySortedByName, &movieArraySortedByScore, &movieArraySortedByYear);
    //Execute menu
    Menu(numberOfMovies, movieArray, movieArraySortedByBudget, movieArraySortedByGenre, movieArraySortedByName, movieArraySortedByScore, movieArraySortedByYear);

    return 0;
}
