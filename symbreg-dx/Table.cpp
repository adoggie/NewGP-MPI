//
// Created by scott on 2021/8/24.
//

#include "Table.h"


//using namespace el;

namespace  el{

    bool RawTable::isLess(const RawTable& rhs) const {
        auto ar = nc::less( data_ ,rhs.data() );
        auto ret = std::all_of(ar.begin() ,ar.end(),[](bool n){ return n;});
        return ret;
    }

    bool RawTable::isEqual(const RawTable& rhs) const {
        auto ret = nc::array_equal( data_ ,rhs.data() );
        return ret;
    }

    RawTable::Ptr RawTable::fromParquet(const std::string& parfile){
        return RawTable::Ptr();
    }

    RawTable::Ptr fromCSV(const std::string& csvfile){
        return RawTable::Ptr();
    }

    void RawTable::saveParquet(const std::string& parfile) const{

    }

    void  RawTable::cos(){
        data_ =nc::cos( data_ );
    }

    void  RawTable::sin(){
        data_ = nc::sin( data_ );
    }

    void  RawTable::exp(){
        data_ = nc::exp( data_ );
    }

    void  RawTable::log(){
        data_ = nc::log( data_ );
    }

    bool operator == (RawTable& lhs, const RawTable& rhs){
        return lhs.isEqual(rhs);
    }


    void operator += (RawTable& lhs, const RawTable& rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() + rhs.data();
    }


    void operator -= (RawTable& lhs, const RawTable& rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() - rhs.data();
    }


    void operator *= (RawTable& lhs, const RawTable& rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() * rhs.data();
    }


    void operator /= (RawTable& lhs, const RawTable& rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() / rhs.data();
    }


    RawTable operator - (RawTable const & lhs,  RawTable const& rhs){
        return lhs.data() - rhs.data();
    }


    RawTable operator * (const RawTable& lhs, const RawTable& rhs){
        return lhs.data() * rhs.data();
    }

    RawTable operator / (const RawTable& lhs, const RawTable& rhs){
        return lhs.data() / rhs.data();
    }


    void operator += (RawTable& lhs, double rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() + rhs;
    }
    void operator -= (RawTable& lhs, double rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() - rhs;
    }

    void operator *= (RawTable& lhs, double rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() * rhs;
    }

    void operator /= (RawTable& lhs, double rhs){
        const_cast< RawTable::DataType &>(lhs.data()) = lhs.data() / rhs;
    }

    RawTable operator - (const RawTable& lhs, double rhs){
        return lhs.data() - rhs;
    }

    RawTable operator * (const RawTable& lhs, double rhs){
        return lhs.data() * rhs;
    }

    RawTable operator / (RawTable const& lhs,  double rhs){
        return lhs.data() / rhs;
    }

    RawTable operator - (double lhs,const RawTable& rhs){
        return lhs - rhs.data() ;
    }

    RawTable operator * (double lhs,const RawTable& rhs){
        return lhs * rhs.data();
    }

    RawTable operator / (double lhs,const RawTable& rhs){
        return lhs / rhs.data() ;
    }
}

///////////////////////////////////////////
///// friend global functions

//std::ostringstream &  operator<<(std::ostringstream & oss, const el::Table& inRightDouble){
//    return oss;
//}
//
//std::istringstream& operator >>(std::istringstream & iss,  el::Table& inRightDouble)
//{
//    return iss;
//}


 bool operator<(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return inLeftDouble.isLess(inRightDouble);
    Beagle_StackTraceEndM("bool operator<(const Double& inLeftDouble, const Double& inRightDouble)");
}


 bool operator<=(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return ( inLeftDouble.isLess(inRightDouble) || inLeftDouble.isEqual(inRightDouble) );
    Beagle_StackTraceEndM("bool operator<=(const Double& inLeftDouble, const Double& inRightDouble)");
}


 bool operator>(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return inRightDouble.isLess(inLeftDouble);
    Beagle_StackTraceEndM("bool operator>(const Double& inLeftDouble, const Double& inRightDouble)");
}

 bool operator>=(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return ( inRightDouble.isLess(inLeftDouble) || inLeftDouble.isEqual(inRightDouble) );
    Beagle_StackTraceEndM("bool operator>=(const Double& inLeftDouble, const Double& inRightDouble)");
}


 bool operator==(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return inLeftDouble.isEqual(inRightDouble);
    Beagle_StackTraceEndM("bool operator==(const Double& inLeftDouble, const Double& inRightDouble)");
}

 bool operator!=(const el::Table& inLeftDouble, const el::Table& inRightDouble)
{
    Beagle_StackTraceBeginM();
    return ( inLeftDouble.isEqual(inRightDouble) == false);
    Beagle_StackTraceEndM("bool operator!=(const Double& inLeftDouble, const Double& inRightDouble)");
}


