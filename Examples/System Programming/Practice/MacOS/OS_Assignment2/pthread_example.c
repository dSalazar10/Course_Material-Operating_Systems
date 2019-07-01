#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int *array; //Global integer array
int sum;    //Global sum variable
int counter = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *front_to_back( void *val )
{
    int SIZE;
    SIZE = *((int *) val);

    while (1)
    {
        pthread_mutex_lock(&mutex);

        if( counter != SIZE )
        {
            sum = sum + array[counter];

            printf("Thread_1 > read %d, sum is = %d \n", array[counter], sum);

            counter++;

        }else
        {
            printf("Thread_1 > End. \n");

            pthread_mutex_unlock(&mutex);

            break;
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


void *back_to_front( void *val )
{
    int SIZE;
    SIZE = *((int *) val);

    int i=1;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        if( counter != SIZE )
        {
            sum = sum + array[SIZE - i];

            printf("Thread_2 > read %d, sum is = %d \n", array[SIZE - i], sum);

            counter++;
            i++;

        }else
        {
            printf("Thread_2 > End. \n");

            pthread_mutex_unlock(&mutex);

            break;
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {

    int SIZE;

    printf("Array size? :");
    scanf("%d", &SIZE);

    array = malloc(SIZE*sizeof(int));

    int i=0;

    printf("Array of contents? :");
    for (; i<SIZE ; i++)
    {
        scanf("%d", &array[i]);
    }

    pthread_t t[2];

    pthread_create(&t[0], NULL, front_to_back, (void *) &SIZE );
    pthread_create(&t[1], NULL, back_to_front, (void *) &SIZE );

    pthread_join(t[0],NULL);
    pthread_join(t[1],NULL);

    free(array);

    return 0;

}