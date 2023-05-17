# Restaurant Simulator
This is a scheduling app disguised as a restaurant simulator. It is a recreation of Project 3 with some key differences:

1. The entire project has been rebuilt to support classes, allowing for modularity and expansion of future algorithms.
2. Multi-threading support to simulate the number of tables available at a restaurant. 
3. A Lottery-based policy as an additional scheduling algorithm.
4. Workloads have been changed to contain the following on each line, with the file ending in a blank line: 
    - Customer arrival time 
    - Eating time
    - Time willing to wait for a table
    - Revenue generated 

##### Example Workload:
>0 5 10 20<br>
>1 3 5 10<br>
>2 2 5 10<br>
><br>

If a customer is not helped in time, they will leave the queue causing the restaurant to lose potential revenue. The purpose of this program is to see which scheduling policies are best for total revenue generated in a simulated environment. The following algorithms are currently supported:

- First In First Out (FIFO)
- Shortest Job First (SJF)
- Shortest Time to Completion First (STCF)
- Round Robin (RR)
- Lottery (LOT)

After each run, metrics are printed to the console with stats on jobs, average turnaround time, average response time, and revenues. Each algorithm can be run with a different number of tables to see how it affects the various stats. There are options to run the algorithms individually or all currently available algorithms.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. This program has only been tested on a Linux and MacOS environment.

### Prerequisites

- [CMake](https://cmake.org/download/)
    - CMake is used to build the project.

### Installing

1. Clone the repository
    ```
    git clone https://github.com/Osyki/cs377_restaurant_scheduling_simulation
    ```
2. Create a build directory
    ```
    mkdir build
    ```
3. Change to the build directory
    ```
    cd build
    ```
4. Run CMake
    ```
    cmake ..
    ```
5. Build the project
    ```
    make
    ```
6. Move back to the root directory
    ```
    cd ..
    ```
7. Run the program
    ```
    ./bin/restaurant_simulation <workload file> <num_tables>
    ```
- Example:
    ```
    ./bin/restaurant_simulation ./workloads/workload_01.txt 5
    ```

## Running tests

A test executable is generated when running the `make` command above. It runs all the tests given to us for Project 3 and ensures the algorithms are correct when running on a single thread. To run the tests, run the following command from the root directory:
```
./bin/restaurant_simulation_test
```

## Link to Presentation
- [Presentation](https://www.youtube.com/watch?v=v1r6in8FKbY)

## Authors

- **Jon Rubio** - [Osyki](https://github.com/Osyki)