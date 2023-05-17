#ifndef PRIORITY_QUEUES_H
#define PRIORITY_QUEUES_H

#include <queue>
#include "customer.h"

/**
 * Sorts customers by arrival time.
 */
class ArrivalComparator
{
public:
  bool operator()(const Customer lhs, const Customer rhs) const
  {
    if (lhs.arrival != rhs.arrival)
      return lhs.arrival > rhs.arrival;
    else
      return lhs.duration > rhs.duration;
  }
};

/**
 * Sorts customers by duration.
 */
class DurationComparator
{
public:
  bool operator()(const Customer lhs, const Customer rhs) const
  {
    if (lhs.duration != rhs.duration)
      return lhs.duration > rhs.duration;
    else
      return lhs.arrival > rhs.arrival;
  }
};

/**
 * Sorts customers by completion time.
 */
class CompletionComparator
{
public:
  bool operator()(const Customer lhs, const Customer rhs) const
  {
    if (lhs.completion != rhs.completion)
      return lhs.completion > rhs.completion;
    else
      return lhs.arrival > rhs.arrival;
  }
};

typedef std::priority_queue<Customer, std::vector<Customer>, ArrivalComparator>
    pqueue_arrival;
typedef std::priority_queue<Customer, std::vector<Customer>, DurationComparator>
    pqueue_duration;
typedef std::priority_queue<Customer, std::vector<Customer>, CompletionComparator>
    pqueue_completion;

#endif