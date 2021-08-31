//
// Created by scott on 2021/8/24.
//

#ifndef SYMBREG_DX_Table_H
#define SYMBREG_DX_Table_H

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Exception.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/GP.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "NumCpp.hpp"

namespace el{

class RawTable :public Beagle::GP::Datum {

public:
    RawTable(){}
    RawTable(const nc::Shape& shape){ data_ = nc::zeros<double>(shape.rows,shape.cols);}
    using Ptr =  std::shared_ptr< RawTable >;
    using DataType = nc::NdArray<double>  ;

    RawTable(const DataType& data){ data_ = data;}
public:
    bool isLess(const RawTable& rhs) const ;
    bool isEqual(const RawTable& rhs) const ;

    static Ptr fromParquet(const std::string& parfile);
    static Ptr fromCSV(const std::string& csvfile);

    void saveParquet(const std::string& parfile) const;
    void  cos();
    void  sin();
    void  exp();
    void  log();

    const DataType& data() const  { return data_; }

private:
    DataType  data_;
};

//typedef nc::NdArray<double> RawTable;
typedef Beagle::WrapperT<RawTable> Table;

//typedef RawTable Table;

//typedef Beagle::GP::AddT<Table> Add;
//typedef Beagle::GP::SubtractT<Table> Subtract;
//typedef Beagle::GP::MultiplyT<Table> Multiply;
//typedef Beagle::GP::DivideT<Table> Divide;


//typedef Beagle::GP::Sin Sin;
//typedef Beagle::GP::Cos Cos;
//typedef Beagle::GP::Exp Exp;
//typedef Beagle::GP::Log Log;

//
bool operator == (RawTable& lhs, const RawTable& rhs);
void operator += (RawTable& lhs, const RawTable& rhs);
void operator -= (RawTable& lhs, const RawTable& rhs);
void operator *= (RawTable& lhs, const RawTable& rhs);
void operator /= (RawTable& lhs, const RawTable& rhs);
RawTable operator - (const RawTable& lhs, const RawTable& rhs);
RawTable operator * (const RawTable& lhs, const RawTable& rhs);
RawTable operator / (RawTable const& lhs,  RawTable const& rhs);


//bool operator == (RawTable& lhs, const double& rhs);
void operator += (RawTable& lhs, double rhs);
void operator -= (RawTable& lhs, double rhs);
void operator *= (RawTable& lhs, double rhs);
void operator /= (RawTable& lhs, double rhs);
RawTable operator - (const RawTable& lhs, double rhs);
RawTable operator * (const RawTable& lhs, double rhs);
RawTable operator / (RawTable const& lhs,  double rhs);

RawTable operator - (double lhs,const RawTable& rhs);
RawTable operator * (double lhs,const RawTable& rhs);
RawTable operator / (double lhs,const RawTable& rhs);
}


//////////////////////////////////////////////////
//\li bool operator<(const T&, const T&); to compare strict weak ordering two wrapped values.
//*  \li bool operator==(const T&, const T&); to compare equality of two wrapped values.
//*  \li std::ostream& operator<<(std::ostream&, const T&); to output value wrapped.
//*  \li std::istream& operator>>(std::istream&, T&); to input value wrapped.

//std::ostringstream &  operator<<(std::ostringstream & oss, const el::Table& inRightDouble);
//void operator >>(std::istringstream & iss, const el::Table& inRightDouble);
inline
std::ostringstream &  operator<<(std::ostringstream & oss, const el::Table& inRightDouble){
    return oss;
}
inline
std::istringstream& operator >>(std::istringstream & iss,  el::Table& inRightDouble){
    return iss;
}

//inline
//std::ostringstream &  operator<<(std::ostringstream & oss, const el::Table& inRightDouble){
//    return oss;
//}
inline
std::istringstream& operator >>(std::istringstream & iss, const el::Table& inRightDouble){
    return iss;
}

bool operator<(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator<=(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator>(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator>=(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator==(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator!=(const el::Table& inLeftDouble, const el::Table& inRightDouble);
bool operator==(const el::Table& inLeftDouble, double inRightDouble);
bool operator!=(const el::Table& inLeftDouble, double inRightDouble);
bool operator<(const el::Table& inLeftDouble, double inRightDouble);
bool operator<=(const el::Table& inLeftDouble, double inRightDouble);
bool operator>(const el::Table& inLeftDouble, double inRightDouble);
bool operator>=(const el::Table& inLeftDouble, double inRightDouble);
el::Table operator+(const el::Table& inLeftDouble, const el::Table& inRightDouble);
el::Table operator+(const el::Table& inLeftDouble, double inRightDouble);
void operator+=(el::Table& inLeftDouble, const el::Table& inRightDouble);
void operator+=(el::Table& inLeftDouble, double inRightDouble);
el::Table operator-(const el::Table& inLeftDouble, const el::Table& inRightDouble);
el::Table operator-(const el::Table& inLeftDouble, double inRightDouble);
el::Table operator-=(el::Table& inLeftDouble, const el::Table& inRightDouble);
void operator-=(el::Table& inLeftDouble, double inRightDouble);
el::Table operator*(const el::Table& inLeftDouble, const el::Table& inRightDouble);
el::Table operator*(const el::Table& inLeftDouble, double inRightDouble);
el::Table operator*=(el::Table& inLeftDouble, const el::Table& inRightDouble);
void operator*=(el::Table& inLeftDouble, double inRightDouble);
el::Table operator/(const el::Table& inLeftDouble, const el::Table& inRightDouble);
el::Table operator/(const el::Table& inLeftDouble, double inRightDouble);
void operator/=(el::Table& inLeftDouble, const el::Table& inRightDouble);
void operator/=(el::Table& inLeftDouble, double inRightDouble);


/**
 https://dpilger26.github.io/NumCpp/doxygen/html/md__c___github__num_cpp_docs_markdown__building.html
 */

#endif //SYMBREG_DX_Table_H


