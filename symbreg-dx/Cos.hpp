
#ifndef EL_Cos_hpp
#define EL_Cos_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
//#include "beagle/GP/Primitive.hpp"
#include "beagle/IndividualBag.hpp"

#include "beagle/WrapperT.hpp"
#include "beagle/Double.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/PrimitiveInline.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

//
namespace el
{

class Cos : public Beagle::GP::Primitive
{

public:

	//! GP::Cos allocator type.
	typedef Beagle::AllocatorT<Cos,Primitive::Alloc>
	Alloc;
	//! GP::Cos handle type.
	typedef Beagle::PointerT<Cos,Primitive::Handle>
	Handle;
	//! GP::Cos bag type.
	typedef Beagle::ContainerT<Cos,Primitive::Bag>
	Bag;

	explicit Cos(std::string inName="COS");
	virtual ~Cos()
	{ }

#ifdef BEAGLE_HAVE_RTTI
virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};


}


#endif //