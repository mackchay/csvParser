#ifndef CSVPARSER_PRINT_H
#define CSVPARSER_PRINT_H

#include <iostream>
#include <tuple>

template <size_t> struct int_{};

template<class Tuple, class Stream>
auto &print_tuple(Tuple &tup, int_<1>, Stream &os) {
    return os << std::get<std::tuple_size<Tuple>::value - 1>(tup);
}

template<class Tuple, class Stream, size_t pos>
auto &print_tuple(Tuple &tup, int_<pos>, Stream &os) {
    os << std::get<std::tuple_size<Tuple>::value - pos>(tup) << " ";
    return print_tuple(tup, int_<pos - 1>(), os);
}

template<class Ch, class Tr, class... Args>
auto &operator<<(std::basic_ostream<Ch, Tr>& os,
                std::tuple<Args...> tup) {
    return print_tuple(tup, int_<sizeof...(Args)>(), os);
}

class print {

};

#endif //CSVPARSER_PRINT_H
