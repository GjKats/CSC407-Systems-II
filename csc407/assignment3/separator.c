#include        <stdlib.h>
#include        <stdio.h>
#include        <unistd.h>
#include        <pthread.h>
#include 	<string.h>

const  int MAX_LINE = 256;

struct Word
{
    char* 		textPtr_;
    struct  Word* 	nextPtr_;
};

struct  Word*  obtainCommaSeparatedList( const char* string) {
    char*  		charRun  	= strchr(string, ',');
    size_t 		wordLen;
    size_t              length          = strlen(string);
    struct  Word*       toReturn        = (struct Word*)malloc(sizeof(struct Word));
    char* 		stringRemaining;

    if (charRun != NULL) {
        charRun  	+= 1;
        wordLen 	= (charRun - string - 1);
        stringRemaining   = (char*)malloc(length - wordLen);
        strncpy(stringRemaining, charRun, (length - wordLen + 1));
    } else {
        wordLen  	= length;
        stringRemaining  	= string;
    }

    char* 		wordText 	= (char*)malloc(wordLen+1);       
    strncpy(wordText, string, wordLen);
    toReturn->textPtr_ = wordText;

    if (charRun == NULL) {
        toReturn->nextPtr_ = NULL ;
        free(wordText);
        free(stringRemaining);
        return(toReturn);
    } else {
        toReturn->nextPtr_ = obtainCommaSeparatedList(stringRemaining);       
    }
    return(toReturn);           
}

void 		printCommaSeparatedList(const struct Word* list) {
    printf("Word text = \"%s\" \n", list->textPtr_);
    if (list->nextPtr_ == NULL)
        return;
    printCommaSeparatedList(list->nextPtr_);
}

void 		freeCommaSeparatedList(struct Word* list) {
    if (list->nextPtr_ == NULL) {
        free(list->textPtr_);
        return;
    }
    free(list->textPtr_);
    freeCommaSeparatedList(list->nextPtr_);
    free(list->nextPtr_);    
}

int main () {
    char 	  	line[MAX_LINE];
    char*  	  	cPtr;
    struct  Word*  	separatedList;

    printf("Enter a line of text with commas: ");
    fgets(line, MAX_LINE, stdin);

    cPtr  	= strchr(line,'\n');

    if (cPtr != NULL) {
        *cPtr = '\0';
        separatedList 	= obtainCommaSeparatedList(line);
        printCommaSeparatedList(separatedList);
        freeCommaSeparatedList(separatedList);
    } else {
        printf("Invalid string. \n");
        printf("Too short \n");
    }

    return(EXIT_SUCCESS);
}