//
//bool operator==(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() == inRightDouble;
//    Beagle_StackTraceEndM("bool operator==(const Double& inLeftDouble, double inRightDouble)");
//}
//
//bool operator!=(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() != inRightDouble;
//    Beagle_StackTraceEndM("bool operator!=(const Double& inLeftDouble, double inRightDouble)");
//}

//bool operator<(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() < inRightDouble;
//    Beagle_StackTraceEndM("ool operator<(const Double& inLeftDouble, double inRightDouble)");
//}
//


//bool operator<=(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() <= inRightDouble;
//    Beagle_StackTraceEndM("bool operator<=(const Double& inLeftDouble, double inRightDouble)");
//}
//
//
//
//bool operator>(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() > inRightDouble;
//    Beagle_StackTraceEndM("bool operator>(const Double& inLeftDouble, double inRightDouble)");
//}


//bool operator>=(const el::Table& inLeftDouble, double inRightDouble)
//{
//    Beagle_StackTraceBeginM();
//    return inLeftDouble.getWrappedValue() >= inRightDouble;
//    Beagle_StackTraceEndM("bool operator>=(const Double& inLeftDouble, double inRightDouble)");
//}



el::Table operator+(const el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() + inRightDouble.getWrappedValue();
    Beagle_StackTraceEndM("Double operator+(const Double& inLeftDouble, const Double& inRightDouble)");
}


el::Table operator+(const el::Table& inLeftDouble, double inRightDouble)
{
    Beagle_StackTraceBeginM();
    auto rs = const_cast<el::RawTable& >(inLeftDouble.getWrappedValue()).data();
    auto raw = nc::add( rs , inRightDouble );
    return el::Table(raw);

    Beagle_StackTraceEndM("Double operator+(const Double& inLeftDouble, double inRightDouble)");
}


void operator+=(el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() += inRightDouble.getWrappedValue();
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator+=(Double& inLeftDouble, const Double& inRightDouble)");
}


void operator+=(el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() += inRightDouble;
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator+=(Double& inLeftDouble, double inRightDouble)");
}


el::Table operator-(const el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() - inRightDouble.getWrappedValue();
    Beagle_StackTraceEndM("Double operator-(const Double& inLeftDouble, const Double& inRightDouble)");
}

el::Table operator-(const el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() - inRightDouble;
    Beagle_StackTraceEndM("Double operator-(const Double& inLeftDouble, double inRightDouble)");
}

el::Table operator-=(el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() -= inRightDouble.getWrappedValue();
    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator-=(Double& inLeftDouble, const Double& inRightDouble)")
}


void operator-=(el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() -= inRightDouble;
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator-=(Double& inLeftDouble, double inRightDouble)");
}

el::Table operator*(const el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() * inRightDouble.getWrappedValue();
    Beagle_StackTraceEndM("Double operator*(const Double& inLeftDouble, const Double& inRightDouble)");
}

el::Table operator*(const el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() * inRightDouble;
    Beagle_StackTraceEndM("Double operator*(const Double& inLeftDouble, double inRightDouble)");
}

el::Table operator*=(el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() *= inRightDouble.getWrappedValue();
    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator*=(Double& inLeftDouble, const Double& inRightDouble)");
}

void operator*=(el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() *= inRightDouble;
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator*=(Double& inLeftDouble, double inRightDouble)");
}

el::Table operator/(const el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() / inRightDouble.getWrappedValue();
    Beagle_StackTraceEndM("Double operator/(const Double& inLeftDouble, const Double& inRightDouble)");
}

el::Table operator/(const el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    return inLeftDouble.getWrappedValue() / inRightDouble;
    Beagle_StackTraceEndM("Double operator/(const Double& inLeftDouble, double inRightDouble)");
}

void operator/=(el::Table& inLeftDouble, const el::Table& inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() /= inRightDouble.getWrappedValue();
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator/=(Double& inLeftDouble, const Double& inRightDouble)");
}


void operator/=(el::Table& inLeftDouble, double inRightDouble){
    Beagle_StackTraceBeginM();
    inLeftDouble.getWrappedValue() /= inRightDouble;
//    return inLeftDouble;
    Beagle_StackTraceEndM("Double operator/=(Double& inLeftDouble, double inRightDouble)");
}
