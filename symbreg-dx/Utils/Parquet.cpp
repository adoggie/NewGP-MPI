//
// Created by scott on 2021/8/25.
//

#include "Parquet.h"
#include <iostream>


#include <arrow/io/file.h>
#include <arrow/util/logging.h>

#include <parquet/api/reader.h>
#include <parquet/api/writer.h>

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <algorithm>

#include "NumCpp.hpp"

#include "rolling_window.h"


#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
//#include <execution>

namespace ba = boost::accumulators;
namespace bt = ba::tag;
typedef ba::accumulator_set < double, ba::stats <bt::rolling_mean > > MeanAccumulator;

int test_accumulator() {

    MeanAccumulator acc(bt::rolling_window::window_size = 3);

//    for (uint32_t i : { 252, 189, 248, 154, 620, 885, 939, 196 }) {
    for (double i : { 1, 2,3,4,5,6,7 }) {
        acc(i);
        std::cout << i << " actualMean: " << std::fixed << boost::accumulators::rolling_mean(acc) << "\n";
    }
}


using parquet::LogicalType;
using parquet::Repetition;
using parquet::Type;
using parquet::schema::GroupNode;
using parquet::schema::PrimitiveNode;

constexpr int FIXED_LENGTH = 10;
constexpr int NUM_ROWS_PER_ROW_GROUP = 500;
const char PARQUET_FILENAME[] = "parquet_cpp_example.parquet";

/*
void test() {
    try {
// Create a ParquetReader instance
        std::unique_ptr<parquet::ParquetFileReader> parquet_reader =
                parquet::ParquetFileReader::OpenFile(PARQUET_FILENAME, false);

// Get the File MetaData
        std::shared_ptr<parquet::FileMetaData> file_metadata = parquet_reader->metadata();

// Get the number of RowGroups
        int num_row_groups = file_metadata->num_row_groups();
        assert(num_row_groups == 1);

// Get the number of Columns
        int num_columns = file_metadata->num_columns();
        assert(num_columns == 8);

// Iterate over all the RowGroups in the file
        for (int r = 0; r < num_row_groups; ++r) {
// Get the RowGroup Reader
            std::shared_ptr<parquet::RowGroupReader> row_group_reader =
                    parquet_reader->RowGroup(r);

            int64_t values_read = 0;
            int64_t rows_read = 0;
            int16_t definition_level;
            int16_t repetition_level;
            int i;
            std::shared_ptr<parquet::ColumnReader> column_reader;

// Get the Column Reader for the boolean column
            column_reader = row_group_reader->Column(0);
            parquet::BoolReader *bool_reader =
                    static_cast<parquet::BoolReader *>(column_reader.get());

// Read all the rows in the column
            i = 0;
            while (bool_reader->HasNext()) {
                bool value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = bool_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                bool expected_value = ((i % 2) == 0) ? true : false;
                assert(value == expected_value);
                i++;
            }

// Get the Column Reader for the Int32 column
            column_reader = row_group_reader->Column(1);

            parquet::Int32Reader *int32_reader =
                    static_cast<parquet::Int32Reader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (int32_reader->HasNext()) {
                int32_t value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = int32_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                assert(value == i);
                i++;
            }

// Get the Column Reader for the Int64 column
            column_reader = row_group_reader->Column(2);
            parquet::Int64Reader *int64_reader =
                    static_cast<parquet::Int64Reader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (int64_reader->HasNext()) {
                int64_t value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = int64_reader->ReadBatch(1, &definition_level, &repetition_level,
                                                    &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                int64_t expected_value = i * 1000 * 1000;
                expected_value *= 1000 * 1000;
                assert(value == expected_value);
                if ((i % 2) == 0) {
                    assert(repetition_level == 1);
                } else {
                    assert(repetition_level == 0);
                }
                i++;
            }

// Get the Column Reader for the Int96 column
            column_reader = row_group_reader->Column(3);
            parquet::Int96Reader *int96_reader =
                    static_cast<parquet::Int96Reader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (int96_reader->HasNext()) {
                parquet::Int96 value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = int96_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                parquet::Int96 expected_value;
                expected_value.value[0] = i;
                expected_value.value[1] = i + 1;
                expected_value.value[2] = i + 2;
                for (int j = 0; j < 3; j++) {
                    assert(value.value[j] == expected_value.value[j]);
                }
                i++;
            }

// Get the Column Reader for the Float column
            column_reader = row_group_reader->Column(4);
            parquet::FloatReader *float_reader =
                    static_cast<parquet::FloatReader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (float_reader->HasNext()) {
                float value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = float_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                float expected_value = static_cast<float>(i) * 1.1f;
                assert(value == expected_value);
                i++;
            }

// Get the Column Reader for the Double column
            column_reader = row_group_reader->Column(5);
            parquet::DoubleReader *double_reader =
                    static_cast<parquet::DoubleReader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (double_reader->HasNext()) {
                double value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = double_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                double expected_value = i * 1.1111111;
                assert(value == expected_value);
                i++;
            }

// Get the Column Reader for the ByteArray column
            column_reader = row_group_reader->Column(6);
            parquet::ByteArrayReader *ba_reader =
                    static_cast<parquet::ByteArrayReader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (ba_reader->HasNext()) {
                parquet::ByteArray value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read =
                        ba_reader->ReadBatch(1, &definition_level, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// Verify the value written
                char expected_value[FIXED_LENGTH] = "parquet";
                expected_value[7] = static_cast<char>('0' + i / 100);
                expected_value[8] = static_cast<char>('0' + (i / 10) % 10);
                expected_value[9] = static_cast<char>('0' + i % 10);
                if (i % 2 == 0) {  // only alternate values exist
// There are no NULL values in the rows written
                    assert(values_read == 1);
                    assert(value.len == FIXED_LENGTH);
                    assert(memcmp(value.ptr, &expected_value[0], FIXED_LENGTH) == 0);
                    assert(definition_level == 1);
                } else {
// There are NULL values in the rows written
                    assert(values_read == 0);
                    assert(definition_level == 0);
                }
                i++;
            }

// Get the Column Reader for the FixedLengthByteArray column
            column_reader = row_group_reader->Column(7);
            parquet::FixedLenByteArrayReader *flba_reader =
                    static_cast<parquet::FixedLenByteArrayReader *>(column_reader.get());
// Read all the rows in the column
            i = 0;
            while (flba_reader->HasNext()) {
                parquet::FixedLenByteArray value;
// Read one value at a time. The number of rows read is returned. values_read
// contains the number of non-null rows
                rows_read = flba_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
// Ensure only one value is read
                assert(rows_read == 1);
// There are no NULL values in the rows written
                assert(values_read == 1);
// Verify the value written
                char v = static_cast<char>(i);
                char expected_value[FIXED_LENGTH] = {v, v, v, v, v, v, v, v, v, v};
                assert(memcmp(value.ptr, &expected_value[0], FIXED_LENGTH) == 0);
                i++;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Parquet read error: " << e.what() << std::endl;
        //return -1;
    }

    std::cout << "Parquet Writing and Reading Complete" << std::endl;

}
*/


