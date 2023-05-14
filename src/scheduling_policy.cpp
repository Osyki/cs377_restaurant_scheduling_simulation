#include "scheduling_policy.h"
#include <iostream>

void SchedulingPolicy::print_jobs() {
    std::list<Customer> xs = completed_jobs;
            std::cout << "Processes:" << std::endl;
            while (!xs.empty()) {
                Customer p = xs.front();
                std::cout << "\tarrival=" << p.arrival
                    << ", duration=" << p.duration
                    << ", willingness_to_wait=" << p.willingness_to_wait
                    << ", revenue=" << p.revenue
                    << ", first_run=" << p.first_run
                    << ", completion=" << p.completion << std::endl;
                xs.pop_front();
            }
}

void SchedulingPolicy::print_metrics() {
    std::cout << "Metrics:" << std::endl;
    // TODO: calculate metrics
    // std::cout << "\taverage_turnaround_time=" << metrics.average_turnaround_time() << std::endl;
    // std::cout << "\taverage_waiting_time=" << metrics.average_waiting_time() << std::endl;
    // std::cout << "\taverage_response_time=" << metrics.average_response_time() << std::endl;
    // std::cout << "\tthroughput=" << metrics.throughput() << std::endl;
    // std::cout << "\ttotal_revenue=" << metrics.total_revenue() << std::endl;
}

// double SchedulingPolicy::avg_turnaround() {
//     int turnaround = 0;
//     list<Customer> xs = processes;
//     while (!xs.empty()) {
//         Customer p = xs.front();
//         xs.pop_front();
//         turnaround += p.completion - p.arrival;
//     }
//     return (double)turnaround / (double)processes.size();
// }
