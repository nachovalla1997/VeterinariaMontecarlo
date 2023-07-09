#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CLIENTS_PER_HOUR 2.0
#define DELIVERY_PER_HOUR 6.8
#define WORKING_HOURS 12
#define WORKING_DAYS 365

#define START_CLIENT_ID 1000
#define START_DELIVERY_ID 1000
#define START_DAY 8
#define START_MONTH 1
#define START_YEAR 23
#define START_HOUR 8
#define START_MINUTE 0
#define START_SECOND 0
#define TOTAL_CLIENTS 0
#define TOTAL_DELIVERY 0

double poisson(double lambda, int k) {
    return exp(-lambda) * pow(lambda, k) / tgamma(k + 1);
}

typedef struct {
    int month;
    int total_clients;
} results;



struct tm increment_day(struct tm time) {
    time.tm_mday++;
    if (time.tm_mday > 31) {
        printf("End of month %d\n", time.tm_mon + 1);
        time.tm_mday = 1;
        time.tm_mon++;
        if (time.tm_mon > 11) {
            printf("End of year %d\n", time.tm_year + 1);
            time.tm_mon = 1;
            time.tm_year++;
        }
    }
    return time;
}

void print_results_table(results* results_array, int num_months, char* title) {
    printf("\n%s\n", title);
    printf("Month\tTotal Clients\n");
    for (int i = 0; i < num_months; i++) {
        printf("%d\t%d\n", i + 1, results_array[i].total_clients);
    }
}


int calculate_clients(double lambda) {
    int arrivals = 0;
    double u = (double) rand() / RAND_MAX;
    double prob = poisson(lambda, arrivals);

    while (u >= prob) {
        arrivals++;
        prob += poisson(lambda, arrivals);
    }

    return arrivals;
}

int calculate_minutes(int arrivals, int client) {
    int interval = 60 / arrivals; // Calculate the interval length

    // Calculate the start and end minutes for the current client
    int start_minute = interval * client;
    int end_minute = start_minute + interval - 1;

    // Generate a random number within the specified range
    return rand() % (end_minute - start_minute + 1) + start_minute;
}

struct tm manage_time_overflow(struct tm start_time) {
    while (start_time.tm_min >= 60) {
        start_time.tm_min -= 60;
        start_time.tm_hour++;
    }

    return start_time;
}

void process_client(struct tm start_time, int* client_id) {
    char date_time[19];
    strftime(date_time, 19, "%d/%m/%y %H:%M:%S", &start_time);
    printf("Client ID: %d | Date & Time: %s\n", (*client_id)++, date_time);
}

void simulate_client_arrivals_per_day(struct tm start_time, int* client_id, double clients_per_hour, results* results) {
    int total_arrivals = 0;
    for (int hour = 0; hour < WORKING_HOURS; hour++) {
        start_time.tm_hour = START_HOUR + hour;
        start_time.tm_min = START_MINUTE;
        int arrivals = calculate_clients(clients_per_hour);
        total_arrivals += arrivals;
        printf("Arrivals: %d\n", arrivals);
        for (int client = 0; client < arrivals; client++) {
            int minutes = calculate_minutes(arrivals, client);
            start_time.tm_min = minutes;
            process_client(start_time, client_id);
        }
    }
    results[start_time.tm_mon].total_clients += total_arrivals;
}

void simulate_delivery_arrivals_per_day(struct tm start_time, int* delivery_id, double delivery_per_hour, results* results) {
    int total_deliveries = 0;
    for (int hour = 0; hour < WORKING_HOURS; hour++) {
        start_time.tm_hour = START_HOUR + hour;
        start_time.tm_min = START_MINUTE;
        int arrivals = calculate_clients(delivery_per_hour);
        total_deliveries += arrivals;
        printf("Delivery's: %d\n", arrivals);
        for (int delivery = 0; delivery < arrivals; delivery++) {
            int minutes = calculate_minutes(arrivals, delivery);
            start_time.tm_min = minutes;
            process_client(start_time, delivery_id);
        }
    }
    results[start_time.tm_mon].total_clients += total_deliveries;
}


void simulate_client_arrivals(int start_client_id, double clients_per_hour, int days, results* results) {
    srand(time(NULL));  // Initialize random number generator

    struct tm start_time = {0, START_MINUTE, START_HOUR, START_DAY, START_MONTH - 1, START_YEAR + 100, 0, 0, 0};
    int client_id = start_client_id;

    for (int day = 0; day < days; day++) {
        if(day != 0) {
            printf("End of day %d\n", day + 1);
            start_time = increment_day(start_time);
        }
        simulate_client_arrivals_per_day(start_time, &client_id, clients_per_hour, results);
    }
}

void simulate_delivery_arrivals(int start_delivery_id, double delivery_per_hour, int days, results* results) {
    srand(time(NULL));  // Initialize random number generator

    struct tm start_time = {0, START_MINUTE, START_HOUR, START_DAY, START_MONTH - 1, START_YEAR + 100, 0, 0, 0};
    int delivery_id = start_delivery_id;

    for (int day = 0; day < days; day++) {
        if(day != 0) {
            printf("End of day %d\n", day + 1);
            start_time = increment_day(start_time);
        }
        simulate_delivery_arrivals_per_day(start_time, &delivery_id, delivery_per_hour, results);
    }
}

int main() {
    results resultsArrivals [12];
    results resultsDeliveries [12];
    simulate_client_arrivals(START_CLIENT_ID, CLIENTS_PER_HOUR, WORKING_DAYS, resultsArrivals);
    simulate_delivery_arrivals(START_DELIVERY_ID, DELIVERY_PER_HOUR, WORKING_DAYS, resultsDeliveries);
    print_results_table(resultsArrivals, 12, "Arrivals");
    print_results_table(resultsDeliveries, 12, "Deliveries");
    return 0;
}
