#include "fifo.h"

void print_menu(const std::string &workload_filename,const int &num_tables){
    std::cout<<"Please select a scheduling policy:"<<std::endl;
    std::cout<<"1. First In First Out (FIFO)"<<std::endl;
    // std::cout<<"2. Shortest Job First (SJF)"<<std::endl;
    // std::cout<<"3. Shortest Remaining Time First (SRTF)"<<std::endl;
    // std::cout<<"4. Round Robin (RR)"<<std::endl;
    // std::cout<<"5. Lottery"<<std::endl;
    // std::cout<<"6. Multi-Level Feedback Queue (MLFQ)"<<std::endl;
    // std::cout<<"7. All of the above"<<std::endl;
    std::cout<<"8. Quit"<<std::endl;
    std::cout<<"Please enter your choice: ";

    int choice;
    std::cin>>choice;
    std::cout << "\nSelected ";
    switch(choice){
        case 1: {
            std::cout<<"FIFO"<<std::endl;
            FIFO fifo(workload_filename, num_tables); // threads start running here
            fifo.print_metrics();
            break;
        }
        // case 2:
        //     std::cout<<"SJF"<<std::endl;
        //     // break;
        // case 3:
        //     std::cout<<"SRTF"<<std::endl;
        //     // break;
        // case 4:
        //     std::cout<<"RR"<<std::endl;
        //     // break;
        // case 5:
        //     std::cout<<"Lottery"<<std::endl;
        //     // break;
        // case 6:
        //     std::cout<<"MLFQ"<<std::endl;
        //     // break;
        // case 7:
        //     std::cout<<"All of the above"<<std::endl;
        //     // break;
        // case 8:
        //     std::cout<<"Quit"<<std::endl;
        //     break;
        default:
            std::cout<<"Invalid choice"<<std::endl;
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: ./restaurant_simulation <workload file> <num_tables>" << std::endl;
        return 1;
    }

    const std::string workload_filename = argv[1];
    const int num_tables = atoi(argv[2]);

    std::cout << "Welcome to the restaurant simulator!\n" << std::endl;
    print_menu(workload_filename, num_tables);

    return 0;
}