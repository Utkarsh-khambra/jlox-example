#include <scanner.hpp>
int main(int argc , char** argv) {
  Scanner scanner;
    if(argc == 2)
        scanner.scan(argv[1]);
    if(argc == 1)
        scanner.run_prompt();
    return 0;
}
