#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <time.h>

// constants
// max number of arguments user can provide
const int maxArgumentsLength = 6;
// letters constants
const int defaultLettersLength = 7;
const int defaultMinLettersLength = 3;
const int lowestMinLettersLength = 3;
const int highestMinLettersLength = 5;
const int maxLettersLength = 13;
// exit status
const int usageErrorStatus = 7;
const int invalidLengthStatus = 1;
const int invalidLetterSetStatus = 19;
const int excessLettersLengthStatus = 13;
const int shortLettersLengthStatus = 11;
const int invalidDictStatus = 6;
const int exitGameStatus = 0;
const int exitGameNoGuessStatus = 18;
// used in declaring variables
// max length for an argument the user provides
const int maxArgValLength = 100;
// initial size for the dictionary path
const int initialDictSize = 100;
// initial size for the words extracted from the dictionary file
const int initialWordsSize = 100;
// initial size for a single word amongst words
const int initialOneWordSize = 3;
// extra score added when user's guess uses all the letters provided
const int bonusScore = 10;

/* Given the pair of arguments and corresponding value, assign the value to
 * the corresponding variables provided as arguments.
 */
int assign_values(char* firstEle, char* secondEle, int* minLength,
        char* letters, char* dict)
{
    if (strcmp(firstEle, "--min-length") == 0) {
        // check if the length of the value for min length is 1 and isn't
        // a letter
        if (!(strlen(secondEle) == 1 && !isalpha(secondEle[0]))) {
            return 1;
        }
        *minLength = atoi(secondEle);
    } else if (strcmp(firstEle, "--letters") == 0) {
        strcpy(letters, secondEle);
    } else if (strcmp(firstEle, "--dict") == 0) {
        // check if size of new dict path is longer than the initial size
        if ((int)strlen(secondEle) > initialDictSize) {
            dict = realloc(dict, 2 * initialDictSize * sizeof(char));
        }

        strcpy(dict, secondEle);
    }
    return 0;
}

/* Check if the array provided already contains the argument name provided
 */