void _read_parquet_file() {

    std::cout << "Reading parquet-arrow-example.parquet at once" << std::endl;
//    std::shared_ptr<arrow::io::ReadableFile> infile;
//    PARQUET_THROW_NOT_OK(arrow::io::ReadableFile::Open(
//            "parquet-arrow-example.parquet", arrow::default_memory_pool(), &infile));

//    arrow::io::ReadableFile::Open("/home/scott/mnt/svr2/data/highprice.parquet");
    auto r = arrow::io::ReadableFile::Open("../highprice.parquet" ,arrow::default_memory_pool());

    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
            parquet::arrow::OpenFile(r.ValueOrDie(), arrow::default_memory_pool(), &reader));
//    parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader);
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));
    std::cout << "Loaded " << table->num_rows() << " rows in " << table->num_columns()
              << " columns." << std::endl;
//    arrow::io::ReadableFile::Open("abc");
    auto col = table->column(0);
    std::cout << col->length() << std::endl;
    auto one = col->chunk(0);
    auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);

    auto s = one->GetScalar(1);
    std::cout << s.ValueOrDie()->type->name()<< std::endl;

    std::cout<< arrow_double_array->values()->size() << std::endl;
//    for(auto n=0 ;n< arrow_double_array->length();n++){
//        std::cout<<arrow_double_array->Value(n) << std::endl;
//    }
//    return;
    double * v = (double*)(arrow_double_array->values()->data()+ arrow_double_array->values()->size() -8);
    auto ar = nc::asarray( (double*)arrow_double_array->values()->data(),arrow_double_array->length());
    std::cout << *v << std::endl;
    std::cout << ar.size() << std::endl;
