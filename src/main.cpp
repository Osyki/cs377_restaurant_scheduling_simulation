#include "color.h"
#include "fifo.h"
#include "sjf.h"
#include "stcf.h"
#include "rr.h"
#include "lottery.h"

void print_menu(const std::string &workload_filename, const int &num_tables)
{
    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << blue 
              << "***********************************************" << std::endl;
    std::cout << "*     Welcome to the restaurant simulator     *" << std::endl;
    std::cout << "***********************************************" << std::endl;
    std::cout << " --------------------------------------------- " << std::endl;
    std::cout << "|      Please select a scheduling policy      |" << std::endl;
    std::cout << " --------------------------------------------- " << std::endl;
    std::cout << "|" << green 
              << " 1. First In First Out (FIFO)                " 
              << blue << "|" << std::endl;
    std::cout << "|" << green 
              << " 2. Shortest Job First (SJF)                 " 
              << blue << "|" << std::endl;
    std::cout << "|" << green 
              << " 3. Shortest Time To Completion First (STCF) " 
              << blue << "|" << std::endl;
    std::cout << "|" << green 
              << " 4. Round Robin (RR)                         " 
              << blue << "|" << std::endl;
    std::cout << "|" << green 
              << " 5. Lottery                                  " 
              << blue << "|" << std::endl;
    std::cout << "|" << red 
              << " 6. Multi-Level Feedback Queue (MLFQ)        " 
              << blue << "|" << std::endl;
    std::cout << "|" << red 
              << " 7. All of the above                         " 
              << blue << "|" << std::endl;
    std::cout << "|" << green 
              << " 8. Quit                                     " 
              << blue << "|" << std::endl;
    std::cout << " ---------------------------------------------\n"
              << std::endl;
    std::cout << def;
    std::cout << "Please enter your choice: ";

    int choice;
    std::cin >> choice;
    std::cout << "\nSelected ";
    switch (choice)
    {
    case 1:
    {
        std::cout << "FIFO\n"
                  << std::endl;
        FIFO fifo(workload_filename, num_tables); // threads start running here
        fifo.print_metrics();
        break;
    }
    case 2:
    {
        std::cout << "SJF\n"
                  << std::endl;
        SJF sjf(workload_filename, num_tables); // threads start running here
        sjf.print_metrics();
        break;
    }
    case 3: // STCF
    {
        std::cout << "STCF\n"
                  << std::endl;
        STCF stcf(workload_filename, num_tables); // threads start running here
        stcf.print_metrics();
        break;
    }
    case 4:
    {
        std::cout << "RR\n"
                  << std::endl;
        RR rr(workload_filename, num_tables); // threads start running here
        rr.print_metrics();
        break;
    }
    case 5:
    {
        std::cout << "Lottery\n"
                  << std::endl;
        Lottery lottery(workload_filename, num_tables); // threads start running here
        lottery.print_metrics();
        break;
    }
    // case 6:
    //     std::cout<<"MLFQ"<<std::endl;
    //     // break;
    // case 7:
    //     std::cout<<"All of the above"<<std::endl;
    //     // break;
    case 8:
    {
        std::cout << "Quit" << std::endl;
        break;
    }
    default:
        std::cout << "Invalid choice" << std::endl;
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./restaurant_simulation <workload file> <num_tables>" << std::endl;
        return 1;
    }

    const std::string workload_filename = argv[1];
    const int num_tables = atoi(argv[2]);

    print_menu(workload_filename, num_tables);

    return 0;
}