#ifndef FIFO_H
#define FIFO_H

#include "scheduling_policy.h"

class FIFO : public SchedulingPolicy {
    public:
        FIFO(const std::string &filename, const int &num_tables);
        void run_policy();

    private:
        pqueue_arrival xs; // copy of job queue
};

#endif