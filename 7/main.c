#include <stdio.h>

char lower_char(char c) //Return lowercase version of the given char
{
    return c < 97 ? c + 32 : c;
}

int is_char_vowel(char c) //Return true if given char is a vowel
{
    int isVowel = 0; //Is char vowel
    if(c % 2)
    {          
        switch(lower_char(c)) //All vowels have odd ascii value, I filtered that
        {
            case 97: //a
            case 101: //e
            case 105: //i
            case 111: //o
            case 117: //u
                isVowel = 1;
                break;
        }
    }
    return isVowel;
}

void copy_word(char *to, char *from, int len) //Copies word from one to another char array, maximum of given number of characters
{
    int i;
    for(i = 0; i < len && from[i] != 0; ++i) to[i] = from[i]; //Copy as long as copied word or number limit is not finished
    if(i != len) to[i] = 0; //Mark the end
}

int word_len(char *word) //Returns the length of the given word
{
    int len = 0;
    while(word[len]) ++len;
    return len;
}

void append_words(char *to, char *from) //Puts given word from at the end of the to
{
    int i = 0, j = 0;
    while(to[i]) ++i;
    while(from[j]) to[i++] = from[j++];
    to[i] = 0;
}

/*Compares given words characters, tracks the number of characters that are different at same index, if given two words are 
different in length that difference is also added to the 'difference' number,returns that number.*/
int compare_words(char *w1, char *w2, int *consonantToVowelCount)
{
    int i, j, differentCharCount = 0; //Number of different characters at same indexes in each word
    *consonantToVowelCount = 0; //Number of characters at index n that are consonant in w1 and vowel in w2
    for(i = 0, j = 0; w1[i] != 0 || w2[j] != 0;) //Go as long as neither word's end is reached
    {
        if(lower_char(w1[i]) != lower_char(w2[j])) //If characters are different
        {       
            ++differentCharCount; //Increment corresponding tracker variable
            if(!is_char_vowel(w1[i]) && is_char_vowel(w2[j])) ++(*consonantToVowelCount); /*If character in w1 is consonant
			and one in w2 is vowel. Increment corresponding variable.*/
        }
        i += w1[i] != 0; //Increment index if word's end is not reached
        j += w2[j] != 0;
    }

    return differentCharCount;
}

/*Compares words with 1 character of difference in length, idea is one may make a typo where they type one more or less character 
accidently, w1 is 1 character longer than w2.*/ 
int compare_words1(char *w1, char *w2, int *consonantTowVowelCount)
{
    char curWord[20]; //Current form of the w1
    int minDifference = 20, maxConsonantToVowel = 0; //Min difference and max consonant to vowel count amongst w1's forms
    int curDifference, curConsonantToVowel; //Current difference and consonant to vowel count of current form
    int i, j, k; //Iterator variables
    for(i = 0; w1[i]; ++i) /*Forms of w1 is obtained by deleting one character from w1 so there are forms as many as 
	characters of w1.*/
    { 
        for(j = 0, k = 0; w1[j]; ++j) //Write form to curWord
            if(j != i) curWord[k++] = w1[j];
        curWord[k] = 0;
        curDifference = compare_words(curWord, w2, &curConsonantToVowel); //Get difference of the form of w1 and w2
        if(!curDifference) //If they are the same don't waste time with other forms
        {
            minDifference = curDifference;
            maxConsonantToVowel = curConsonantToVowel;
            break;
        }
        if(curDifference < minDifference) //If this form's difference is lesser
        {
            curDifference = minDifference;
            maxConsonantToVowel = curConsonantToVowel;
        }
        //Or difference is the same but this one has more consonants that turned into vowels
        else if(curDifference == minDifference && curConsonantToVowel > maxConsonantToVowel)
        {   
            maxConsonantToVowel = curConsonantToVowel;
        }
    }
    *consonantTowVowelCount = maxConsonantToVowel; //Return consonant to vowel count
    return minDifference; //Return difference
}

