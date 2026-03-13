#include "rideshare.h"

static RiderQueue *riderQueue = NULL;
static int rideCounter = 1;

static Ride rides[MAX_RIDERS];
static int rideCount = 0;

static int graph[MAX_LOCATIONS][MAX_LOCATIONS];

HashEntry driverTable[TABLE_SIZE];

//queue implimentation
RiderQueue *createQueue() {
    RiderQueue *q = (RiderQueue *)malloc(sizeof(RiderQueue));
    q->front = q->rear = NULL;
    return q;
}

void enqueueRider(RiderQueue *q, Rider rider) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->rider = rider;
    temp->next = NULL;
    if (!q->rear)
        q->front = q->rear = temp;   
    else {
        q->rear->next = temp;        
        q->rear = temp;
    }
}

Rider dequeueRider(RiderQueue *q) {
    Rider empty = {-1, "", -1};    
    if (!q->front)
        return empty;            
    Node *temp = q->front;
    Rider r = temp->rider;
    q->front = q->front->next;    
    if (!q->front)                
        q->rear = NULL;  
    free(temp);            
    return r;
}

int isQueueEmpty(RiderQueue *q) {
    return (q->front == NULL);
}

void freeQueue(RiderQueue *q) {
    while (!isQueueEmpty(q))
        dequeueRider(q);
    free(q);
}

//graph implementation
void initGraph() {
    for (int i = 0; i < MAX_LOCATIONS; ++i) {
        for (int j = 0; j < MAX_LOCATIONS; ++j) {
            if (i == j) graph[i][j] = 0;
            else graph[i][j] = abs(i - j);  //linear difference bw loc i and j
        }
    }
}

void addEdge(int src, int dest, int distance) {     //add custom path-distance
    if (src < MAX_LOCATIONS && dest < MAX_LOCATIONS) {
        graph[src][dest] = distance;
        graph[dest][src] = distance;     
    }
}


int getShortestDistance(int start, int end) {
    if (start < 0 || start >= MAX_LOCATIONS || end < 0 || end >= MAX_LOCATIONS)
        return INF;
    return graph[start][end];   //return dist using adjacency matrix
}


//Hashmap implimentation
int hashFunction(int key) {
    return key % TABLE_SIZE;   //index where driver will be stored in hashtable
}

void initDriverHash() {
    for (int i = 0; i < TABLE_SIZE; i++)
        driverTable[i].occupied = 0;
}

void addDriverToHash(Driver d) {
    int index = hashFunction(d.id);
    int startIndex = index;
    while (driverTable[index].occupied) {
        index = (index + 1) % TABLE_SIZE;  //linear probing (handles collision)
        if (index == startIndex) {
            printf("Hash table full! Cannot add driver %d\n", d.id);
            return;
        }
    }
    driverTable[index].key = d.id;
    driverTable[index].value = d;
    driverTable[index].occupied = 1;
}

Driver *getDriverFromHash(int id) {
    int index = hashFunction(id);
    int startIndex = index;
    while (driverTable[index].occupied) {
        if (driverTable[index].key == id)      //scan till key is found
            return &driverTable[index].value;

        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex)
            break;
    }
    return NULL;
}


void addDriver(int id, char *name, int location) {
    Driver d;
    d.id = id;
    strcpy(d.name, name);
    d.location = location;
    d.available = 1;
    d.earnings = 0;

    addDriverToHash(d);
    printf("Driver %s (ID %d) added.  Location: %d\n", name, id, location);
}
   

void addRider(int id, char *name, int location) {
    Rider rider;
    rider.id = id;
    strcpy(rider.name, name);
    rider.location = location;
    enqueueRider(riderQueue, rider);
    printf("Rider %s (ID %d) added to queue.  Location: %d\n", name, id, location);
}

