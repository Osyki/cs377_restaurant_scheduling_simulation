#include <list>
#include <queue>
#include <string>
#include <pthread.h>

using namespace std;

typedef struct Process Process;
struct Process {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int willingness_to_wait;
  int revenue;
};

class ArrivalComparator {
 public:
  bool operator()(const Process lhs, const Process rhs) const {
    if (lhs.arrival != rhs.arrival)
      return lhs.arrival > rhs.arrival;
    else
      return lhs.duration > rhs.duration;
  }
};

class DurationComparator {
 public:
  bool operator()(const Process lhs, const Process rhs) const {
    if (lhs.duration != rhs.duration)
      return lhs.duration > rhs.duration;
    else
      return lhs.arrival > rhs.arrival;
  }
};

class RevenueComparator {
  public:
    bool operator()(const Process lhs, const Process rhs) const {
      if (lhs.revenue != rhs.revenue)
        return lhs.revenue < rhs.revenue;
      else
        return lhs.arrival > rhs.arrival;
    }
};

typedef priority_queue<Process, vector<Process>, ArrivalComparator>
    pqueue_arrival;
typedef priority_queue<Process, vector<Process>, DurationComparator>
    pqueue_duration;
typedef priority_queue<Process, vector<Process>, RevenueComparator>
    pqueue_revenue;

pqueue_arrival read_workload(const string& filename);
void show_workload(pqueue_arrival workload);
void show_processes(list<Process> processes);

list<Process> fifo(const pqueue_arrival& workload);
list<Process> sjf(const pqueue_arrival& workload);
list<Process> stcf(const pqueue_arrival& workload);
list<Process> rr(const pqueue_arrival& workload);

float avg_turnaround(const list<Process>& processes);
float avg_response(const list<Process>& processes);
void show_metrics(const list<Process>& processes);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;