#include <fstream>
#include "Print/print.h"
#include "CSVParser/CSVParser.h"

int main() {
    std::ifstream file;
    file.open(R"(C:\Users\Public\ClionProjects_C++\CSVparser\gas.csv)");
    CSVParser<std::string, int, int, int, float, double, double, double, int> parser(file, 0, ';');
    CSVParser<std::string, int, int, int, float, double, double, double, int>::Iterator it = parser.begin();

    for (auto rs = parser.begin(); rs != parser.end(); rs += 14) {
        std::cout << *rs << std::endl;
    }

    for (auto rs : parser) {
       std::cout<<rs<<std::endl;
    }
    file.close();
    return 0;
}
