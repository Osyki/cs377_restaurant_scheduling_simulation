#ifndef SCHEDULING_POLICY_H
#define SCHEDULING_POLICY_H

#include <list>
#include "customer.h"
#include "priority_queues.h"
#include "metrics.h"


class SchedulingPolicy {
    public:
        virtual void run_policy() = 0;
        void print_jobs();
        void print_metrics();

    private:
        Metrics metrics;
        pqueue_arrival arrival_queue;
        std::list<Customer> completed_jobs;

        // double avg_turnaround();
        // double avg_response();
};

#endif