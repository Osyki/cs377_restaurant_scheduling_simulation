#ifndef RR_H
#define RR_H

#include "scheduling_policy.h"

class RR : public SchedulingPolicy
{
public:
    RR(const std::string &filename, const int &num_tables);
    ~RR(){};
    void run_policy();

private:
    pqueue_arrival xs;  // copy of job queue
    std::queue<Customer> ys; // queue of customers who have arrived
};

#endif