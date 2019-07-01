/* ferry.c
Daniel Salazar
https://www.tldp.org/LDP/lpg/node21.html
*/
#include "ferry.h"
#include "message.h"

int main()
{
    srand(time(NULL));
    CreateSchedule();
    printf("Created Schedule\n");
    struct CAR e;
    AddEl(schedule.ferry, &e);
    printf("Added a Car\n");
    PrintParking(schedule.ferry);
    DestroySchedule();
    printf("Destroyed Schedule\n");
    return 0;
}