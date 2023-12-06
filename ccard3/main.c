//passing system from main source(admin).
//admin menu too remote open door, add new id or remove id, and check if id is in the system.
//csv file to save all id cards with the date and time.
//unfinished code//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "safeinput.h"

//struct do manage id card
typedef struct{
    char cardId[20];
    time_t timestamp;
}CARD;


//struct to manage list of id cards
typedef struct
{
    CARD *Lista;
    int count;
}CARDLIST;






//function to format time for writing to CSV file
void formatTimeForCSV(const time_t timestamp, char formattedTime[50]) {
    strftime(formattedTime, 50, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
}




//funktion to save new card id in a csv file
void writeCardToCSV(const CARD *card) {
    FILE *file = fopen("cardMemberList.csv", "a");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // date time (format timestamp)
    char formattedTime[50];
    formatTimeForCSV(card->timestamp, formattedTime);

    fprintf(file, "card ID: %s, Added on: %s\n", card->cardId, formattedTime);

    if (fclose(file) != 0) {
        perror("Error closing file after writing");
    }
}


 

void takeIdCardFromFile(CARDLIST *cardList) {
    FILE *file = fopen("cardMemberList.csv", "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    cardList->count = 0;
    cardList->Lista = NULL;

    // pointer to hold reallocated memory
    CARD *tempLista;

    while (1) {
        // Reallocate memory only if there are more cards to read
        tempLista = realloc(cardList->Lista, sizeof(CARD) * (cardList->count + 1));

        if (tempLista == NULL) {
            perror("Error reallocating memory");
            // handling memory allocation failure (free existing memory and return)
            free(cardList->Lista);
            cardList->count = 0;
            fclose(file);
            return;
        }

        cardList->Lista = tempLista;

        char formattedTime[50];
        if (fscanf(file, "card ID: %19[^,], Added on: %49[^\n]", cardList->Lista[cardList->count].cardId, formattedTime) == 2) {
            struct tm tm = {0};  //Initialize with zeros
            int year, month, day, hour, minute, second;

            //Parse the components of the timestamp
            if (sscanf(formattedTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6) {
                tm.tm_year = year - 1900;
                tm.tm_mon = month - 1;
                tm.tm_mday = day;
                tm.tm_hour = hour;
                tm.tm_min = minute;
                tm.tm_sec = second;

                //Convert the timestamp to time_t
                cardList->Lista[cardList->count].timestamp = mktime(&tm);

                // Increment count
                cardList->count++;

                // Reallocate memory if needed
                cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * (cardList->count + 1));
            } else {
                break;  // Exit the loop if parsing fails
            }
        } else {
            break;  // Exit the loop if no more cards to read
        }
    }

    fclose(file);

    // Print the count of cards after reading
    printf("Read %d cards from the file.\n", cardList->count);
}





void readCardsFromCSV(CARDLIST *cardList) {
    FILE *file = fopen("cardMemberList.csv", "r");
    if (file == NULL) {
        perror("Error opening file for reading.\n");
        return;
    }

    // Initialize count to 0
    cardList->count = 0;

    // Set an initial size for Lista, you can adjust it based on your needs
    int initialSize = 10;
    cardList->Lista = malloc(initialSize * sizeof(CARD));

    // Read until the end of the file
    while (1) {
        // Reallocate memory only if there are more cards to read
        CARD *tempLista = realloc(cardList->Lista, sizeof(CARD) * (cardList->count + 1));

        if (tempLista == NULL) {
            perror("Error reallocating memory");
            // handling memory allocation failure (free existing memory and return)
            free(cardList->Lista);
            cardList->count = 0;
            fclose(file);
            return;
        }

        cardList->Lista = tempLista;

        // Use a temporary variable to store the timestamp as a string
        char formattedTime[50];
/*
        char formattedTime[50];
        if (fscanf(file, "card ID: %19[^,], Added on: %49[^\n]", cardList->Lista[cardList->count].cardId, formattedTime) == 2) {
            struct tm tm = {0};  //Initialize with zeros
            int year, month, day, hour, minute, second;
*/
        // Use fscanf to read both fields from the CSV line
        if (fscanf(file, "card ID: %19[^,], Added on: %49[^\n]", cardList->Lista[cardList->count].cardId, formattedTime) == 2) {
            struct tm tm = {0};  // Initialize with zeros
            int year, month, day, hour, minute, second;

            // Parse the components of the timestamp
            if (sscanf(formattedTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6) {
                tm.tm_year = year - 1900;
                tm.tm_mon = month - 1;
                tm.tm_mday = day;
                tm.tm_hour = hour;
                tm.tm_min = minute;
                tm.tm_sec = second;

                // Convert the timestamp to time_t
                cardList->Lista[cardList->count].timestamp = mktime(&tm);

                // Increment count
                cardList->count++;

                // Reallocate memory if needed
                if (cardList->count >= initialSize) {
                    initialSize *= 2;  // Double the size
                    CARD *temp = realloc(cardList->Lista, initialSize * sizeof(CARD));
                    if (temp == NULL) {
                        perror("Error reallocating memory");
                        free(cardList->Lista);
                        cardList->count = 0;
                        fclose(file);
                        return;
                    }
                    cardList->Lista = temp;
                }
            } else {
                break;  // Exit the loop if parsing fails
            }
        } else {
            break;  // Exit the loop if no more cards to read
        }
    }

    fclose(file);
}
/*
void removeCardFromCSV(CARDLIST *cardList, const char *cardId) {
    int indexToRemove = -1;
    for (int i = 0; i < cardList->count; ++i) {
        if (strcmp(cardList->Lista[i].cardId, cardId) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove != -1) {
        for (int i = indexToRemove; i < cardList->count - 1; ++i) {
            cardList->Lista[i] = cardList->Lista[i + 1];
        }

        cardList->count--;
        cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * cardList->count);
        printf("Card ID '%s' removed successfully.\n", cardId);

        // Update the CSV file after removing the card
        FILE *file = fopen("cardMemberList.csv", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        for (int i = 0; i < cardList->count; ++i) {
            writeCardToCSV(&cardList->Lista[i]);
        }

        fclose(file);
    } else {
        printf("Card ID '%s' not found in the list.\n", cardId);
    }
}
*/





//funktion to make a random id card number
void randomGeneratedCardId(CARD *card) {
    sprintf(card->cardId, "%04d", rand() % 10000); //random 4 digit (low digit for testing purpose)
    card->timestamp = time(NULL); //timestamp for card id
    



    ////
    //strncpy(card->cardId, "some_random_id", sizeof(card->cardId));
    ////
}


//checks so it wont dublicate same id
int noCopyOfSameIdCheck(const CARDLIST *cardList, const char *cardId) {
    for (int i = 0; i < cardList->count; ++i) {
        if (strcmp(cardList->Lista[i].cardId, cardId) == 0) {
            return 1; 
        }
    }
    return 0;
}


//funktion to remove id card from list
void removeCardById(CARDLIST *cardList, const char *cardId) {
    int indexToRemove = -1;
    for (int i = 0; i < cardList->count; ++i) {
        if (strcmp(cardList->Lista[i].cardId, cardId) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove != -1) {
        for (int i = indexToRemove; i < cardList->count - 1; ++i) {
            cardList->Lista[i] = cardList->Lista[i + 1];
        }

        cardList->count--;
        cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * cardList->count);
        printf("\nCard ID '%s' removed successfully.\n", cardId);

        //update the CSV file after removing the card
        FILE *file = fopen("cardMemberList.csv", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        for (int i = 0; i < cardList->count; ++i) {
            writeCardToCSV(&cardList->Lista[i]);
        }

        fclose(file);
    } else {
        printf("Card ID '%s' not found in the list.\n", cardId);
    }
}




//funktion for admin menu
void adminMenu(CARDLIST *cardList){
    readCardsFromCSV(cardList);
    while (1){
        
        printf("Admin menu\n");
        printf("1. Remote open door\n");
        printf("2. List all cards in system \n");
        printf("3. add/remove access\n");
        printf("4. Exit\n");
        printf("0. TEST SCAN CARD ID\n"); 


        //user input
        int sel; 
        
        //int count;
        //int *lista;

        GetInputInt("Select: ", &sel);


        //remote open door with three seconds timer
        if (sel == 1){
            printf("Green lamp is on (3 seconds)\n");
            sleep(3);//three second timer for door to be open

            printf("\n");

        }

        
        //list and show card ID
        else if (sel == 2){
            for(int i = 0; i < cardList->count ;i++){
            printf("card ID: %s\t", cardList->Lista[i].cardId);
            printf("Added on: %s\n", ctime(&cardList->Lista[i].timestamp));
            }
        }



        //adding/removing acces
        else if (sel == 3){

            
            while (1) {

                printf("1. Add new card ID\n");
                printf("2. Remove card ID\n");
                printf("3. Back to main menu\n");

                int AddOrRemove;
                GetInputInt("Enter option: ", &AddOrRemove);

                //key one to add new card id
                if (AddOrRemove == 1) {
                    CARD newCard;
                    int maxAttempts = 10;
                    int attemptCount = 0;
    
                    do {
                        randomGeneratedCardId(&newCard);
                        if (!noCopyOfSameIdCheck(cardList, newCard.cardId)) {
                            cardList->count++;
                            cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * cardList->count);
                            cardList->Lista[cardList->count - 1] = newCard;

                            printf("New card ID '%s' added successfully.  %s\n", newCard.cardId, ctime(&newCard.timestamp));
                            writeCardToCSV(&newCard);//csv file saving id cards
                            break;
                        }

                        attemptCount++;
                    } while (attemptCount < maxAttempts);

                    if (attemptCount == maxAttempts) {
                        printf("Error: Unable to generate card ID after %d attempts.\n", maxAttempts);
                    }
                } 




                
                //key 2 if i want to remove
                else if (AddOrRemove == 2) {
                    char cardIdToRemove[20];
                    GetInput("Type ID card number to remove: ", cardIdToRemove, sizeof(cardIdToRemove));

                    int indexToRemove = -1;
                    for (int i = 0; i < cardList->count; ++i) {
                        if (strcmp(cardList->Lista[i].cardId, cardIdToRemove) == 0) {
                            indexToRemove = i;
                            break;
                        }
                    }

                    if (indexToRemove != -1) {
                        //calling the removeCardById function to handle removal
                        removeCardById(cardList, cardIdToRemove);
                        //removeCardFromCSV(cardList, cardIdToRemove);
                    } else {
                        printf("Card ID '%s' not found in the list.\n", cardIdToRemove);
                    }
                }

                //going back to admin menu
                else if (AddOrRemove == 3) {
                    break; 
                } 
                else {
                    printf("Invalid option, please try again\n");
                }
            }
        





//#################################################################################
//testing diffrent ways error handling


/*
            CARD cardsArray[10];

            CARDLIST cardList;
            cardList.Lista = cardsArray;
            cardList.count = 0;
*/
            //GetInputInt("enter card id to add user: %d", &cardsArray);
            /*
            CARD newCard;
            GetInputInt("Enter access card number: ", newCard.cardId);

           
            cardList->count++;
            cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * cardList->count);

            // Add the new card to the list
            cardList->Lista[cardList->count - 1] = newCard;

            printf("Access card added successfully\n\n");

*/

            /*
            CARD newCard;
            printf("Enter access card number: ");
            scanf("%19s", newCard.cardId);

            // Increase count and reallocate memory for Lista
            cardList->count++;
            cardList->Lista = realloc(cardList->Lista, sizeof(CARD) * cardList->count);

            // Add the new card to the list
            cardList->Lista[cardList->count - 1] = newCard;

            printf("Access card added successfully\n\n");
            */
        }

        

//#####################################################################################



        else if(sel == 4){
            break;
        }
    
        else if (sel == 0) {
            char scannedCardId[20];
            GetInput("Scan card ID: ", scannedCardId, sizeof(scannedCardId));

            int foundIndex = -1;
            for (int i = 0; i < cardList->count; ++i) {
                if (strcmp(cardList->Lista[i].cardId, scannedCardId) == 0) {
                    foundIndex = i;
                    break;
                }
            }

            if (foundIndex != -1) {
                printf("Card ID '%s' found in the list.\n", scannedCardId);
            } 
            else 
            {
                printf("Card ID '%s' not found in the list.\n", scannedCardId);
            }
        }
    
    }
}

int main(){

    //FILE *file = fopen("cardMemberList.csv", "a+");


    CARDLIST cardList;

    cardList.Lista = NULL;
    cardList.count = 0;

    //read existing cards from the CSV file
    readCardsFromCSV(&cardList);

    
    takeIdCardFromFile(&cardList);

    //fclose(file);  // Close the file after reading

    adminMenu(&cardList);

    //wave the updated card list back to the file
    FILE *file = fopen("cardMemberList.csv", "w");
    for (int i = 0; i < cardList.count; ++i) {
        writeCardToCSV(&cardList.Lista[i]);
    }

    fclose(file);
    free(cardList.Lista);

    return 0;

//###########################################################################################################
//error handling
/*
    // Save the updated card list back to the file
    file = fopen("cardMemberList.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    for (int i = 0; i < cardList.count; ++i) {
        printf(file, "%s,%s", cardList.Lista[i].cardId, ctime(&cardList.Lista[i].timestamp));
    }

    fclose(file);
    */  
    //takeIdCardFromFile(&cardList);
//#######################################################################################################
}