//
// Created by scott on 2021/8/28.
//

#ifndef SYMBREG_DX_ROLLING_WINDOW_H
#define SYMBREG_DX_ROLLING_WINDOW_H

#include <numeric>
#include <complex>
#include <algorithm>
//#include <execution>
#include <iostream>
#include <vector>
#include <utility>

namespace el {

    template <typename InputIt>
    double stdev(InputIt begin,InputIt end){
        double mean = std::accumulate( begin, end, 0.0) / std::distance(begin,end);

        double sq_sum = std::inner_product(begin, end, begin, 0.0,
                                           [](double const & x, double const & y) { return x + y; },
                                           [mean](double const & x, double const & y) { return (x - mean)*(y - mean); });
//        return 0;
        return mean;
//        return sqrt(sq_sum);
        return std::distance(begin,end);
        return std::sqrt(sq_sum / std::distance(begin,end));
    }

    struct Stdev{
        template<typename InputIt>
        double operator ()(InputIt begin,InputIt end){
            return stdev(begin,end);
        }
    };

    struct Mean2{
        double sum = 0;
        template<typename InputIt>
        double operator ()(InputIt begin,InputIt end){

            double mean = 0 ;
            mean = std::accumulate(begin, end, 0.0) / std::distance(begin,end);
            return mean;
        }
    };


    struct Mean{
        double sum = 0;
        template<typename InputIt>
        double operator ()(InputIt begin,InputIt end){

            double mean = 0 ;
            mean = std::accumulate(begin, end, 0.0) / std::distance(begin,end);
            return mean;
        }
    };

    template< typename IMPL, typename InputIt,typename OutputIt>
    void rolling_window(InputIt begin,InputIt end, OutputIt output,std::uint32_t win_size ){
        auto it = begin;
        IMPL impl;
        while( it != end){
            auto  s = it;
            std::int32_t steps = -win_size + 1;
            if( std::abs(steps) > std::distance(begin,it) ){
                s = begin;
            }else {
                std::advance(s, steps);
            }
//            std::advance(s, steps);
            it = std::next(it);
            double result = impl(s,it);
//            double result = 1;
            *output = result;
            output = std::next(output);
        }

    }

    template<  typename InputIt,typename OutputIt>
    void _rolling_mean(InputIt begin,InputIt end, OutputIt output,std::uint32_t win_size ){
        auto it = begin;
        double  sum = 0;
        while( it != end){
            sum+= *it;
            auto size = std::distance(begin,it) %win_size;
            size +=1;
            if(std::distance(begin,it) >= win_size){
                size = win_size;
                sum -= *(it - win_size);
            }
            *output++ = sum / size;
            it++;
        }
    }

    template<  typename InputIt,typename OutputIt>
    void rolling_std(InputIt begin,InputIt end, OutputIt output,std::uint32_t win_size ){
        auto it = begin;
        double  sum = 0;
        double mean =0;
        while( it != end){
            sum+= *it;
            auto size = std::distance(begin,it) %win_size;
            size +=1;
            if(std::distance(begin,it) >= win_size){
                size = win_size;
                sum -= *(it - win_size);
            }
            mean = sum / size;
            *output++ = sum / size;
            it++;
        }
    }


//
//    double sq_sum = std::inner_product(begin, end, begin, 0.0,
//                                       [](double const & x, double const & y) { return x + y; },
//                                       [mean](double const & x, double const & y) { return (x - mean)*(y - mean); });
//
//    return std::sqrt(sq_sum / std::distance(begin,end));
//

    /***
  rolling_window_par()
  滚动并行计算
  IMPL : 统计函数对象
  split_size : 列记录分组大小( 并行计算)
  InputIt : （begin,end)
  OutputIt:  输出
  win_size： 滚动窗口大小

///////////////////////////////////////
     std::vector<int> l(100);
     std::vector<double> m(l.size());
     std::iota(l.begin(), l.end(), 0);
     rolling_window_par<Stdev,20>(l.begin(), l.end(), m.begin(), 3);
     std::for_each(m.begin(),m.end(),[](auto &n){
         std::cout<< n << std::endl;
     });
  ///////////////////////////////////////
*/

    template< typename IMPL,std::uint32_t split_size = 1000,typename InputIt,typename OutputIt>
    void _rolling_window_par(InputIt begin,InputIt end, OutputIt output,std::uint32_t win_size ){
        std::vector< std::pair<decltype(begin), decltype(begin) > > groups;
        decltype(begin) itr = begin;
        while(true) {
            if (itr + split_size >= end) {
                groups.push_back(std::make_pair(itr, end));
                break ;
            } else {
                groups.push_back(std::make_pair(itr, itr + split_size));
                itr = itr + split_size;
            }
        }
        std::for_each(groups.begin(),groups.end(),[&](auto & pair){
            auto _b = pair.first;
            decltype(begin) _e = pair.second;
            auto _output = std::next(output, std::distance(begin,_b) );
            auto it = _b;
            IMPL impl;
            while( it != _e){
                auto  s = it;
                std::int32_t steps = -win_size + 1;
                if( std::abs(steps) > std::distance(begin,it) ){
                    s = begin;
                }else {
                    std::advance(s, steps);
                }
                it = std::next(it);
                double result = impl(s,it);
                *_output = result;
                _output = std::next(_output);
            }
        });
    }

    template<  typename InputIt,typename OutputIt,typename IterOps>
    void _rolling_window(InputIt begin,InputIt end, OutputIt output,std::uint32_t win_size ,IterOps itr_fun){
        auto it = begin;

        while( it != end){
            auto  s = it;
            std::int32_t steps = -win_size + 1;
            if( std::abs(steps) > std::distance(begin,it) ){
                s = begin;
            }else {
                std::advance(s, steps);
            }
//            std::advance(s, steps);
            it = std::next(it);
            double result = itr_fun(s,it);
            *output = result;
            output = std::next(output);
        }

    }




        void rolling_mean(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size ){
            uint64_t begin = 0;
            uint64_t end = a->dimensions[0];
            uint64_t it = begin;
            float * W = 0;
            float  sum = 0;
            uint64_t  size = 0;
            uint64_t  p = 0;
            while( it != end){
                float *v = (float*)PyArray_GETPTR2(a, it,col);
                uint64_t  offset = ((char*)v) - a->data;
                sum+= *v;
                size = it - p + 1;
                if( it -begin >=  win_size){
                    size = win_size;
                    v = (float*)PyArray_GETPTR2(a, p,col);
                    sum -= *v;
                    p++;
                }else{
                    W = (float*)PyArray_GETPTR2(b, it,col);
                    W = (float*)(b->data + offset);
                    *W = NAN;// sum /(float)size;  // expensive write cost
                    it++;
                    continue;
                }
                W = (float*)PyArray_GETPTR2(b, it,col);
                *W =  sum /(float)size;  // expensive write cost
                it++;
            }
        }

}


#endif //SYMBREG_DX_ROLLING_WINDOW_H
