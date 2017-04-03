////////////////////////////////////////////////////////////////////////////////
/*!
 * @file wordsBuffer.c
 * @date 1-2014
 * @author Angel Lareo
 * 
 * @brief Archivo con las funciones de operación de la estructura WordsBuffer
*/
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "wordsBuffer.h"

/**
 * Inicializa los valores y punteros de la estructura WordsBuffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb       puntero a la estructura a rellenar
 * @param[in]       length   longitud de palabras en estructura expresada en bits    
 * @param[in]       maxWords máximo de palabras a almacenar en la estructura  
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
	wb->overlap = -1;
    return OK;
}

int wbInit_overlap(WordsBuffer* wb, int length, int maxWords, int overlap){
    if (overlap>=length)
        return ERR;

    wb->insert=wb->words;
    wb->init=wb->words;
    wb->check = wb->words;
    wb->bb.wordLength=length;
    wb->numWords=0;
    wb->maxWords=maxWords; //Calculated from the GUI
    wb->bb.init = wb->bb.bits;
    wb->bb.insert = wb->bb.bits;
	wb->overlap = overlap;
    return OK;
}

void wbCreateHistogram (WordsBuffer* wb, int* results){
	int i;
	
	for (i=0; i<pow(2,wb->bb.wordLength); i++){
		results[i]=0;
	}
	
	for (i=0; i<wb->numWords; i++){
		results[wb->words[i]]++;
	}
}

/**
 * Inicializa los valores y punteros de la estructura WordsBuffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      puntero a la estructura a rellenar
 * @param[in]       bit     bit a introducir
*/
int wbBitInsert(WordsBuffer* wb, char bit){  //Inserts bit on BitBuffer
    int i;
    
    //printf("%d\n", bit);    

    *(wb->bb.insert)=bit;
    bbAdvancePtr(&(wb->bb), &(wb->bb.insert));

    //Advance bit init
    if (wb->overlap==-1)
        bbAdvancePtr(&(wb->bb),&(wb->bb.init));
    else{
        for (i=0; i<=(wb->bb.wordLength - wb->overlap); ++i)
            bbAdvancePtr(&(wb->bb),&(wb->bb.init));
    }

    wb->bb.numBits++;
    return OK;
}

/**
 * Avanza el puntero en la estructura de bits, convirtiéndola en un buffer circular
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   bb      puntero a la estructura a rellenar
 * @param[in,out]       ptr     puntero a avanzar
*/
void bbAdvancePtr (BitsBuffer* bb, char** ptr){
    if (*ptr==&(bb->bits[bb->wordLength-1])) *ptr = bb->bits;
    else (*ptr)++;
}

/**
 * Inserta una palabra con el valor de word en la estructura WordsBuffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      puntero a la estructura a rellenar
 * @param[in]       word    valor de la palabra
*/
int wbWordInsert (WordsBuffer* wb, int word){ //Inserts word on WordsBuffer
                                                 //Called by StoreWord
    *wb->insert = word; //copy word value
	wb->check=wb->insert;
    wb->numWords++; 

    if (wb->insert == wb->words+wb->maxWords-1){
        wb->insert = wb->words;
        if (wb->init == wb->words+wb->maxWords-1) wb->init = wb->words;
        else wb->init++;
    } else wb->insert++;

    return OK;
}

/**
 * Inserta la palabra almacenada en el buffer de bits de la estructura en el buffer de palabras.
 * Para ello primero realiza una transformacion de bits a entero.
 * Si no se han introducido suficientes bits para establecer una palabra, devuelve ERR.
 * 
 * @author Ángel Lareo
 * @date 1/2014
 * 
 * @see wbWordInsert
 * 
 * @param[in,out]   wb      puntero a la estructura donde se insertará la palabra
 * 
 * @return retorna el valor entero de la palabra o ERR
*/
int wbStoreWord (WordsBuffer *wb){ 
	int wordResult;
    int i;

    if (wb->bb.numBits<wb->bb.wordLength){
		return ERR;
    }
    
    wordResult = wbBits2Int(wb);
	//printf("%d\n", wordResult);

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
        //printf("%d",*auxPtr);
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
    //printf(" IniPos: %d  LastInPos: %d\n", (wb->bb.init)-(wb->bb.bits));
    
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
 * Comprueba si la palabra binaria word coincide con la que se encuentra en la estructura de bits
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      puntero a la estructura donde se encuentran los bits de una palabra a comparar
 * @param[in]   word    array de bits con la otra palabra a comparar
*/
int wbCheckWordMatch(WordsBuffer *wb, char *word){
	return (*(wb->check)==Bits2Int(word, wb->bb.wordLength));
}


int wbRestartBitBuff(WordsBuffer *wb){
    wb->bb.numBits = 0;
}

int wbGetWordInt(WordsBuffer *wb){
    return wbBits2Int(wb);
}

void wbGetWordChar(char* bbOut, WordsBuffer *wb){
    char *auxPtr;
	int i, exp;
	
	auxPtr=wb->bb.init;
    for (i=0; i<wb->bb.wordLength; ++i){
        bbOut[i] = *auxPtr;
        bbAdvancePtr(&(wb->bb), &auxPtr);
    }
}
