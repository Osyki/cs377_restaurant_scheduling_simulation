#include <iostream>
#include <fstream>
#include <vector>
#include "scheduling.cpp"
#include <thread>



// vector array of policies
const std::vector<std::string> policies = {"fifo", "sjf", "stcf", "rr", "revenue"};

// struct to store workload and policy
struct thread_data {
    pqueue_arrival workload;
    int policy;
};

void run_policy(thread_data thread_arg); // forward declaration

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
    // print_workload(workload);

    // match string to policy
    int policy = -1;
    for (int i = 0; i < (int) policies.size(); i++) {
        if (policies[i] == argv[3]) {
            policy = i;
            break;
        }
    }

    int table_count;
     try {
        table_count = std::stoi(argv[2]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument: " << argv[1] << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Argument out of range: " << argv[1] << std::endl;
        return 1;
    }

    // store workload and policy in struct
    struct thread_data workload_and_policy;
    workload_and_policy.workload = workload;
    workload_and_policy.policy = policy;
    
    cout << "Running policy: " << policies[policy] << endl;
    cout << "Number of tables: " << table_count << endl;
    show_metrics(workload);

    // create threads
    std::vector<std::thread> threads;
    for (int i = 0; i < (int)table_count; i++) {
        threads.emplace_back(run_policy, workload_and_policy);
    }
    // join pthreads
    for (std::thread& t : threads) {
        t.join();
    }

    // TODO: print statistics
    show_metrics(workload);

    return 0;
}

void run_policy(thread_data thread_arg) {
    struct thread_data my_data = thread_arg;
    int policy = my_data.policy;
    pqueue_arrival workload = my_data.workload;
    switch(policy) {
        case 0:
            fifo(workload);
            break;
        case 1:
            sjf(workload);
            break;
        case 2:
            stcf(workload);
            break;
        case 3:
            rr(workload);
            break;
        case 4:
            // TODO: implement revenue
            // revenue(workload);
            // break;
        default:
            std::cout << "Error: unknown policy" << std::endl;
            break;
    }
}
