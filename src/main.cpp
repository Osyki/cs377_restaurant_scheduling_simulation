#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Welcome to the restaurant simulator!" << std::endl;
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input file> <number of tables available>" << std::endl;
        std::cout << "The input file should contain a list of numbers, 4 per line in the following format:" << std::endl;
        std::cout << "<time arrived> <time to finish eating> <time willing to wait for table> <revenue>" << std::endl;
        return 1;
        }
    return 0;
}