/*Same idea with funcion compare_words1 but this time there's 2 number of character difference in given words
w1 is 2 characters longer than w2.*/
int compare_words2(char *w1, char *w2, int *consonantTowVowelCount)
{
    char curWord[20]; //Current form of the w1
    int minDifference = 20, maxConsonantToVowel = 0; //Min difference and max consonant to vowel count amongst w1's forms
    int curDifference, curConsonantToVowel; //Current difference and consonant to vowel count of current form
    int i, j, k; //Iterator variables
    for(i = 0; w1[i]; ++i) //Forms of w1 is obtained by deleting one character from w1 so there are forms as many as characters of w1.*/
    { 
        for(j = 0, k = 0; w1[j]; ++j) //Write form to curWord
            if(j != i) curWord[k++] = w1[j];
        curWord[k] = 0;

        /*Get difference of the form of w1 and w2, we use compare_words1 to eliminate second difference of character length.*/
        curDifference = compare_words1(curWord, w2, &curConsonantToVowel);
        if(!curDifference) //If they are the same don't waste time with other forms
        {
            minDifference = curDifference;
            maxConsonantToVowel = curConsonantToVowel;
            break;
        }
        if(curDifference < minDifference) //If this form's difference is lesser
        {
            curDifference = minDifference;
            maxConsonantToVowel = curConsonantToVowel;
        }
        //Or difference is the same but this one has more consonants that turned into vowels
        else if(curDifference == minDifference && curConsonantToVowel > maxConsonantToVowel)
        {   
            maxConsonantToVowel = curConsonantToVowel;
        }
    }
    *consonantTowVowelCount = maxConsonantToVowel; //Return consonant to vowel count
    return minDifference; //Return difference
}

/*Finds best candidate for misspelled word with max of 1 number of character difference in length, writes in given 
char array foundWord.*/
void find_best_correction1(char *dictionaryFileName, char *word, char *foundWord)
{
    FILE *dictionaryFile = fopen(dictionaryFileName, "r"); //Dictionary that will be used
    //Result word and read word
    char leastDifferentWord[20], curWord[20];
    int minDifference = 20, maxConsonantToVowel = 0; //Result word's properties
    int curDifference, curConsonantToVowel; //Read word's properties
    int wordLen = word_len(word);
    int curWordLen;
    int occurance = 0; //Variable to distinguish second occurance of same quality candidate to return
    while(!feof(dictionaryFile)) //Until dictionary file is all readed
    {
        fscanf(dictionaryFile, "%s", curWord); //Read one word
        curWordLen = word_len(curWord);
        if(curWordLen == wordLen) //Get difference between read word and given word, sequence is important
        {
            curDifference = compare_words(word, curWord, &curConsonantToVowel);
            if(!curDifference) //If they are the same
            {
                copy_word(leastDifferentWord, curWord, 20);
                break;
            }
        }
        /*If read word is 1 character different in length than given word, compare using compare_words1, longer word 
		comes first.*/ 
        else if(curWordLen == wordLen - 1) curDifference = compare_words1(word, curWord, &curConsonantToVowel);
        else if(curWordLen == wordLen + 1) curDifference = compare_words1(curWord, word, &curConsonantToVowel);
        else continue; //If lengths don't match up get to next word in dictionary
        if(curDifference < minDifference) //If new read word is better, update the candidate
        {
            copy_word(leastDifferentWord, curWord, 20);
            minDifference = curDifference;
            maxConsonantToVowel = curConsonantToVowel;
            occurance = 0;
        }
        else if(curDifference == minDifference)
        {
            if(curConsonantToVowel > maxConsonantToVowel) //Higher consonant to vowel transform count is preferred
            { 
                copy_word(leastDifferentWord, curWord, 20);
                maxConsonantToVowel = curConsonantToVowel;
                occurance = 0;
            }
            else if(curConsonantToVowel == maxConsonantToVowel && !occurance) //If all the properties are the same, check if this is second time
            {
                copy_word(leastDifferentWord, curWord, 20); //If so update the candidate
                ++occurance;
            }
        }
    }
    fclose(dictionaryFile);
    copy_word(foundWord, leastDifferentWord, 20); //Write found equivalent
}

