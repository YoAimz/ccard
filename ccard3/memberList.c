#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "safeinput.h"




typedef struct{
    char cardId[5];
    time_t timestamp;
}CARD;



typedef struct
{
    CARD *Lista;
    int count;
}CARDLIST;

//funktion to generate random number
void randomGeneratedCardId(char*cardId){
    sprintf(cardId, "%04d", rand() % 10000);
}


//funktion to check if card is already in list when creating a new id
int noCopyOfSameIdCheck(const CARDLIST* cardList, const char* cardId){
    for (int i = 0; i < cardList->count; ++i) {
        if (strcmp(cardList->Lista[i].cardId, cardId) == 0) {
            return 1; // ID already exists
        }
    }
    return 0; // ID does not exist
}



void members(){
    CARD cardsArray[10];

    CARDLIST cardList;
    cardList.Lista = cardsArray;
    cardList.count = 0;
    
    
    int addingCardId;
    while(1){
            GetInputInt("do you want to add user: ", &addingCardId);
    }
    
    
}