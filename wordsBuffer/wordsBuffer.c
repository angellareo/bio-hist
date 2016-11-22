////////////////////////////////////////////////////////////////////////////////
/*!
 * @file wordsBuffer.c
 * @date 1-2013
 * @author Angel Lareo
 * 
 * @brief Implementation of WordsBuffer structure functions
*/
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "wordsBuffer.h"

/**
 * WordsBuffer constructor
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb       pointer to WordsBuffer
 * @param[in]       length   word length in bits    
 * @param[in]       maxWords max of word to store
*/
int wbInit(WordsBuffer* wb, int length, int maxWords){
    wb->insert=wb->words;
    wb->init=wb->words;
    wb->check = wb->words;
    wb->bb.wordLength=length;
    wb->numWords=0;
    wb->maxWords=maxWords; //Calculated from the GUI
    wb->bb.init = wb->bb.bits;
    wb->bb.insert = wb->bb.bits;
	wb->bb.numBits = 0;

    return OK;
}

void wbCreateHistogram (WordsBuffer* wb, int* results){
	int i;

	for (i=0; i<wb->maxWords; i++){
		results[i]=0;
	}
	
	for (i=0; i<wb->numWords; i++){
		results[wb->words[i]]++;
	}
}

/**
 * Insert new bit in WordsBuffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * @param[in]       bit     bit a introducir
*/
int wbBitInsert(WordsBuffer* wb, char bit){  //Inserts bit on BitBuffer
    *(wb->bb.insert)=bit;
    bbAdvancePtr(&(wb->bb), &(wb->bb.insert));
    wb->bb.numBits++;
    return OK;
}

/**
 * Forward insert pointer, circular buffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      pointer to WordsBuffer
 * @param[in,out]       ptr     insert pointer to advance
*/
void bbAdvancePtr (BitsBuffer* bb, char** ptr){
    if (*ptr==&(bb->bits[bb->wordLength-1])) *ptr = bb->bits;
    else (*ptr)++;
}

/**
 * Inserts word in WordsBuffer structure
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * @param[in]       word    word value
*/
int wbWordInsert (WordsBuffer* wb, int word){ //Inserts word on WordsBuffer
                                                 //Called by StoreWord

    *wb->insert = word; //copy word value
	wb->check=wb->insert;
    wb->numWords++; 

    //if (wb->insert == wb->words+wb->maxWords){
    //    wb->insert = wb->words;
    //    if (wb->init == wb->words+wb->maxWords) wb->init = wb->words;
    //    else wb->init++;
    //} else wb->insert++;
    wb->insert++;

    return OK;
}

/**
 * Store the word contained in the bits buffer in the words buffer as an integer
 * Performs bits to integer transformation
 * ERR if there is no word in the bits buffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @see wbWordInsert
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * 
 * @return word value or ERR
*/
int wbStoreWord (WordsBuffer *wb){ 
    int wordResult;

    if (wb->bb.numBits<wb->bb.wordLength) return ERR;
    
    wordResult = wbBits2Int(wb);

    //Advance bit init
    bbAdvancePtr(&(wb->bb),&(wb->bb.init));

    //Insert word
    wbWordInsert(wb, wordResult);

    return wordResult;
}

int wbBits2Int(WordsBuffer *wb){ 
	char *auxPtr;
	int i, exp;
	int wordResult = 0;
	
	exp=wb->bb.wordLength - 1;
	
	auxPtr=wb->bb.init;
    for (i=0; i<wb->bb.wordLength; ++i){
        if ((int)*auxPtr){
            switch (exp){
                case 0: wordResult += 1; break;
                case 1: wordResult += 2; break;
                default: wordResult += pow(2,exp); break;
            }
        }
        exp--;
        bbAdvancePtr(&(wb->bb), &auxPtr);
    }
    
    return wordResult;
}

int Bits2Int(char* bb, int length){ 
	char *auxPtr;
	int i, exp;
	int wordResult = 0;
	
	exp=length - 1;
	
	auxPtr=bb;
    for (i=0; i<length; ++i){
        if ((int)*auxPtr){
            switch (exp){
                case 0: wordResult += 1; break;
                case 1: wordResult += 2; break;
                default: wordResult += pow(2,exp); break;
            }
        }
        exp--;
        auxPtr++;
    }
    
    return wordResult;
}

/**
 * Check if the bits contained in the bits buffer matched with the one passed as a parameter
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      pointer to WordsBuffer
 * @param[in]   word    binary word to check if matches
*/
int wbCheckWordMatch(WordsBuffer *wb, char *word){
	return (*(wb->check)==Bits2Int(word, wb->bb.wordLength));
}
