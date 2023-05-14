#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <queue>
#include <vector>

struct Customer {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int willingness_to_wait;
  int revenue;
};

#endif