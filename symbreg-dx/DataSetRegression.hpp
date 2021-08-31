

#ifndef EL_DataSetRegression_hpp
#define EL_DataSetRegression_hpp

#include "beagle/Beagle.hpp"
#include <vector>
#include <memory>
#include "Table.h"

namespace el {

class DataSetRegression : public Beagle::Component, public std::vector< std::pair<double,Beagle::Vector> >
{

public:

	//! DataSetRegression allocator type.
	typedef Beagle::AllocatorT< DataSetRegression, Beagle::Component::Alloc > Alloc;
	//!< DataSetRegression handle type.
	typedef Beagle::PointerT< DataSetRegression, Beagle::Component::Handle > Handle;
	//!< DataSetRegression bag type.
	typedef Beagle::ContainerT< DataSetRegression, Beagle::Component::Bag > Bag;

	explicit DataSetRegression(const std::string& inName=std::string("DataSetRegression"));
	virtual ~DataSetRegression()
	{ }

	void readCSV(std::istream& ioIS);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

public:
    std::vector< std::tuple<el::RawTable,el::RawTable> > & dataset() { return  dataset_;}

private:
	std::vector< std::tuple<el::RawTable,el::RawTable> > dataset_;
};

}

#endif
