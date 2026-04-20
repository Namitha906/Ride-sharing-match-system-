# Ride-sharing-match-system-
Ride Sharing Match System using HashMap, Queue, Priority Queue, and Graph


## Problem Statement:
Ride Sharing Match System - Match riders with available drivers based on proximity and queue requests efficiently.


## Functional Requirements:
1. addRider(rider) – enqueue new rider request.
2. addDriver(driver) – register driver.
3. matchRide() – assign nearest available driver to waiting rider.
4. completeRide(rideId) – mark ride complete and free the driver.


## Additional Functional Implimentations Given:
1. calculateFare(distance) - total ride fare (Fare = BaseFare + (Distance × RatePerKm)).
2. deleteDriver(driverId) - removes driver from system.
3. showDriverEarnings(driverId) - total earnings of a driver.


## Data Structures Used:
1. Queue (Linked-list queue) - Manage waiting rider requests (FIFO).
    Used in: enqueueRider(), dequeueRider(), addRider(), matchRide()
2. HashMap (A linear-probing hash table) - Store and retrieve drivers in O(1) time.
    Used in: addDriverToHash(), getDriverFromHash(), completeRide()              
3. Graph - Represent location distances.
    Used in: initGraph(), addEdge(), getShortestDistance(), matchRide()                         
4. Priority Queue Logic - choose the nearest available driver for a rider. 
5. Structs - To represent Drivers, Riders, and Rides.


## Compile Instructions:
In terminal
- gcc rideshare_main.c rideshare.c -o rideshare
- ./rideshare (or) rideshare


## Program Menu:
1. Register Driver
2. Register Rider
3. Match Ride
4. Complete Ride
5. Show Driver Earnings
6. Delete Driver
7. View System Status
8. Exit


## File Structure:
1. README.md - Documentation & usage instructions
2. rideshare.c - Core logic (queue, graph, matching, ride completion)
3. rideshare.h - Header with structs & function declarations
4. rideshare_main.c-Menu-driven interface & user inputs.

## Author
Nihira Hassan