int check_duplicate_arguments(
        char array[][maxArgValLength], char* argName, int arraySize)
{
    for (int i = 0; i < arraySize; i++) {
        if (strcmp(array[i], argName) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Check if the arguments are valid, with correct argument name provided,
 * no duplicate argument names and assign the default values. */
int check_arguments(
        int argc, char** argv, int* minLength, char* letters, char* dict)
{
    // default value for the arugments
    *minLength = defaultMinLettersLength;
    strcpy(dict, "words.txt");

    // initialised array and index to keep track of arguments already assigned
    char array[maxArgumentsLength][maxArgValLength];
    int index = 0;

    // if no arguments present, return immediately
    if (argc == 0) {
        return 0;
    }

    // loop through the arguments in pairs (argument name and value)
    // and assinging them
    for (int i = 1; i < argc; i += 2) {
        char* firstEle = argv[i];
        char* secondEle = argv[i + 1];

        // check the first element of each pair of arguments are valid
        if (!(strcmp(firstEle, "--letters") == 0
                    || strcmp(firstEle, "--dict") == 0
                    || strcmp(firstEle, "--min-length") == 0)) {
            return 1;
        }

        // check for duplicated arguments
        if ((check_duplicate_arguments(array, firstEle, index)) == 1) {
            return 1;
        }

        // keep track of values already provided by the user to avoid duplicates
        strcpy(array[index++], firstEle);

        // store the value of the second element in corresponding variables
        // check length provided is a single digit number
        if (assign_values(firstEle, secondEle, minLength, letters, dict) == 1) {
            return 1;
        }
    }

    return 0;
}

/* Prints out the relevant error message when there's a usage error and
 * return the corresponding status
 */
int print_usage_err()
{
    fprintf(stderr,
            "Usage: unscramble [--min-length numchars] [--dict file] "
            "[--letters chars]\n");
    return usageErrorStatus;
}

/* Checks if the provided string contain only letters from (a-z and A-Z). */
int is_string_alpha(const char* letters)
{
    for (int i = 0; letters[i] != '\0'; i++) {
        if (!isalpha(letters[i])) {
            return 1;
        }
    }
    return 0;
}

/* Given letters, verify they are all letters, and between min length provided
 * and the max length 13
 */
int validate_letters(char* letters, const int* minLength)
{
    // check the string contains only letters
    if (is_string_alpha(letters)) {
        fprintf(stderr, "unscramble: letter set is invalid\n");
        return invalidLetterSetStatus;
    }
    // check it has max 13 letters
    // type casting maxLettersLength and minLength from int to size_t,
    // the same type as the return value of strlen
    if (strlen(letters) > (size_t)maxLettersLength) {
        fprintf(stderr,
                "unscramble: number of letters "
                "should be no more than 13\n");
        return excessLettersLengthStatus;
    }
    // check it has more letters than the minLength provided
    if (strlen(letters) < (size_t)*minLength) {
        fprintf(stderr,
                "unscramble: too few letters "
                "for the given minimum length (%d)\n",
                *minLength);
        return shortLettersLengthStatus;
    }
    return 0;
}

/* Randomly generate string of random letters with length determined 
by the argument provided*/
char* get_random_letters(int length) {
    // allocate memory for the string
    char* result = (char*)malloc((length + 1) * sizeof(char)); 

    if (result == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < length; i++) {
        // generate a random uppercase letter (ASCII values 65 to 90)
        result[i] = 'A' + (rand() % 26);
    }

    result[length] = '\0';

    return result;
}

/* Initialise the provided string with random letters
 */
void initialise_letters(char* letters)
{
    strcpy(letters, get_random_letters(defaultLettersLength));
}

/* Check if the value for letters has been initialised, and
 * initialise with get_random_letters if not.
 * If initialised, check for its validity.
 */
int check_letters(char* letters, int* minLength)
{
    // check if the letters is empty
    if (strcmp(letters, " ") == 0) {
        initialise_letters(letters);
    } else {
        // check for errors in letters
        int lettersStatus = validate_letters(letters, minLength);
        if (lettersStatus != 0) {
            return lettersStatus;
        }
    }
    return 0;
}

/* Given string, return the uppercase version. */
char* convert_to_upper(char* word)
{
    // extra 1 for the null terminator at the end of the string
    char* newWord = malloc(((int)strlen(word) + 1) * sizeof(char));
    if (newWord == NULL) {
        return NULL;
    }

    int i;
    for (i = 0; word[i] != '\0'; i++) {
        newWord[i] = toupper(word[i]);
    }
    newWord[i] = '\0';
    return newWord;
}

// Used to keep track of the letter and its count.
typedef struct CharCount {
    char letter;
    int count;
} CharCount;

// Keep track of the array of CharCount and its size
typedef struct CharCountArr {
    CharCount* letters;
    int size;
} CharCountArr;

/* Given letter check if it is inside the array of existing letters count,
 * if yes, add 1 to the count
 * else, add the new letter count to the array.
 * size provided externally to keep track of how big the array is.
 * numReads keeps track of how many unique letters have been added to
 * lettersCount,
 * bufferSize keeps track of the amount of memory assigned to lettersCount.
 */
void check_in_letters_count(
        char letter, CharCount** lettersCount, int* numReads, int* bufferSize)
{
    // allocate more space if necessary
    if (*numReads >= *(bufferSize)-1) {
        *bufferSize *= 2;
        *lettersCount = realloc(*lettersCount, *bufferSize * sizeof(CharCount));
    }

    // check if letter already present in the array
    // if yes, add 1 to the count
    // else add the letter to the array
    for (int i = 0; i < *numReads; i++) {
        if ((*lettersCount)[i].letter == letter) {
            (*lettersCount)[i].count++;
            return;
        }
    }

    // reach here if letter not found, then add the letter to the array
    (*lettersCount)[*numReads].letter = letter;
    (*lettersCount)[*numReads].count = 1;
    *numReads += 1;
}

/* Given a string, return list containing each character and their count. */
CharCountArr get_letter_count(char* word)
{
    // keeps track of how many unique letters have been counted
    int numLetterRead = 0;
    // keep track of the amount of memory assigned to the counts
    int bufferSize = 1;

    // dynamically allocate the size of lettersCount
    CharCount* lettersCount = malloc(bufferSize * sizeof(CharCount));

    // loop over the word
    for (int i = 0; word[i] != '\0'; i++) {
        check_in_letters_count(
                word[i], &lettersCount, &numLetterRead, &bufferSize);
    }

    // return new struct that also keep track of the size of the array
    CharCountArr newLetterCount = {lettersCount, numLetterRead};

    return newLetterCount;
}

/* Compare two arrays of CharCount to see if the the letter array has all
 * the characters of the input and >= counts of all the characters as well.
 */
int compare_letters_counts(CharCountArr inputCounts, CharCountArr lettersCounts)
{
    // loop through all the CharCount pairs of inputCounts
    // for each pair, loop through all the CharCount pairs of lettersCounts
    // to see if there is a match and if so, does the count match
    for (int i = 0; i < inputCounts.size; i++) {
        // 1 for current input letter not found
        // 0 for found
        int letterFound = 1;

        // check if the current input letter is inside the letters
        // if letter present but count doesn't match immediately return 1
        // if letter present and count matches set letterFound to 0 and
        // proceed to the next inputer letter.
        // If input letter not found, letterFound flag will trigger and return
        // 1 as well.
        for (int j = 0; j < lettersCounts.size; j++) {
            CharCount input = inputCounts.letters[i];
            CharCount letters = lettersCounts.letters[j];

            if (input.letter == letters.letter) {
                if (input.count > letters.count) {
                    return 1;
                }
                letterFound = 0;
                break;
            }
        }

        if (letterFound == 1) {
            return 1;
        }
    }
    return 0;
}

/* Check if the word provided can be formed using the provided letters set.
 * Done by counting the number of letters.
 */
int letter_can_form(char* input, char* letters)
{
    // convert input and letters both to uppercase for easy comparison
    char* newInput = convert_to_upper(input);
    char* newLetters = convert_to_upper(letters);

    // if initialisation failed, immediately free the memories
    if (newInput == NULL) {
        free(newInput);
    }
    if (newLetters == NULL) {
        free(newLetters);
    }

    // get counts for inputs and letters
    CharCountArr inputsCount = get_letter_count(newInput);
    CharCountArr lettersCount = get_letter_count(newLetters);

    // compare the counts
    if (compare_letters_counts(inputsCount, lettersCount)) {
        // free the memories before returning error
        free(newInput);
        free(newLetters);
        free(inputsCount.letters);
        free(lettersCount.letters);
        return 1;
    }
    // free used memories before returning success
    free(newInput);
    free(newLetters);
    free(inputsCount.letters);
    free(lettersCount.letters);
    return 0;
}

/* Given an array of strings, print out every single one in separate lines. */
void print_string_array(char** stringArr)
{
    for (int i = 0; stringArr[i] != NULL; i++) {
        printf("%s\n", stringArr[i]);
    }
}

/* Read a single line from the file.
 * Convert every single character to uppercase as well.
 * REF: Ed Lessons Week 3.2 file handling
 * */
char* read_line(FILE* file)
{
    int lineSize = initialOneWordSize;
    char* line = malloc(sizeof(char) * lineSize);
    int numReads = 0;
    int next;

    if (feof(file)) {
        free(line);
        return NULL;
    }

    while (1) {
        next = fgetc(file);
        if (next == EOF && numReads == 0) {
            free(line);
            return NULL;
        }
        if (numReads == lineSize - 1) {
            lineSize *= 2;
            line = realloc(line, lineSize * sizeof(char));
        }
        if (next == '\n' || next == EOF) {
            line[numReads] = '\0';
            break;
        }
        line[numReads++] = toupper(next);
    }
    return line;
}

/* Read every single line from the file and save it to the list of words.
 * REF: Ed Lesson week 3.2
 */
void read_file(FILE* file, char*** words, int* numLines, int* size)
{
    if (*words == NULL) {
        free(*words);
        return;
    }
    // for storing each line read from the file
    char* line;
    // keep track of how many lines read
    *numLines = 0;

    // read words from the file
    while ((line = read_line(file))) {
        // If the number of words exceeds the current size, reallocate the array
        if (*numLines >= ((*size) - 1)) {
            *size *= 2;
            *words = realloc(*words, (*size) * sizeof(char*));
            // if realloc failed, free all memories
            if (*words == NULL) {
                for (int i = 0; i < *numLines; i++) {
                    free((*words)[i]);
                }
                free(*words);
                return;
            }
        }

        // allocate memory for the current line
        (*words)[*numLines] = malloc(((int)strlen(line) + 1) * sizeof(char));

        // copy the line into the array
        strcpy((*words)[*numLines], line);
        free(line);
        *numLines += 1;
    }
}

/* Check if the provided filename can be opened then read all
 * words from it and save it to an array of strings.
 */
int check_file(const char* filename, char*** words, int* numLines, int* size)
{
    FILE* file = fopen(filename, "r");
    // output error message if file can't be opened.
    if (file == NULL) {
        fprintf(stderr,
                "unscramble: dictionary named \"%s\" cannot "
                "be opened\n",
                filename);
        return 1;
    }

    read_file(file, words, numLines, size);
    fclose(file);
    return 0;
}

/* Add the length of the input to the score.
 * If it is the same as the max length, add extra 10 to the score.
 */
void add_score(int* score, int inputLen, int maxLen)
{
    *score += inputLen;

    if (inputLen == maxLen) {
        *score += bonusScore;
    }

    printf("OK! Score so far is %d\n", *score);
}

/* Determines what to print based on the final score */
int exit_game(const int* score)
{
    if (*score == 0) {
        printf("No words guessed!\n");
        return exitGameNoGuessStatus;
    }

    if (*score > 0) {
        printf("Your final score is %d\n", *score);
        return exitGameStatus;
    }
    return 1;
}

/* Print the welcome message */
void print_welcome(const int* minLength, int lettersLength, char* letters)
{
    printf("Welcome to unscramble!\n");
    printf("Enter words of length %d to %d made from the letters "
           "\"%s\"\n",
            *minLength, lettersLength, letters);
}

/* Checks if the input has already been guessed */
int input_already_guessed(char* input, char** guesses)
{
    // skip if no guesses have been made yet
    if (guesses[0] == NULL) {
        return 0;
    }

    // go through all the guesses and see if the input has already been
    // guessed
    for (int i = 0; guesses[i] != NULL; i++) {
        if (strcmp(input, guesses[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Checks if the provided input is part of the array of valid words.
 * If yes, add it to the array of guesses.
 */
int input_in_dictionary(char* input, char** words, char** guesses,
        int* numValidGuess, int* guessesSize)
{
    // go over the list of valid and check if input is one of them
    for (int i = 0; words[i] != NULL; i++) {
        // if guess is valid, add the input to the list of guesses
        if (strcmp(input, words[i]) == 0) {
            // assign extra memory if needed
            if (*numValidGuess == *guessesSize - 1) {
                *guessesSize *= 2;
                guesses = realloc(guesses, sizeof(char*) * *guessesSize);
            }

            guesses[*numValidGuess]
                    = malloc(((int)strlen(input) + 1) * sizeof(char));
            strcpy(guesses[*numValidGuess], input);
            *numValidGuess += 1;
            return 0;
        }
    }

    return 1;
}

/* Check if the user input is valid.
 * Perform checks on only letters in the input, length of input, can be formed
 * with available letters, guessed before, is a valid word.
 */
int check_input(char* input, const int* minLength, char* letters, char** words,
        char** guesses, int* numValidGuess, int* guessesSize)
{
    if (is_string_alpha(input)) {
        printf("Word must contain only letters\n");
        free(input);
        return 1;
    }

    if ((int)strlen(input) < *minLength) {
        printf("Word too short - it must be at least %d characters long\n",
                *minLength);
        free(input);
        return 1;
    }

    if ((int)strlen(input) > (int)strlen(letters)) {
        printf("Word must be no more than %d characters long\n",
                (int)strlen(letters));
        free(input);
        return 1;
    }

    if (letter_can_form(input, letters)) {
        printf("Word can't be formed with available letters\n");
        free(input);
        return 1;
    }

    if (input_already_guessed(input, guesses)) {
        printf("You've guessed that word before\n");
        free(input);
        return 1;
    }

    if (input_in_dictionary(
                input, words, guesses, numValidGuess, guessesSize)) {
        printf("Word can't be found in dictionary\n");
        free(input);
        return 1;
    }
    return 0;
}

/* Start the game with the welcome message and start asking for user input.
 * REF: Ed lesson Week 3.2 file handling.
 */
int start_game(int* minLength, char* letters, char** words)
{
    int score = 0;
    int numValidGuess = 0;
    int guessesSize = initialWordsSize;
    char** guesses = calloc(sizeof(char*), guessesSize);
    char* input;

    // Print welcome message
    print_welcome(minLength, (int)strlen(letters), letters);

    // Start the game
    while ((input = read_line(stdin))) {
        // implement the checks on user input
        // if any error occur, immediately ask for the next input
        if (check_input(input, minLength, letters, words, guesses,
                    &numValidGuess, &guessesSize)) {
            continue;
        }

        // add score to the user
        add_score(&score, (int)strlen(input), (int)strlen(letters));
        free(input);
    }
    // free the guesses and words
    for (int i = 0; i < numValidGuess; i++) {
        free(guesses[i]);
    }
    free(guesses);
    for (int i = 0; words[i] != NULL; i++) {
        free(words[i]);
    }
    free(words);
    return exit_game(&score);
}

/* Starts the entire program. Validates and saves arguments provided.
 * Use arguments to start the game.
 */
int main(int argc, char** argv)
{
    // Check even number of arguments and total is less than or equal to 6
    // minus 1 to account for the program name included in argc
    int realArgc = argc - 1;
    if (!(realArgc % 2 == 0 && realArgc <= maxArgumentsLength)) {
        return print_usage_err();
    }

    // verify argument names are valid and assigning the values
    int minLength = 0;
    char letters[maxLettersLength];
    strcpy(letters, " ");
    // dynamically allocate memory for dict
    char* dict = malloc(initialDictSize * sizeof(char));

    // storing arugments provided in approproiate variables
    if (check_arguments(realArgc, argv, &minLength, letters, dict) == 1) {
        free(dict);
        return print_usage_err();
    }

    // check the min length is between 3 and 5
    if (!(minLength >= lowestMinLettersLength
                && minLength <= highestMinLettersLength)) {
        fprintf(stderr,
                "unscramble: minimum length must be between 3 and 5\n");
        free(dict);
        return invalidLengthStatus;
    }

    // check letters are valid
    int letterStatus = check_letters(letters, &minLength);
    if (letterStatus != 0) {
        free(dict);
        return letterStatus;
    }

    // check directory provided works and saves all the content of the file
    int size = initialWordsSize;
    char** words = calloc(sizeof(char*), size);
    int numLines;
    if (check_file(dict, &words, &numLines, &size) == 1) {
        free(dict);
        return invalidDictStatus;
    }
    free(dict);
    return start_game(&minLength, letters, words);
}
