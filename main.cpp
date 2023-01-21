#include <fstream>
#include "Print/print.h"
#include "CSVParser/CSVParser.h"

int main() {
    std::ifstream file;
    file.open(R"(C:\Users\Public\ClionProjects_C++\CSVparser\test.csv)");

    CSVParser<int, int, char> parser(file, 0);
    for (auto rs : parser) {
        std::cout<<rs<<std::endl;
    }
    file.close();
    return 0;
}
