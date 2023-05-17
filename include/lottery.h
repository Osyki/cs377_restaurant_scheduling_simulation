#ifndef LOTTERY_H
#define LOTTERY_H

#include "scheduling_policy.h"

/**
 * Lottery scheduling policy.
 */
class Lottery : public SchedulingPolicy
{
public:
    Lottery(const std::string &filename, const int &num_tables);
    ~Lottery(){};
    void run_policy();

private:
    pqueue_arrival xs;        // copy of job queue
    std::vector<Customer> ys; // vector of customers who have arrived
};

#endif