//    ar.print();
//    test_accumulator();
    std::vector<double> means(ar.size());

//    for(auto e=0;e < 10;e++) {
//        MeanAccumulator acc(bt::rolling_window::window_size = 5);
//        std::transform(ar.begin(), ar.end(), means.begin(), [&acc](auto &i) {
//            acc(i);
//            return boost::accumulators::rolling_mean(acc);
//        });
//    }
    for(auto e=0;e < 150;e++)
    el::rolling_window<el::Stdev>(ar.begin(),ar.end(),means.begin(),100);

//    for(auto e=0;e < 50;e++) {
//        el::rolling_window(ar.begin(), ar.end(), means.begin(), 2,
//                           [](auto &begin, auto &end) {
//                               auto x = begin;
//                               auto y = std::next(begin);
//                               return (*x - *y) / (*x);
//                           });
//    }





//    }
}



void __attribute__((optimize("O0"))) read_parquet_file() {
//void __attribute__((optimize("O2"))) read_parquet_file() {

    std::cout << "Reading parquet-arrow-example.parquet at once" << std::endl;
    auto r = arrow::io::ReadableFile::Open("../highprice.parquet" ,arrow::default_memory_pool());
    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
            parquet::arrow::OpenFile(r.ValueOrDie(), arrow::default_memory_pool(), &reader));
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));
    std::cout << "Loaded " << table->num_rows() << " rows in " << table->num_columns()
              << " columns." << std::endl;

    std::vector<decltype(table->column(0)->chunk(0))> cols ;

    std::vector< nc::NdArray<double> > datas;
    for(auto cc=0;cc < table->num_columns();cc++){
        auto col = table->column(cc);
        auto one = col->chunk(0);
        cols.push_back(one);

        auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);
        auto ar = nc::asarray((double *) arrow_double_array->values()->data(),
                              arrow_double_array->length());
        datas.push_back(ar);
    }

    std::vector<double> means(table->num_rows());
//    std::vector<double> means(100000000);
//    std::iota(means.begin(), means.end(), 0);

    auto start = std::chrono::steady_clock::now();

    volatile double mean = 0;
    for(int n=0;n<1;n++) {
        mean = std::accumulate( means.begin(), means.end(), 0.0) ;// / std::distance(means.begin(), means.end());
        std::for_each( datas.begin(), datas.end(),
                      [&](auto &ar) {
//                          el::rolling_window<el::Stdev>(ar.begin(), ar.end(), means.begin(), 100);
                            el::rolling_mean(means.begin(), means.end(), means.begin(), 100);
                      });
        // boost 版本
//       for(auto e=0;e < 1;e++) {
//            MeanAccumulator acc(bt::rolling_window::window_size = 100);
//            std::transform(means.begin(), means.end(), means.begin(), [&acc](auto &i) {
//                acc(i);
//                return boost::accumulators::rolling_mean(acc);
//            });
//        }
       // 自己版本
//        el::rolling_window_mean(means.begin(), means.end(), means.begin(), 100);
    }

//    for(int n=0;n<1;n++) {
//        std::for_each(std::execution::par_unseq, cols.begin(), cols.end(),
//                      [](auto &one) {
//                          auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);
//                          auto ar = nc::asarray((double *) arrow_double_array->values()->data(),
//                                                arrow_double_array->length());
//                          std::vector<double> means(ar.size());
//                          el::rolling_window<el::Stdev>(ar.begin(), ar.end(), means.begin(), 100);
////          el::rolling_window_par<el::Stdev,1000>(ar.begin(),ar.end(),means.begin(),100);
//                      });
//    }


    auto end = std::chrono::steady_clock::now();
    std::cout << "1 - Elapsed time in milliseconds: "
              << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
    std::cout << "read columns okay!" << std::endl;
    std::cout << mean << std::endl;


//    for(auto one: cols){
//        auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);
//        auto ar = nc::asarray( (double*)arrow_double_array->values()->data(),arrow_double_array->length());
//        std::vector<double> means(ar.size());
//        el::rolling_window<el::Mean>(ar.begin(),ar.end(),means.begin(),100);
//    }


}




