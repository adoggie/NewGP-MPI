#include <numeric>
#include <complex>
#include <algorithm>
#include <vector>
#include <list>
#include <utility>
#include <boost/circular_buffer.hpp>

inline double
calc_skew(int64_t minp, int64_t nobs,
          double x, double xx,
          double xxx) {

    double result, dnobs;
    double A, B, C, R;

    if (nobs >= minp) {
        dnobs = (double) nobs;
        A = x / dnobs;
        B = xx / dnobs - A * A;
        C = xxx / dnobs - A * A * A - 3 * A * B;

        if (B <= 1e-14 || nobs < 3) {
            result = NAN;
        } else {
            R = std::sqrt(B);
            result = ((sqrt(dnobs * (dnobs - 1.)) * C) /
                      ((dnobs - 2) * R * R * R));
        }
    } else {
        result = NAN;
    }

    return result;
}


inline void
add_skew(double val, int64_t *nobs,
                     double *x, double *xx,
                     double *xxx,
                     double *compensation_x,
                     double *compensation_xx,
                     double *compensation_xxx) {
    //""" add a value from the skew calc """

    double y, t;
    //# Not NaN
    //if notnan(val):
    nobs[0] = nobs[0] + 1;
    y = val - compensation_x[0];
    t = x[0] + y;
    compensation_x[0] = t - x[0] - y;
    x[0] = t;
    y = val * val - compensation_xx[0];
    t = xx[0] + y;
    compensation_xx[0] = t - xx[0] - y;
    xx[0] = t;
    y = val * val * val - compensation_xxx[0];
    t = xxx[0] + y;
    compensation_xxx[0] = t - xxx[0] - y;
    xxx[0] = t;
}

inline void
remove_skew(double val, int64_t *nobs,
            double *x, double *xx,
            double *xxx,
            double *compensation_x,
            double *compensation_xx,
            double *compensation_xxx) {
//""" remove a value from the skew calc """
//cdef:
    double y, t;

//# Not NaN
//if notnan(val):
    nobs[0] = nobs[0] - 1;
    y = -val - compensation_x[0];
    t = x[0] + y;
    compensation_x[0] = t - x[0] - y;
    x[0] = t;
    y = -val * val - compensation_xx[0];
    t = xx[0] + y;
    compensation_xx[0] = t - xx[0] - y;
    xx[0] = t;
    y = -val * val * val - compensation_xxx[0];
    t = xxx[0] + y;
    compensation_xxx[0] = t - xxx[0] - y;
    xxx[0] = t;
}



std::vector<double> roll_skew(std::vector<double> values,
                              std::vector<int64_t> start,
                              std::vector<int64_t> end,
                              int64_t minp) {

    int64_t i, j;
    double val, prev, min_val, mean_val, sum_val = 0;
    double compensation_xxx_add = 0, compensation_xxx_remove = 0;
    double compensation_xx_add = 0, compensation_xx_remove = 0;
    double compensation_x_add = 0, compensation_x_remove = 0;
    double x = 0, xx = 0, xxx = 0;
    int64_t nobs = 0, N = values.size(), nobs_mean = 0;
    int64_t s, e;
    std::vector<double> output(N), mean_array, values_copy(values);
    minp = fmax(minp, 3);

//is_monotonic_increasing_bounds = is_monotonic_increasing_start_end_bounds(
//        start, end
//)

//    output = np.empty(N, dtype=np.float64)
//min_val = np.nanmin(values)
    min_val = *std::min_element(values.begin(), values.end());
//    std::vector<double> values_copy(values);


//for i in range(0, N):
    for ( i = 0; i < N; i++) {
        val = values_copy[i];
//        if notnan(val):
        nobs_mean += 1;
        sum_val += val;
    }
    mean_val = sum_val / nobs_mean;
//# Other cases would lead to imprecision for smallest values
    if (min_val - mean_val > -1e5) {
        mean_val = std::round(mean_val);
        for ( i = 0; i < N; i++) {
            values_copy[i] = values_copy[i] - mean_val;
        }
    }

    for ( i = 0; i < N; i++) {
        s = start[i];
        e = end[i];

//# Over the first window, observations can only be added
//# never removed
//if i == 0 or not is_monotonic_increasing_bounds:
        if (i == 0) {
            for ( j = s; j < e; j++) {
                val = values_copy[j];
                add_skew(val, &nobs, &x, &xx, &xxx, &compensation_x_add,
                         &compensation_xx_add, &compensation_xxx_add);
            }
        } else {
            //# After the first window, observations can both be added
            //# and removed
            //# calculate deletes
            for ( j = start[i - 1]; j < s; j++) {
                val = values_copy[j];
                remove_skew(val, &nobs, &x, &xx, &xxx, &compensation_x_remove,
                            &compensation_xx_remove, &compensation_xxx_remove);
            }

            //# calculate adds
            //for j in range(end[i - 1], e):
            for ( j = end[i - 1]; j < e; j++) {
                val = values_copy[j];
                add_skew(val, &nobs, &x, &xx, &xxx, &compensation_x_add,
                         &compensation_xx_add, &compensation_xxx_add);
            }

        }
        output[i] = calc_skew(minp, nobs, x, xx, xxx);
    }

    return output;
}

int main(int ,char**){

    return 0;
}