void matchRide() {
    if (isQueueEmpty(riderQueue)) {
        printf("No riders waiting.\n");
        return;
    }

    Rider rider = dequeueRider(riderQueue);    //get next rider- fifo
    int minDist = INF;
    Driver *chosenDriver = NULL;          

    for (int i = 0; i < TABLE_SIZE; i++) {             
        if (driverTable[i].occupied) {    
            Driver *d = &driverTable[i].value;
            
            if (d->available) {
                int dist = getShortestDistance(d->location, rider.location);   
                if (dist < minDist) {              
                    minDist = dist;             //nearest driver- greedy priority
                    chosenDriver = d;            
                }
            }
        }
    }

    if (!chosenDriver) {
        printf("No available drivers for rider %s.\n", rider.name);
        enqueueRider(riderQueue, rider);
        return;
    }

    float fare = calculateFare(minDist);
    int rideID = rideCounter++;

    rides[rideCount++] =
        (Ride){ rideID, rider.id, chosenDriver->id, 0, fare };   //create ride record

    chosenDriver->available = 0; 
    chosenDriver->earnings += fare;

    printf("\nMatched Ride:\n");
    printf("Driver: %s | Distance: %d | Fare: Rs. %.2f | RideID: %d\n", 
        chosenDriver->name, minDist, fare, rideID);
}


void completeRide(int rideId) {
    for (int i = 0; i < rideCount; i++) {
        if (rides[i].rideId == rideId && rides[i].status == 0) {

            rides[i].status = 1;

            int did = rides[i].driverId;

            Driver *d = getDriverFromHash(did);
            if (d)
                d->available = 1;
            printf("Ride %d completed. Driver %s is now available.\n",
                   rideId, d ? d->name : "Unknown");

            return;
        }
    }
    printf("Invalid ride ID.\n");
}

//additional functional implimentation
float calculateFare(int distance) {
    float base = 30.0;          
    float perKm = 10.0;         
    return base + (distance * perKm);
}

void deleteDriver(int driverId) {
    int index = hashFunction(driverId);
    int start = index;

    while (driverTable[index].occupied) {
        if (driverTable[index].key == driverId) {
            printf("Driver %s earned Rs %.2f today.\n", driverTable[index].value.name, driverTable[index].value.earnings);

            driverTable[index].occupied = 0;
            printf("Driver removed.\n");
            return;
        }

        index = (index + 1) % TABLE_SIZE;
        if (index == start)
            break;
    }
    printf("Driver not found.\n");
}

void showDriverEarnings(int driverId) {
    Driver *d = getDriverFromHash(driverId);

    if (d) {
        printf("Driver %s has earned: Rs. %.2f\n",
               d->name, d->earnings);
        return;
    }

    printf("Driver not found.\n");
}


//sytem
void initSystem() {
    rideCounter = 1;
    rideCount = 0;
    riderQueue = createQueue();
    initGraph();
    initDriverHash();

    //custom connections (locations 0–5)
    addEdge(0, 1, 3);       
    addEdge(1, 2, 5);
    addEdge(2, 3, 2);
    addEdge(3, 4, 4);
    addEdge(4, 5, 6);
    addEdge(0, 5, 10);

    printf("Ride Sharing System Initialized\n");
}

void cleanupSystem() {
    if (riderQueue)
        freeQueue(riderQueue);
    printf("System cleaned up.\n");
}

void printSystemState() {
    printf("\n..............   System Status   ..............\n \n");
    printf("Drivers:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (driverTable[i].occupied){
        Driver drivers = driverTable[i].value;
        printf("ID:%d | Name:%-10s | Loc:%d | %s\n",
               drivers.id, drivers.name, drivers.location, drivers.available ? "Available" : "Busy");
        }
    }

    printf("\nRiders waiting:\n");
    Node *temp = riderQueue->front;    
    if (!temp) printf("None\n");   
    while (temp) {                       
        printf("ID:%d | Name:%-10s | Loc:%d\n",
               temp->rider.id, temp->rider.name, temp->rider.location);
        temp = temp->next;
    }

    printf("\nRide History:\n");
    for (int i = 0; i < rideCount; i++) {
    printf("RideID:%d | Driver:%d | Rider:%d | Fare: Rs. %.2f | %s\n",
       rides[i].rideId, rides[i].driverId, rides[i].riderId, rides[i].fare,
       rides[i].status ? "Completed" : "Ongoing");
    }

    printf(".................................................\n \n");
}
