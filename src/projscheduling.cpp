#include "scheduling.h"
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

pqueue_arrival read_workload(const string& filename) {
    pqueue_arrival workload;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Customer p;
            char *token = strtok(&line[0], " ");
            p.arrival = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.duration = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.willingness_to_wait = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.revenue = (int)strtol(token, nullptr, 10);
            p.first_run = -1;
            p.completion = -1;
            // cout << "Read job: " << p.arrival << ' ' << p.duration << ' '
            //      << p.willingness_to_wait << ' ' << p.revenue << endl;
            workload.push(p);
        }
        file.close();
    } else {
        cout << "Error: Unable to open file: " << filename << endl;
    }
    return workload;
}

void print_workload(pqueue_arrival workload) {
    pqueue_arrival xs = workload;
    cout << "Workload:" << endl;
    while (!xs.empty()) {
        Customer p = xs.top();
        cout << '\t' << p.arrival << ' ' << p.duration << " " << p.willingness_to_wait << " " << p.revenue << endl;
        xs.pop();
    }
}

void show_workload(pqueue_arrival workload) {
    pqueue_arrival xs = workload;
    cout << "Workload:" << endl;
    while (!xs.empty()) {
        Customer p = xs.top();
        cout << '\t' << p.arrival << ' ' << p.duration << " " << p.willingness_to_wait << " " << p.revenue << endl;
        xs.pop();
    }
}

void show_processes(list<Customer> processes) {
    list<Customer> xs = processes;
    cout << "Processes:" << endl;
    while (!xs.empty()) {
        Customer p = xs.front();
        cout << "\tarrival=" << p.arrival
             << ", duration=" << p.duration
             << ", willingness_to_wait=" << p.willingness_to_wait
             << ", revenue=" << p.revenue
             << ", first_run=" << p.first_run
             << ", completion=" << p.completion << endl;
        xs.pop_front();
    }
}

/**
 * First Come First Serve (FCFS) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
void fifo(const pqueue_arrival& workload) {
    // list<Process> complete;
    // print beginning fifo on thread id
    cout << "beginning fifo on " << pthread_self() << endl;
    pqueue_arrival xs = workload;
    // lock thread
    pthread_mutex_lock(&work_mutex);
    while (!xs.empty()) {
        // continue popping customers until the current time is less than their arrival time + willingness to wait
        while (!xs.empty() && time_elapsed > xs.top().arrival + xs.top().willingness_to_wait) {
            cout << "Dropping customer " << xs.top().arrival << " at time " << time_elapsed << endl;
            xs.pop();
        }
        // if the queue is empty, break out of the loop
        if (xs.empty()) {
            break;
        }
        // pop the next customer
        Customer p = xs.top();
        xs.pop();
        pthread_mutex_unlock(&work_mutex); // unlock the thread
        // In case of arrival time in the future, wait until then
        pthread_mutex_lock(&time_mutex);
        if (time_elapsed < p.arrival) {
            time_elapsed = p.arrival;
        }
        p.first_run = time_elapsed;
        time_elapsed += p.duration;
        p.completion = time_elapsed;
        pthread_mutex_unlock(&time_mutex);
        pthread_mutex_lock(&completed_jobs); // lock the thread
        completed_processes.push_back(p);
        pthread_mutex_unlock(&completed_jobs); // unlock the thread
    }
}

/**
 * Shortest Job First (SJF) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
list<Customer> sjf(const pqueue_arrival& workload) {
    list<Customer> complete;
    pqueue_arrival xs = workload;
    pqueue_duration ys;
    int time = 0;
    while (!xs.empty() || !ys.empty()) {
        // add all processes that have arrived
        while (!xs.empty() && xs.top().arrival <= time) {
            ys.push(xs.top());
            xs.pop();
        }
        // if there are no processes to run, wait until the next one arrives
        if (ys.empty()) {
            time = xs.top().arrival;
        }
        // run the shortest process
        Customer p = ys.top();
        ys.pop();
        if (p.first_run == -1) {
            p.first_run = time;
        }
        time += p.duration;
        p.completion = time;
        complete.push_back(p);
    }
    return complete;
}

/**
 * Shortest Time to Completion (STCF) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
list<Customer> stcf(const pqueue_arrival& workload) {
    list<Customer> complete;
    pqueue_arrival xs = workload;
    pqueue_duration ys;
    int time = 0;
    while (!xs.empty() || !ys.empty()) {
        // add all processes that have arrived
        while (!xs.empty() && xs.top().arrival <= time) {
            ys.push(xs.top());
            xs.pop();
        }
        // if there are no processes to run, wait until the next one arrives
        if (ys.empty()) {
            time = xs.top().arrival;
        }
        // run the shortest process
        Customer p = ys.top();
        ys.pop();
        if (p.first_run == -1) {
            p.first_run = time;
        }
        time += 1;
        p.duration -= 1;
        if (p.duration == 0) {
            p.completion = time;
            complete.push_back(p);
        } else {
            ys.push(p);
        }
    }
    return complete;
}

/**
 * Round Robin (RR) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
list<Customer> rr(const pqueue_arrival& workload) {
    list<Customer> complete;
    pqueue_arrival xs = workload;
    queue<Customer> ys;
    int time = 0;
    while (!xs.empty() || !ys.empty()) {
        // add all processes that have arrived
        while (!xs.empty() && xs.top().arrival <= time) {
            Customer p = xs.top();
            ys.push(p);
            xs.pop();
        }
        // if there are no processes to run, wait until the next one arrives
        if (ys.empty()) {
            time = xs.top().arrival;
        }
        // run the next process
        Customer p = ys.front();
        ys.pop();
        if (p.first_run == -1) {
            p.first_run = time;
        }
        time += 1;
        p.duration -= 1;
        if (p.duration == 0) {
            p.completion = time;
            complete.push_back(p);
        } else {
            ys.push(p);
        }
    }
    return complete;
}

double avg_turnaround(const list<Customer>& processes) {
    int turnaround = 0;
    list<Customer> xs = processes;
    while (!xs.empty()) {
        Customer p = xs.front();
        xs.pop_front();
        turnaround += p.completion - p.arrival;
    }
    return (double)turnaround / (double)processes.size();
}

double avg_response(const list<Customer>& processes) {
    int response = 0;
    list<Customer> xs = processes;
    while (!xs.empty()) {
        Customer p = xs.front();
        xs.pop_front();
        response += p.first_run - p.arrival;
    }
    return (double)response / (double)processes.size();
}

int revenue(const list<Customer>& processes) {
    int revenue = 0;
    list<Customer> xs = processes;
    while (!xs.empty()) {
        Customer p = xs.front();
        xs.pop_front();
        revenue += p.revenue;
    }
    return revenue;
}

int total_revenue(const pqueue_arrival& workload) {
    int revenue = 0;
    pqueue_arrival xs = workload;
    while (!xs.empty()) {
        Customer p = xs.top();
        xs.pop();
        revenue += p.revenue;
    }
    return revenue;
}

void show_metrics(pqueue_arrival& workload) {
    list<Customer> processes = completed_processes;
    double avg_t = avg_turnaround(processes);
    double avg_r = avg_response(processes);
    int rev = revenue(processes);
    int total_rev = total_revenue(workload);
    show_processes(processes);
    cout << '\n';
    cout << "Average Turnaround Time: " << avg_t << endl;
    cout << "Average Response Time:   " << avg_r << endl;
    cout << "Revenue Earned:          " << rev << endl;
    cout << "Revenue Possible:        " << total_rev << endl;
    cout << "Percent Revenue Earned:  " << (double)rev / (double)total_rev * 100 << "%\n";
}
