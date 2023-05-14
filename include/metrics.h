#ifndef METRICS_H
#define METRICS_H

struct Metrics {
    int time_elapsed = 0;
    int revenue_earned = 0;
    int total_revenue = 0;
    int total_jobs = 0;
    int total_jobs_completed = 0;
    double avg_turnaround = 0;
    double avg_response = 0;
};

#endif