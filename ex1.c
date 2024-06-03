#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *burstFile, *arrivalFile;
    int count = 0;
    double time, current_time = 0, waiting_time_sum = 0, response_time_sum = 0, queue_length_sum = 0, system_length_sum = 0;
    double *burstTimes, *arrivalTimes, *interArrivalTimes;

    burstFile = fopen("burstTimes.txt", "r");
    arrivalFile = fopen("interArrTimes.txt", "r");

    if (burstFile == NULL || arrivalFile == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Count the number of lines in the burst times file to allocate memory
    while (fscanf(burstFile, "%lf", &time) == 1) {
        count++;
    }
    rewind(burstFile); // Reset file pointer to the beginning for reading

    burstTimes = (double *)malloc(count * sizeof(double));
    arrivalTimes = (double *)malloc(count * sizeof(double));
    interArrivalTimes = (double *)malloc(count * sizeof(double));

    if (burstTimes == NULL || arrivalTimes == NULL || interArrivalTimes == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        fscanf(burstFile, "%lf", &burstTimes[i]);
    }
    fclose(burstFile);

    // Read the inter-arrival times and compute the actual arrival times
    for (int i = 0; i < count; i++) {
        fscanf(arrivalFile, "%lf", &interArrivalTimes[i]);
        arrivalTimes[i] = (i == 0) ? interArrivalTimes[i] : arrivalTimes[i - 1] + interArrivalTimes[i];
    }
    fclose(arrivalFile);

    // FCFS Scheduling Simulation
    for (int i = 0; i < count; i++) {
        double arrival_time = arrivalTimes[i];
        double burst_time = burstTimes[i];
        double start_time = (arrival_time > current_time) ? arrival_time : current_time;
        double finish_time = start_time + burst_time;
        double waiting_time = start_time - arrival_time;

        waiting_time_sum += waiting_time;
        response_time_sum += finish_time - arrival_time;
        queue_length_sum += waiting_time;
        system_length_sum += finish_time - arrival_time;

        current_time = finish_time;
    }
    
    double total_observation_time = current_time;

    double average_waiting_time = waiting_time_sum / count;
    double average_system_response_time = response_time_sum / count;
    double average_queue_length = queue_length_sum / total_observation_time;
    double average_system_length = system_length_sum / total_observation_time;

    printf("Average waiting time: %.2f ms\n", average_waiting_time);
    printf("Average system response time: %.2f ms\n", average_system_response_time);
    printf("Average number of processes in the queue: %.2f\n", average_queue_length);
    printf("Average number of processes in the system: %.2f\n", average_system_length);

    free(burstTimes);
    free(arrivalTimes);
    free(interArrivalTimes);

    return 0;
}