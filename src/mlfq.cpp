#include "mlfq.h"
#include <unistd.h>

MLFQ::MLFQ(const std::string &filename, const int &num_tables)
{
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        this->threads.emplace_back(&MLFQ::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n"
              << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond);
}

void MLFQ::run_policy() {
    return;
}