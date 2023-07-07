#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CLIENTS_PER_HOUR 4.0
#define WORKING_HOURS 12
#define WORKING_DAYS 5

#define START_CLIENT_ID 1000
#define START_DAY 8
#define START_MONTH 6
#define START_YEAR 23
#define START_HOUR 8
#define START_MINUTE 0
#define START_SECOND 0

struct tm start_time = {0, START_MINUTE, START_HOUR, START_DAY, START_MONTH - 1, START_YEAR + 100, 0, 0, 0};

double poisson(double lambda, int k) {
    return exp(-lambda) * pow(lambda, k) / tgamma(k + 1);
}

// Now simulating client arrivals also changes time
void simulate_client_arrivals(struct tm start_time, int start_client_id, double clients_per_hour, int days) {
    srand(time(NULL));  // Initialize random number generator

    int client_id = start_client_id;

    for (int day = 0; day < days; day++) {
        if(day != 0) {
        printf("End of day %d\n", day + 1);
            start_time.tm_mday++;
            if (start_time.tm_mday > 31) {
                printf("End of month %d\n", start_time.tm_mon + 1);
                start_time.tm_mday = 1;
                start_time.tm_mon++;
                if (start_time.tm_mon > 11) {
                    printf("End of year %d\n", start_time.tm_year + 1);
                    start_time.tm_mon = 1;
                    start_time.tm_year++;
                }
            }
        }
        for (int hour = 0; hour < WORKING_HOURS; hour++) {
            start_time.tm_hour = START_HOUR + hour;
            double lambda = clients_per_hour;
            int arrivals = 0;
            double u = (double) rand() / RAND_MAX;
            double prob = poisson(lambda, arrivals);

            while (u >= prob) {
                arrivals++;
                prob += poisson(lambda, arrivals);
            }

            for (int client = 0; client < arrivals; client++) {
                double lambda_minutes = 60.0 / arrivals;
                int minutes = 0;
                double u_minutes = (double) rand() / RAND_MAX;
                double prob_minutes = poisson(lambda_minutes, minutes);

                while (u_minutes >= prob_minutes) {
                    minutes++;
                    prob_minutes += poisson(lambda_minutes, minutes);
                }

                start_time.tm_min += minutes;

                // Manage hours overflow
                while (start_time.tm_min >= 60) {
                    start_time.tm_min -= 60;
                    start_time.tm_hour++;
                }

                char date_time[19];
                strftime(date_time, 19, "%d/%m/%y %H:%M:%S", &start_time);
                printf("Client ID: %d | Date & Time: %s\n", client_id++, date_time);
            }
        }
    }
}


int main() {
    simulate_client_arrivals(start_time, START_CLIENT_ID, CLIENTS_PER_HOUR, WORKING_DAYS);
    return 0;
}
