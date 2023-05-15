#ifndef SJF_H
#define SJF_H

#include "scheduling_policy.h"

class SJF : public SchedulingPolicy
{
public:
    SJF(const std::string &filename, const int &num_tables);
    void run_policy();

private:
    pqueue_arrival xs;  // copy of job queue
    pqueue_duration ys; // duration queue
};

#endif