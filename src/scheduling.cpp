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
            Process p;
            char *token = strtok(&line[0], " ");
            p.arrival = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.duration = (int)strtol(token, nullptr, 10);
            p.first_run = -1;
            p.completion = -1;
            workload.push(p);
        }
        file.close();
    } else {
        cout << "Error: Unable to open file: " << filename << endl;
//        exit(1);
    }
    return workload;
}

void show_workload(pqueue_arrival workload) {
    pqueue_arrival xs = workload;
    cout << "Workload:" << endl;
    while (!xs.empty()) {
        Process p = xs.top();
        cout << '\t' << p.arrival << ' ' << p.duration << endl;
        xs.pop();
    }
}

void show_processes(list<Process> processes) {
    list<Process> xs = processes;
    cout << "Processes:" << endl;
    while (!xs.empty()) {
        Process p = xs.front();
        cout << "\tarrival=" << p.arrival << ", duration=" << p.duration << ", first_run=" << p.first_run
             << ", completion=" << p.completion << endl;
        xs.pop_front();
    }
}

/**
 * First Come First Serve (FCFS) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
list<Process> fifo(const pqueue_arrival& workload) {
    list<Process> complete;
    pqueue_arrival xs = workload;
    int time = 0;
    while (!xs.empty()) {
        Process p = xs.top();
        xs.pop();
        // In case of arrival time in the future, wait until then
        if (time < p.arrival) {
            time = p.arrival;
        }
        p.first_run = time;
        time += p.duration;
        p.completion = time;
        complete.push_back(p);
    }
    return complete;
}

/**
 * Shortest Job First (SJF) Scheduling
 * @param workload Priority queue of processes sorted by arrival time
 * @return List of processes sorted by completion time, with metrics filled in
 */
list<Process> sjf(const pqueue_arrival& workload) {
    list<Process> complete;
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
        Process p = ys.top();
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
list<Process> stcf(const pqueue_arrival& workload) {
    list<Process> complete;
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
        Process p = ys.top();
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
list<Process> rr(const pqueue_arrival& workload) {
    list<Process> complete;
    pqueue_arrival xs = workload;
    queue<Process> ys;
    int time = 0;
    while (!xs.empty() || !ys.empty()) {
        // add all processes that have arrived
        while (!xs.empty() && xs.top().arrival <= time) {
            Process p = xs.top();
            ys.push(p);
            xs.pop();
        }
        // if there are no processes to run, wait until the next one arrives
        if (ys.empty()) {
            time = xs.top().arrival;
        }
        // run the next process
        Process p = ys.front();
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

float avg_turnaround(const list<Process>& processes) {
    int turnaround = 0;
    list<Process> xs = processes;
    while (!xs.empty()) {
        Process p = xs.front();
        xs.pop_front();
        turnaround += p.completion - p.arrival;
    }
    return (float)turnaround / (float)processes.size();
}

float avg_response(const list<Process>& processes) {
    int response = 0;
    list<Process> xs = processes;
    while (!xs.empty()) {
        Process p = xs.front();
        xs.pop_front();
        response += p.first_run - p.arrival;
    }
    return (float)response / (float)processes.size();
}

void show_metrics(const list<Process>& processes) {
    float avg_t = avg_turnaround(processes);
    float avg_r = avg_response(processes);
    show_processes(processes);
    cout << '\n';
    cout << "Average Turnaround Time: " << avg_t << endl;
    cout << "Average Response Time:   " << avg_r << endl;
}
