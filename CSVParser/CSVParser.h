#ifndef CSVPARSER_CSVPARSER_H
#define CSVPARSER_CSVPARSER_H

#include "../Print/print.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <memory>
#include <sstream>


template<typename... Types>
class CSVParser {
private:

    std::ifstream &file;
    char splitColumn;
    char splitRow;
    std::streampos fileBegin;

public:

    class Iterator;

    CSVParser(const CSVParser<Types...>& source) = delete;

    explicit CSVParser(std::ifstream &file, size_t rowStart = 0, char splitColumn = ',',
                       char splitLines = '\n')
            : file(file), splitColumn(splitColumn), splitRow(splitLines) {
        try {
            if (file.is_open()) {
                for (size_t i = 0; i < rowStart; i++) {
                    if (file.eof()) {
                        throw std::invalid_argument("Too big number offset.");
                    }
                    std::string skipBuffer;
                    std::getline(file, skipBuffer, splitLines);
                }
                fileBegin = file.tellg();
            }
            else {
                throw std::invalid_argument ("Can't open file.");
            }
        }
        catch (const std::exception &e) {
            e.what();
            exit(1);
        }
    }

    Iterator begin() {
        Iterator iterator(&file, fileBegin, splitColumn, splitRow);
        return iterator;
    }

    Iterator end() {
        Iterator iterator(nullptr, -1, splitColumn, splitRow);
        return iterator;
    }
};


template<typename... Types>
class CSVParser<Types...>::Iterator {
public:

    friend class CSVParser;

    typedef std::tuple<Types...> Tuple;
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_cv_t<Tuple>;
    using pointer = Tuple *;
    using reference = Tuple &;
    using iterator_type = Tuple;
    using iterator_category = std::input_iterator_tag;

    std::ifstream *file;
    char splitColumn;
    char splitRow;
    iterator_type value;
    std::streamoff rowPos;
private:


    template <typename... Args> struct typesList{};

    template<typename ...Args>
    std::tuple<Types...> makeTuple(typesList<Args...>, Types... types) {
        return std::make_tuple(types...);
    }


    template<typename ...Arg, typename Head, typename ...Args>
    std::tuple<Types...> makeTuple(typesList<Head, Args...>, Arg... arg) {
        try {
            if (file->eof()) {
                throw std::invalid_argument("File ended too early.");
            }

            std::istringstream column(getData());
            Head elem;
            if (column.fail()) {
                throw std::istringstream::failure("Fail column.");
            }
            column >> elem;

            return makeTuple(typesList<Args...>(), arg..., elem);
        }
        catch (const std::exception &e) {
            std::cout << e.what();
            exit(2);
        }
    }

    std::tuple<Types...> read() {
        return makeTuple(typesList<Types...>());
    }

    std::string getData() {
        char sym;
        std::string row;
        while (file && !(*file).eof() && (*file).get(sym) && sym != splitColumn && sym != splitRow) {
            row += sym;
        }
        return row;
    }



    Iterator(std::ifstream *file, std::streamoff rowPos, char splitColumn, char splitRow) : file(file),
    rowPos(rowPos), splitColumn(splitColumn), splitRow(splitRow) {
        if (file) {
            if (file->eof())
                this->file = nullptr;
            else
                value = read();
        }
    };

public:

    Iterator(const Iterator &it) : value(it.value), file(it.file), splitColumn(it.splitColumn),
    splitRow(it.splitRow),
    rowPos(it.rowPos) {

    };


    bool operator==(Iterator const &other) const {
        return file == other.file && rowPos == other.rowPos;
    }

    bool operator!=(Iterator const &other) const {
        return !(*this == other);
    }

    iterator_type operator*() const {
        return value;
    }

    Iterator &operator++() {
        if (file == nullptr) {
            return *this;
        }
        value = read();
        rowPos = (*file).tellg();
        if (file->eof()) {
            file = nullptr;
            rowPos = -1;
        }
        return *this;
    }

};

#endif //CSVPARSER_CSVPARSER_H
