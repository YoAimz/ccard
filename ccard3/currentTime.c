#include <stdio.h>
#include <time.h>
    
    time_t currentTime;
    time(&currentTime);

    // Convert the time to a string format
    char* timeString = ctime(&currentTime);

    // Print the current time
    printf("Current time: %s", timeString);