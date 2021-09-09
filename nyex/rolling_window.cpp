//
// Created by scott on 2021/8/28.
//

#include "rolling_window.h"
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>

#include "NumCpp.hpp"

using namespace el;


void test_rolling(){
    std::vector<double> x = {1,2,3,4,5,6,7};
    std::vector<double> y(x.size());
    std::vector<double>::iterator  it;
//    rolling_window(x.begin(),x.end(),y.begin(),3,[](auto & n){ return n;});
    rolling_window<Stdev>(x.begin(),x.end(),y.begin(),3);
//    rolling_window<Mean>(x.begin(),x.end(),y.begin(),3);

    std::for_each(y.begin(),y.end(),[](auto &n){
       std::cout<< n << std::endl;
    });

    double xx = std::numeric_limits<double>::quiet_NaN();
    xx++;

    rolling_window(x.begin(),x.end(),y.begin(),3,
                   [](auto &x,auto &y){
        return (*x-*y)/(*x);
    });


}