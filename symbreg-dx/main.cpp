#include <iostream>
//#include "Table.h"
#include <algorithm>
//#include <execution>
#include <chrono>
//using namespace std::experimental::parallel;
void test_rolling();
void test_rolling_mean();

void read_parquet_file() ;

int test_c17_parall() {
    /*
    auto a = nc::arange<double>(0,100000000);
    auto b = nc::NdArray<double>(a.shape());
//    std::cout << "Hello, World!" << std::endl;
//    auto a = std::vector<int>(200000000);
//    auto b = std::vector<int>(200000000);
//    a.print();
//    b.print();
//    auto start = std::chrono::steady_clock::now();
    auto start = std::chrono::system_clock::now();

    std::transform( a.begin(),a.end(),b.begin(),
                   [](auto & x){ return x*2;});

    auto end = std::chrono::system_clock::now();
    std::cout << "1 - Elapsed time in milliseconds: "
    << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count()
    << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    std::transform(std::execution::par_unseq, a.begin(),a.end(),b.begin(),
                   [](auto & x){ return x*2;});

    std::transform(std::execution::par_unseq, a.begin(),a.end(),b.begin(),
                   [](auto & x){ return x*2;});
    end = std::chrono::system_clock::now();
    std::cout << "2 - Elapsed time in milliseconds: "
    << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count()
    << " ms" << std::endl;

//    b.print();
    return 0;
     */
}


#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include<boost/accumulators/framework/extractor.hpp>
#include <boost/fusion/functional.hpp>


using namespace boost::accumulators;

// https://www.boost.org/doc/libs/1_77_0/doc/html/accumulators/reference.html#statistics_library_reference
// http://www.bnikolic.co.uk/boostqf/accumulators.html
// https://www.boost.org/doc/libs/1_77_0/libs/numeric/ublas/doc/matrix_expression.html#1MatrixExpression

void test_par();

int test_numcpp(){
//    test_c17_parall();
//    test_rolling();
//    test_rolling_mean();
    test_par();
//    read_parquet_file();
//    test_rolling();
    return 0;
}

int main(int argc, char *argv[]) {
    test_numcpp();
    return 0;
}