void test_rolling_mean() {
    std::cout << "Reading parquet at once" << std::endl;
    auto r = arrow::io::ReadableFile::Open("../highprice.parquet" ,arrow::default_memory_pool());
    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
            parquet::arrow::OpenFile(r.ValueOrDie(), arrow::default_memory_pool(), &reader));
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));
    std::cout << "Loaded " << table->num_rows() << " rows in " << table->num_columns()
              << " columns." << std::endl;

    std::vector<decltype(table->column(0)->chunk(0))> cols ;

    std::vector< nc::NdArray<double> > datas;
    for(auto cc=0;cc < table->num_columns();cc++){
        auto col = table->column(cc);
        auto one = col->chunk(0);
        cols.push_back(one);

        auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);
        auto ar = nc::asarray((double *) arrow_double_array->values()->data(),
                              arrow_double_array->length());
        datas.push_back(ar);
    }

    std::vector<double> means(table->num_rows());

    auto start = std::chrono::steady_clock::now();

    for(int n=0;n<10;n++) {
        std::for_each( datas.begin(), datas.end(),
                       [&](auto &ar) {
                           el::rolling_mean(ar.begin(), ar.end(), means.begin(), 100);
                       });
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "1 - Elapsed time in milliseconds: "
              << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
    std::cout << "read columns okay!" << std::endl;

}

void test_sqrt() {
    std::cout << "test_sqrt .." << std::endl;
    auto r = arrow::io::ReadableFile::Open("../highprice.parquet" ,arrow::default_memory_pool());
    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
            parquet::arrow::OpenFile(r.ValueOrDie(), arrow::default_memory_pool(), &reader));
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));
    std::cout << "Loaded " << table->num_rows() << " rows in " << table->num_columns()
              << " columns." << std::endl;

    std::vector<decltype(table->column(0)->chunk(0))> cols ;

    std::vector< nc::NdArray<double> > datas;
    for(auto cc=0;cc < table->num_columns();cc++){
        auto col = table->column(cc);
        auto one = col->chunk(0);
        cols.push_back(one);

        auto arrow_double_array = std::static_pointer_cast<arrow::DoubleArray>(one);
        auto ar = nc::asarray((double *) arrow_double_array->values()->data(),
                              arrow_double_array->length());
        datas.push_back(ar);
    }

    std::vector<double> means(table->num_rows());

    auto start = std::chrono::steady_clock::now();

    for(int n=0;n<10;n++) {
        std::for_each( datas.begin(), datas.end(),
                       [&](auto &ar) {
//                           el::rolling_window_mean(means.begin(), means.end(), means.begin(), 100);
                           std::for_each(ar.begin(),ar.end(),[](auto & v){
//                               std::sqrt( v);
                               std::pow(v,0.5);
                           });
                       });
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "1 - Elapsed time in milliseconds: "
              << std::chrono::duration_cast< std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
    std::cout << "read columns okay!" << std::endl;

}

#define TRACE_PRINT( sq , sp) \
    std::for_each(sq.begin(),sq.end(),[](auto & x){ std::cout << x << sp ;}); \
    std::cout << std::endl;

void test_rolling_mean_ok(){
    std::vector<double> means(100);
    std::vector<double> result(means.size(),0);
    std::iota(means.begin(), means.end(), 0);
//    TRACE_PRINT(means,",");
    double mean = 0;
    el::rolling_mean(means.begin(), means.end(), result.begin(), 5);
    TRACE_PRINT(result,",");

    std::cout << "----- TRY boost::rolling_mean ----------" << std::endl;
    std::fill(result.begin(),result.end(),0);
    MeanAccumulator acc(bt::rolling_window::window_size = 5);
    std::transform(means.begin(), means.end(), result.begin(), [&acc](auto &i) {
        acc(i);
        return boost::accumulators::rolling_mean(acc);
    });
    TRACE_PRINT(result,",");

}

void test_par(){
    //test_sqrt();
    test_rolling_mean_ok();
}

/*
 *Arrow::Tables
https://arrow.apache.org/docs/cpp/api/table.html#_CPPv4N5arrow16TableBatchReaderE


double stddev(std::vector<double> const & func)
{
    double mean = std::accumulate(func.begin(), func.end(), 0.0) / func.size();
    double sq_sum = std::inner_product(func.begin(), func.end(), func.begin(), 0.0,
        [](double const & x, double const & y) { return x + y; },
        [mean](double const & x, double const & y) { return (x - mean)*(y - mean); });
    return std::sqrt(sq_sum / func.size());
}

 */