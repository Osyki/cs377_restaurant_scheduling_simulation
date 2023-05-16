#include <list>
#include <queue>
#include <string>
#include <pthread.h>

using namespace std;

typedef struct Customer Customer;
struct Customer {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int willingness_to_wait;
  int revenue;
};

class ArrivalComparator {
 public:
  bool operator()(const Customer lhs, const Customer rhs) const {
    if (lhs.arrival != rhs.arrival)
      return lhs.arrival > rhs.arrival;
    else
      return lhs.duration > rhs.duration;
  }
};

class DurationComparator {
 public:
  bool operator()(const Customer lhs, const Customer rhs) const {
    if (lhs.duration != rhs.duration)
      return lhs.duration > rhs.duration;
    else
      return lhs.arrival > rhs.arrival;
  }
};

class RevenueComparator {
  public:
    bool operator()(const Customer lhs, const Customer rhs) const {
      if (lhs.revenue != rhs.revenue)
        return lhs.revenue < rhs.revenue;
      else
        return lhs.arrival > rhs.arrival;
    }
};

typedef priority_queue<Customer, vector<Customer>, ArrivalComparator>
    pqueue_arrival;
typedef priority_queue<Customer, vector<Customer>, DurationComparator>
    pqueue_duration;
typedef priority_queue<Customer, vector<Customer>, RevenueComparator>
    pqueue_revenue;

pqueue_arrival read_workload(const string& filename);
void show_workload(pqueue_arrival workload);
void show_processes(list<Customer> processes);

void fifo(const pqueue_arrival& workload);
list<Customer> sjf(const pqueue_arrival& workload);
list<Customer> stcf(const pqueue_arrival& workload);
list<Customer> rr(const pqueue_arrival& workload);

double avg_turnaround(const list<Customer>& processes);
double avg_response(const list<Customer>& processes);
void show_metrics(pqueue_arrival& workload);

list<Customer> completed_processes;
pqueue_arrival workload;
pthread_mutex_t completed_jobs = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t work_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;
int time_elapsed = 0;