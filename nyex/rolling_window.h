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
#include <list>
#include <utility>
#include <boost/circular_buffer.hpp>

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

    // https://www.johndcook.com/blog/standard_deviation/
    //        https://www.it-swarm.cn/zh/algorithm/%E6%BB%9A%E5%8A%A8%E6%96%B9%E5%B7%AE%E7%AE%97%E6%B3%95/971899514/
        class RunningStat
        {
            public:
            RunningStat(uint32_t win_size) : win_size_(win_size),
            windows_(win_size){

            }

            uint32_t n = 0;
            float mean = 0;
            float run_var = 0;
            uint32_t win_size_ = 0;


            boost::circular_buffer<float> windows_;

            void clear(){
                this->n = 0;
//                this.windows_.clear();
            }

            /*
             * Initialize M1 = x1 and S1 = 0.
                Mk = Mk-1+ (xk – Mk-1)/k
                Sk = Sk-1 + (xk – Mk-1)*(xk – Mk).
                For 2 ≤ k ≤ n, the kth estimate of the variance is s2 = Sk/(k – 1).
             */

            void push(float x) {
                float x_removed = this->windows_[0];
                this->windows_.push_back(x);

                if (this->n <= this->win_size_) {
                    //# Calculating first variance
                    this->n += 1;
                    float delta = x - this->mean;
                    this->mean += delta / this->n;
//                    this->run_var += delta * (x - this->mean);
                    this->run_var += delta * delta;
                } else {
//# Adjusting variance
//                float x_removed = self.windows.popleft()
                    float old_m = this->mean;
                    this->mean += (x - x_removed) / this->win_size_;
//                    this->run_var += (x + x_removed - old_m - this->mean) * (x - x_removed);

//                    accVar += (observation - prevMean) * (observation - mean) - (then - prevMean) * (then - mean);
                    this->run_var += (x - old_m) *(x - this->mean) - ( x_removed - old_m)*(x_removed - this->mean);
                }
            }

            float get_mean() {
                if(this->n){
                    return this->mean;
                }
                return 0;
            }

            float get_var() {
                if(this->n > 1){
                    return this->run_var / (this->win_size_ -1);
                }
                return 0;
            }

            float get_std() {
                return std::sqrt(this->get_var());
            }
        };

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

        void __rolling_mean(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size ){
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

        float stdev(PyArrayObject *a,uint64_t col,float mean,uint64_t begin,uint64_t end,int bias=1){
            float result = 0;
            double sum = 0;
            for(auto n=begin;n< end;n++){
                float v = *(float *) PyArray_GETPTR2(a, n, col);
                sum += (v - mean)*(v - mean);
            }
            result = std::sqrt( sum / (end - begin + bias) );
            return result ;
        }


        void rolling_stdev(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size,int bias ){
            uint64_t begin = 0;
            uint64_t end = a->dimensions[0];
            uint64_t it = begin;
            float * W = 0;


            RunningStat stat(win_size);
//            printf("rolling_stdev() col:%lu ,it: %lu , end:%lu- --------------------\n",col,it,end);
//            return ;
            for( ;it < end; it++){
                float v = *(float*)PyArray_GETPTR2(a, it,col);
                stat.push(v);

                if( it -begin >=  win_size){
                    ;
                }else{
                    W = (float*)PyArray_GETPTR2(b, it,col);
                    *W = NAN;// sum /(float)size;  // expensive write cost
                    continue;
                }

//                printf(">A. it: %lu ,end: %lu ,col:%lu \n",it,end,col);
                auto x = stat.get_std();
                W = (float*)PyArray_GETPTR2(b, it,col);
                *W = x;

            }
        }

        void __rolling_stdev(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size,int bias ){
            uint64_t begin = 0;
            uint64_t end = a->dimensions[0];
            uint64_t it = begin;
            float * W = 0;
            float  sum = 0;
            uint64_t  size = 0;
            uint64_t  p = 0;
            boost::circular_buffer<float> mid_vals(win_size);
            std::list<float> list_vals;
            while( it != end){
                float *v = (float*)PyArray_GETPTR2(a, it,col);
                uint64_t  offset = ((char*)v) - a->data;
                sum+= *v;
                size = it - p + 1;
                if( it -begin >=  win_size){
                    size = win_size;
                    v = (float*)PyArray_GETPTR2(a, p,col);

                    p++;
                }else{
                    W = (float*)PyArray_GETPTR2(b, it,col);
                    W = (float*)(b->data + offset);
                    *W = NAN;// sum /(float)size;  // expensive write cost
                    it++;
//                    mid_vals.push_back(*v);
                    continue;
                }
                W = (float*)PyArray_GETPTR2(b, it,col);
//                *W =  sum /(float)size;  // expensive write cost
                sum -= *v;
                float mean = sum /(float)size;
                float x = (*v  - mean) * (*v  - mean);
//                mid_vals.push_back(x);

//                if(list_vals.size()){
//                    list_vals.pop_front();
//                }
//                list_vals.emplace_back(x);

                double y = std::accumulate( mid_vals.begin(), mid_vals.end(), 0.0) / (mid_vals.size()+bias);
                float R = std::sqrt(x);

//                uint64_t s = it -win_size + 1;
//                uint64_t e = it + 1;
//                float R = stdev(a,col,mean,s,e,bias);

                *W = R;
                //-------------
                it++;
            }
        }
}


#endif //SYMBREG_DX_ROLLING_WINDOW_H
