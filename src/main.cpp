#include <iostream>
#include <fstream>
#include <vector>
#include <scheduling.cpp>

// vector array of policies
const std::vector<std::string> policies = {"fifo", "sjf", "stcf", "rr", "revenue"};

int main(int argc, char* argv[]) {
    std::cout << "Welcome to the restaurant simulator!" << std::endl;
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <input file> <number of tables available> <scheduling policy to use>" << std::endl;
        std::cout << "The available scheduling policies are: fifo, sjf, stcf, rr, and revenue" << std::endl;
        std::cout << "The input file should contain a list of numbers, 4 per line in the following format:" << std::endl;
        std::cout << "<time arrived> <time to finish eating> <time willing to wait for table> <revenue>" << std::endl;
        return 1;
    }
    // if table count is less than 0, return error
    if (atoi(argv[2]) < 0) {
        std::cout << "Error: number of tables must be greater than 0" << std::endl;
        return 1;
    }

    // parse file
    pqueue_arrival workload = read_workload(argv[1]);
    show_workload(workload);

    // parse table count
    int table_count = atoi(argv[2]);

    // TODO: create thread array and call scheduling function

    // match string to policy
    int policy = -1;
    for (int i = 0; i < policies.size(); i++) {
        if (policies[i] == argv[3]) {
            policy = i;
            break;
        }
    }

    // TODO: print statistics


    return 0;
}