/*Finds best candidate for misspelled word with max of 2 number of character difference in length, writes in given 
char array foundWord.*/
void find_best_correction2(char *dictionaryFileName, char *word, char *foundWord)
{
    FILE *dictionaryFile = fopen(dictionaryFileName, "r"); //Dictionary that will be used
    //Result word and read word
    char leastDifferentWord[20], curWord[20];
    int minDifference = 20, maxConsonantToVowel = 0; //Result word's properties
    int curDifference, curConsonantToVowel; //Read word's properties
    int wordLen = word_len(word);
    int curWordLen;
    int occurance = 0; //Variable to distinguish second occurance of same quality candidate to return
    //Until dictionary file is all readed
    while(!feof(dictionaryFile))
    {
        fscanf(dictionaryFile, "%s", curWord); //Read one word
        curWordLen = word_len(curWord);
        //Get difference between read word and given word, sequence is important
        if(curWordLen == wordLen)
        {
            curDifference = compare_words(word, curWord, &curConsonantToVowel);
            //If they are the same
            if(!curDifference)
            {
                copy_word(leastDifferentWord, curWord, 20);
                break;
            }
        }
        /*If read word is 1 character different in length than given word, compare using compare_words1, longer word 
		comes first.*/ 
        else if(curWordLen == wordLen - 1) curDifference = compare_words1(word, curWord, &curConsonantToVowel);
        else if(curWordLen == wordLen + 1) curDifference = compare_words1(curWord, word, &curConsonantToVowel);
        /*If read word is 2 characters different in length than given word, compare using compare_words2, longer word
		comes first.*/ 
        else if(curWordLen == wordLen - 2) curDifference = compare_words2(word, curWord, &curConsonantToVowel);
        else if(curWordLen == wordLen + 2) curDifference = compare_words2(curWord, word, &curConsonantToVowel);
        else continue;  //If lengths don't match up get to next word in dictionary
        if(curDifference < minDifference) //If new read word is better, update the candidate
        {
            copy_word(leastDifferentWord, curWord, 20);
            minDifference = curDifference;
            maxConsonantToVowel = curConsonantToVowel;
            occurance = 0;
        }
        else if(curDifference == minDifference)
        {
            if(curConsonantToVowel > maxConsonantToVowel) //Higher consonant to vowel transform count is preferred
            {  
                copy_word(leastDifferentWord, curWord, 20);
                maxConsonantToVowel = curConsonantToVowel;
                occurance = 0;
            }
            else if(curConsonantToVowel == maxConsonantToVowel && !occurance) //If all the properties are the same, check if this is second time
            { 
                copy_word(leastDifferentWord, curWord, 20); //If so update the candidate
                ++occurance;
            }
        }
    }
    fclose(dictionaryFile);
    copy_word(foundWord, leastDifferentWord, 20); //Write found equivalent
}

//Reads given text file and corrects spellings in it by a given dictionary text file 
void fix_spelling_error1(char *textFileName, char *dictionaryFileName)
{
    FILE *textFile = fopen(textFileName, "r"); //File of text to be corrected
    char fixed[10000]; //Fixed form of text goes here
    char readWord[20]; //Read word from incorrect text
    char foundWord[20]; //Correct word from dictionary
    while(!feof(textFile))
    {
        fscanf(textFile, "%19[a-zA-Z&-]", readWord); //Read only the words consisting of letters and '&' and '-'
        if(readWord[1] == 0) //If read word is one letter, let it pass
            copy_word(foundWord, readWord, 20);
        else //If not, correct it
            find_best_correction1(dictionaryFileName, readWord, foundWord);
        append_words(fixed, foundWord); //Write correct form of read word
        fscanf(textFile, "%19[^a-zA-Z]", readWord); //Read everyting (that is lesser than 19 in length) that is not a letter until one is encountered
        append_words(fixed, foundWord); //Print that directly, we want punctuations as they are
    }
    fclose(textFile);
    //Writing to given text file the correct form of the text
    FILE *correctedTextFile = fopen(textFileName, "w");
    fprintf(correctedTextFile, "%s", fixed);
    fclose(correctedTextFile);
}

//Reads given text file and corrects spellings in it by a given dictionary text file
void fix_spelling_error2(char *textFileName, char *dictionaryFileName)
{
    FILE *textFile = fopen(textFileName, "r"); //File of text to be corrected
    char fixed[10000]; //Fixed form of text goes here
    char readWord[20]; //Read word from incorrect text
    char foundWord[20]; //Correct word from dictionary
    while(!feof(textFile))
    {
        fscanf(textFile, "%19[a-zA-Z&-]", readWord); //Read only the words consisting of letters and '&' and '-'
        if(readWord[1] == 0) //If read word is one letter, let it pass
            copy_word(foundWord, readWord, 20);
        else //If not, correct it
            find_best_correction2(dictionaryFileName, readWord, foundWord);
        
        append_words(fixed, foundWord); //Write correct form of read word
        fscanf(textFile,"%19[^a-zA-Z]", readWord); //Read everyting (that is lesser than 19 in length) that is not a letter until one is encountered
        append_words(fixed, readWord); //Print that directly, we want punctuations as they are
    }
    fclose(textFile);
    //Writing to given text file the correct form of the text
    FILE *correctedTextFile = fopen(textFileName, "w"); 
    fprintf(correctedTextFile, "%s", fixed);
    fclose(correctedTextFile);
}

int main()
{
    fix_spelling_error2("text.txt", "dictionary.txt"); //Correct text.txt
    return 0;
}
