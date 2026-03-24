#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 20

struct Ticket {
    int ticketID;
    char name[50];
    int route;
    int seat;
    float fare;
};

char *routes[] = {
    "Vellore -> Chennai",
    "Chennai -> Bangalore",
    "Chennai -> T Nagar"
};

float fares[] = {120.0, 350.0, 50.0};

int seatMatrix[3][MAX_SEATS] = {0};

void showRoutes() {
    printf("\nAvailable Routes:\n");
    for(int i = 0; i < 3; i++) {
        printf("%d. %s (Fare: %.2f)\n", i+1, routes[i], fares[i]);
    }
}

void showSeats(int route) {
    printf("\nSeat Status (0 = Available, 1 = Booked)\n");
    for(int i = 0; i < MAX_SEATS; i++) {
        printf("Seat %d : %d\n", i+1, seatMatrix[route][i]);
    }
}

void bookTicket() {
    struct Ticket t;
    FILE *fp = fopen("tickets.txt", "a");

    if(fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("\nEnter Passenger Name: ");
    scanf("%s", t.name);

    showRoutes();
    printf("Select Route: ");
    scanf("%d", &t.route);

    if(t.route < 1 || t.route > 3) {
        printf("Invalid Route!\n");
        fclose(fp);
        return;
    }

    showSeats(t.route-1);

    printf("Enter Seat Number (1-%d): ", MAX_SEATS);
    scanf("%d", &t.seat);

    if(t.seat < 1 || t.seat > MAX_SEATS) {
        printf("Invalid Seat!\n");
        fclose(fp);
        return;
    }

    if(seatMatrix[t.route-1][t.seat-1] == 1) {
        printf("Seat already booked!\n");
        fclose(fp);
        return;
    }

    seatMatrix[t.route-1][t.seat-1] = 1;

    t.fare = fares[t.route-1];
    t.ticketID = rand() % 10000 + 1000;

    fprintf(fp,"%d %s %d %d %.2f\n",t.ticketID,t.name,t.route,t.seat,t.fare);

    printf("\nTicket Booked Successfully!\n");
    printf("Ticket ID: %d\n",t.ticketID);

    fclose(fp);
}

void searchTicket() {
    int id, found = 0;
    struct Ticket t;
    FILE *fp = fopen("tickets.txt","r");

    if(fp == NULL) {
        printf("No bookings yet.\n");
        return;
    }

    printf("Enter Ticket ID: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d %s %d %d %f",&t.ticketID,t.name,&t.route,&t.seat,&t.fare)!=EOF) {
        if(t.ticketID == id) {
            printf("\nTicket Found!\n");
            printf("Name: %s\n",t.name);
            printf("Route: %s\n",routes[t.route-1]);
            printf("Seat: %d\n",t.seat);
            printf("Fare: %.2f\n",t.fare);
            found = 1;
            break;
        }
    }

    if(!found)
        printf("Ticket not found!\n");

    fclose(fp);
}

void cancelTicket() {
    int id, found = 0;
    struct Ticket t;
    FILE *fp = fopen("tickets.txt","r");
    FILE *temp = fopen("temp.txt","w");

    printf("Enter Ticket ID to cancel: ");
    scanf("%d",&id);

    while(fscanf(fp,"%d %s %d %d %f",&t.ticketID,t.name,&t.route,&t.seat,&t.fare)!=EOF) {
        if(t.ticketID == id) {
            printf("Ticket cancelled!\n");
            seatMatrix[t.route-1][t.seat-1] = 0;
            found = 1;
        } else {
            fprintf(temp,"%d %s %d %d %.2f\n",t.ticketID,t.name,t.route,t.seat,t.fare);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("tickets.txt");
    rename("temp.txt","tickets.txt");

    if(!found)
        printf("Ticket not found!\n");
}

void report() {
    struct Ticket t;
    FILE *fp = fopen("tickets.txt","r");

    int count[3] = {0};
    float revenue[3] = {0};

    while(fscanf(fp,"%d %s %d %d %f",&t.ticketID,t.name,&t.route,&t.seat,&t.fare)!=EOF) {
        count[t.route-1]++;
        revenue[t.route-1] += t.fare;
    }

    printf("\nDaily Collection Report\n");

    for(int i = 0; i < 3; i++) {
        printf("%s\n",routes[i]);
        printf("Tickets Booked: %d\n",count[i]);
        printf("Revenue: %.2f\n\n",revenue[i]);
    }

    fclose(fp);
}

int main() {
    int choice;

    while(1) {
        printf("\n=== BUS RESERVATION SYSTEM ===\n");
        printf("1. Book Ticket\n");
        printf("2. Show Available Seats\n");
        printf("3. Search Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Daily Report\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                bookTicket();
                break;

            case 2: {
                int r;
                showRoutes();
                printf("Select route: ");
                scanf("%d",&r);

                if(r>=1 && r<=3)
                    showSeats(r-1);
                else
                    printf("Invalid route!\n");

                break;
            }

            case 3:
                searchTicket();
                break;

            case 4:
                cancelTicket();
                break;

            case 5:
                report();
                break;

            case 6:
                printf("Exiting Program...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}