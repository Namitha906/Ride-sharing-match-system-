#include "rideshare.h"

int main() {
    int choice, id, loc;
    char name[50];
    initSystem();

    while (1) {
        printf("\n----------   RIDE SHARING DASHBOARD   ----------\n");
        printf("1. Register Driver\n");
        printf("2. Register Rider\n");
        printf("3. Match Ride\n");
        printf("4. Complete Ride\n");
        printf("5. Show Driver Earnings\n");
        printf("6. Delete Driver\n");
        printf("7. View System Status\n");
        printf("8. Exit\n");
        printf("------------------------------------------------\n \n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            printf("Enter Driver ID: ");
            scanf("%d", &id);
            printf("Enter Driver Name: ");
            scanf("%s", name);
            printf("Enter Driver Location (0-9): ");
            scanf("%d", &loc);
            addDriver(id, name, loc);
            break;

        case 2:
            printf("Enter Rider ID: ");
            scanf("%d", &id);
            printf("Enter Rider Name: ");
            scanf("%s", name);
            printf("Enter Rider Location (0-9): ");
            scanf("%d", &loc);
            addRider(id, name, loc);
            break;

        case 3:
            matchRide();
            break;

        case 4:
            printf("Enter Ride ID to complete: ");
            scanf("%d", &id);
            completeRide(id);
            break;

        case 5:
            printf("Enter Driver ID: ");
            scanf("%d", &id);
            showDriverEarnings(id);
            break;
        
        case 6:
            printf("Enter Driver ID to delete: ");
            scanf("%d", &id);
            deleteDriver(id);
            break;

        case 7:
            printSystemState();
            break;

        case 8:
            cleanupSystem();
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
