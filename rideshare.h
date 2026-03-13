#ifndef RIDESHARING_H
#define RIDESHARING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_RIDERS 20
#define MAX_LOCATIONS 10
#define INF 9999
#define TABLE_SIZE 50

typedef struct {
    int id;
    char name[50];
    int location;
} Rider;

typedef struct {
    int id;
    char name[50];
    int location;
    int available; //0=busy, 1=available
    float earnings;
} Driver;

typedef struct {
    int rideId;
    int riderId;
    int driverId;
    int status; //0=ongoing, 1=completed
    float fare;
} Ride;

typedef struct Node {
    Rider rider;
    struct Node *next;
} Node;

typedef struct {
    Node *front, *rear;
} RiderQueue;

//hashtable
typedef struct {
    int key;      //driver ID
    Driver value; //driver data
    int occupied; //0=empty,1=filled
} HashEntry;

extern HashEntry driverTable[TABLE_SIZE];


//hashmap functions
int hashFunction(int key);
void initDriverHash();
void addDriverToHash(Driver d);
Driver *getDriverFromHash(int id);

//Queue functions
RiderQueue *createQueue();
void enqueueRider(RiderQueue *q, Rider rider);
Rider dequeueRider(RiderQueue *q);
int isQueueEmpty(RiderQueue *q);
void freeQueue(RiderQueue *q);

//System functions
void initSystem();

void addDriver(int id, char *name, int location);
void addRider(int id, char *name, int location);

void matchRide();
void completeRide(int rideId);

void cleanupSystem();
void printSystemState();

//Graph functions
void initGraph();
void addEdge(int src, int dest, int distance);
int getShortestDistance(int start, int end);

//given functinal implimentations
float calculateFare(int distance);
void deleteDriver(int driverId);
void showDriverEarnings(int driverId);

#endif
