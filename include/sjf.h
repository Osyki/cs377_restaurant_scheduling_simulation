#ifndef SJF_H
#define SJF_H

#include "scheduling_policy.h"

/**
 * Shortest job first scheduling policy.
 */
class SJF : public SchedulingPolicy
{
public:
    SJF(const std::string &filename, const int &num_tables);
    ~SJF(){};
    void run_policy();

private:
    pqueue_arrival xs;  // copy of job queue
    pqueue_duration ys; // duration queue
};

#endif