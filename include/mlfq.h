#ifndef MLFQ_H
#define MLFQ_H

#include "scheduling_policy.h"

/**
 * Multi-level feedback queue scheduling policy.
 */
class MLFQ : public SchedulingPolicy
{
public:
    MLFQ(const std::string &workload_filename, const int &num_tables);
    ~MLFQ();

    void run_policy();

private:
    pqueue_arrival xs; // copy of job queue

};

#endif // MLFQ_H