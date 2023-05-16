#ifndef STCF_H
#define STCF_H

#include "scheduling_policy.h"

/**
 * Shortest time-to-completion first scheduling policy.
 */
class STCF : public SchedulingPolicy
{
public:
    STCF(const std::string &filename, const int &num_tables);
    ~STCF(){};
    void run_policy();

private:
    pqueue_arrival xs;  // copy of job queue
    pqueue_duration ys; // duration queue
};

#endif