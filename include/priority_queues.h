#ifndef PRIORITY_QUEUES_H
#define PRIORITY_QUEUES_H

#include <queue>
#include "customer.h"

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

typedef std::priority_queue<Customer, std::vector<Customer>, ArrivalComparator>
    pqueue_arrival;
typedef std::priority_queue<Customer, std::vector<Customer>, DurationComparator>
    pqueue_duration;
typedef std::priority_queue<Customer, std::vector<Customer>, RevenueComparator>
    pqueue_revenue;

#endif