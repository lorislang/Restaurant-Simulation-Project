#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <time.h>
#include "myqueue.h"

struct cook_data {
    int cookId;
};

struct guest_data {
    int guestId;
    int orderId;
};

myqueue queue;
pthread_mutex_t mutexCounter;

bool counterEmpty = true;
bool orderCooked = false;
int counter = 0;


void* guest(void* param) {

    time_t start, end;
    double diff;
    time(&start);

    struct guest_data *data = (struct guest_data*) param;
    bool served = false;
    printf("Guest %d has made meal order %d\n", data->guestId, data->orderId);
    
    while(!served) {
        
        pthread_mutex_lock(&mutexCounter);
        if (orderCooked && counter == data->orderId) {
            served = true;
            counterEmpty = true;
            orderCooked = false;
        }
        pthread_mutex_unlock(&mutexCounter);
        usleep(100 * 1000);
    }
    time(&end);
    diff = difftime(end, start);
    printf("Guest %d has picked up order %d after %d ms\n", data->guestId, data->orderId, (int) diff/1000.);

}

void* cook(void* param) {

    struct cook_data *data = (struct cook_data*) param;
    int order;
    while(!myqueue_is_empty(&queue)) { //cooks wait until their first order
        pthread_mutex_lock(&mutexCounter);
        order = myqueue_pop(&queue);
        pthread_mutex_unlock(&mutexCounter);
    

        printf("Cook %d is preparing meal %d\n", data->cookId, order);
        int cooktime = (rand() % 400) + 100;
        usleep(cooktime * 1000);
        printf("Cook %d has finished meal %d\n", data->cookId, order);

        bool placedOrder = false;
        while(!placedOrder) {
            pthread_mutex_lock(&mutexCounter);
            if (counterEmpty ) {
                placedOrder = true;
                counterEmpty = false;
                orderCooked = true;
                counter = order;
                printf("Cook %d is putting meal %d on the counter\n", data->cookId, order);
            }
            pthread_mutex_unlock(&mutexCounter);
        }
    }
}


int main (int argc, char** argv) {

    int notifications = atoi(argv[1]);
    int numberOfGuest = atoi(argv[2]); 
    int numberOfCooks = atoi(argv[3]);
    pthread_t tidg[numberOfGuest], tidc[numberOfCooks];

    struct cook_data cookIds[numberOfCooks];
    struct guest_data guestIds[numberOfGuest];
    
    myqueue_init(&queue);
    pthread_mutex_init(&mutexCounter, NULL);

    for (int i = 0; i < numberOfGuest; i++) {
        guestIds[i].guestId = i;
        guestIds[i].orderId = i;
        myqueue_push(&queue, i);
        pthread_create(&tidg[i], NULL, guest, (void*) &guestIds[i]);
    }
   
    for (int i = 0; i < numberOfCooks; i++) {
        cookIds[i].cookId = i;
        pthread_create(&tidc[i], NULL, cook, (void*) &cookIds[i]);
    }

 

    for (int i = 0; i < numberOfGuest; i++) {
        pthread_join(tidg[i], NULL);
    }

    for (int i = 0; i < numberOfCooks; i++) {
        pthread_join(tidc[i], NULL);
    }

    pthread_mutex_destroy(&mutexCounter);
}