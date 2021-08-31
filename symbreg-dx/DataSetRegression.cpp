
#include "DataSetRegression.hpp"

#include <stdexcept>

using namespace el;


DataSetRegression::DataSetRegression(const std::string& inName) :
		Component(inName)
{ }


void DataSetRegression::readCSV(std::istream& ioIS)
{
  clear();
  char lBuf[4096];
  for(ioIS.getline(lBuf,4096); ioIS.gcount()<4096; ioIS.getline(lBuf,4096)) {
    std::string lStr(lBuf);
    if(lStr.empty()) {
      if(ioIS.good()==false) break;
      else continue;
    }
    resize(size()+1);
    back().second.resize(0);
    std::istringstream lISS(lStr);
    double lDbl=0.0;
    for(lISS >> lDbl; (lISS.peek()==',') && lISS.good(); lISS >> lDbl) {
      const unsigned int lSize = back().second.size();
      back().second.resize(lSize + 1);
      back().second[lSize] = lDbl;
      lISS.get();
    }
    back().first = lDbl;
    if(ioIS.good()==false) break;
  }
  if(ioIS.gcount() == 4096) {
    std::ostringstream lOSS;
    lOSS << "Unable to read data set from given stream, number of characters on one ";
    lOSS << "line exceed 4096 characters!";
    throw std::runtime_error(lOSS.str());
  }
}


void DataSetRegression::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName()))
		throw Beagle_IOExceptionNodeM(*inIter, "invalid node type or tag name!");

	// Get size of data set
	unsigned int lCount = 0;
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType() == PACC::XML::eData) && (lChild->getValue() == "Data")) {
			++lCount;
		}
	}
	std::string lSizeString = inIter->getAttribute("size");
	if(lSizeString.empty() == false) {
	    if(lCount != Beagle::str2uint(lSizeString)) {
			std::ostringstream lOSS;
			lOSS << "number of 'Data' tags (" << lCount;
			lOSS << ") mismatch given value to 'size' attribute (" << lSizeString << ")!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}
	}
	resize(lCount);
	
	// Read data set
	lCount = 0;
	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if((lChild->getType() == PACC::XML::eData) && (lChild->getValue() == "Data")) {
			std::string lTargetAttr = lChild->getAttribute("target");
			if(lTargetAttr.empty()) {
				throw Beagle_IOExceptionNodeM(*lChild, "target attribute should be provided!");
			}
			(*this)[lCount].first = Beagle::str2dbl(lTargetAttr);
			PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
			if(lChild2->getType() != PACC::XML::eString) {
				throw Beagle_IOExceptionNodeM(*lChild2, "expected string content!");
			}
			(*this)[lCount].second.read(lChild2);
			++lCount;
		}
	}
	
	Beagle_StackTraceEndM("void DataSetRegression::readWithSystem(PACC::XML::ConstIterator, System&)");
}


void DataSetRegression::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.insertAttribute("size", Beagle::uint2str(size()));
	for(unsigned int i=0; i<size(); ++i) {
		ioStreamer.openTag("Data", false);
		ioStreamer.insertAttribute("target", Beagle::dbl2str((*this)[i].first));
		(*this)[i].second.write(ioStreamer, false);
		ioStreamer.closeTag();
	}
	Beagle_StackTraceEndM("void DataSetRegression::writeContent(PACC::XML::Streamer&, bool) const");
}
