#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POTENTIAL_ARRIVALS_PER_HOUR 2.0
#define POTENTIAL_DELIVERIES_PER_HOUR 6.8
#define OPERATIONING_HOURS 12
#define OPERATIONING_DAYS 365
#define MONTHS_IN_A_YEAR 12

double poisson(double lambda, int k) {
    return exp(-lambda) * pow(lambda, k) / tgamma(k + 1);
}

typedef struct {
    int total;
} record;


struct tm increment_day(struct tm time) {
    time.tm_mday++;
    if (time.tm_mday > 31) {
        time.tm_mday = 1;
        time.tm_mon++;
        if (time.tm_mon > 11) {
            time.tm_mon = 0;
            time.tm_year++;
        }
    }
    return time;
}

void print_records(record* records, char* title) {
    printf("\n%s\n", title);
    printf("Month\tTotal\n");
    for (int i = 0; i < MONTHS_IN_A_YEAR; i++) {
        printf("%d\t%d\n", i + 1, records[i].total);
    }
}

int calculate_potential(double lambda) {
    int potential = 0;
    double u = (double) rand() / RAND_MAX;
    double prob = poisson(lambda, potential);

    while (u >= prob) {
        potential++;
        prob += poisson(lambda, potential);
    }

    return potential;
}

void simulate_potential_per_day(struct tm start_time, record* records, double lambda) {
    int total_potential = 0;
    for (int hour = 0; hour < OPERATIONING_HOURS; hour++) {
        start_time.tm_hour = hour;
        int potential = calculate_potential(lambda);
        total_potential += potential;
    }
    records[start_time.tm_mon].total += total_potential;
}

void simulate_potential(int days, record* records, double lambda) {
    srand(time(NULL));

    struct tm start_time = {0, 0, 0, 1, 0, 113, 0, 0, 0};

    for (int day = 0; day < days; day++) {
        if(day != 0) {
            start_time = increment_day(start_time);
        }
        simulate_potential_per_day(start_time, records, lambda);
    }
}

void initialize_records(record* records) {
    for (int i = 0; i < MONTHS_IN_A_YEAR; i++) {
        records[i].total = 0;
    }
}

int main() {
    record clientArrivals [MONTHS_IN_A_YEAR];
    record deliveryArrivals [MONTHS_IN_A_YEAR];
    initialize_records(clientArrivals);
    initialize_records(deliveryArrivals);
    simulate_potential(OPERATIONING_DAYS, clientArrivals, POTENTIAL_ARRIVALS_PER_HOUR);
    simulate_potential(OPERATIONING_DAYS, deliveryArrivals, POTENTIAL_DELIVERIES_PER_HOUR);
    print_records(clientArrivals, "Client Arrivals");
    print_records(deliveryArrivals, "Delivery Arrivals");
    return 0;
}
