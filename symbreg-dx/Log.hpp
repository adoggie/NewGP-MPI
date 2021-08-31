
/*!
 *  \file   beagle/GP/Log.hpp
 *  \brief  Definition of the templated class GP::Log.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef EL_LOG_HPP
#define EL_LOG_HPP

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"

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


namespace el
{

class Log : public Beagle::GP::Primitive{

public:

	//! GP::Log allocator type.
	typedef Beagle::AllocatorT<Log,Beagle::GP::Primitive::Alloc>
	Alloc;
	//! GP::Log handle type.
	typedef Beagle::PointerT<Log,Beagle::GP::Primitive::Handle>
	Handle;
	//! GP::Log bag type.
	typedef Beagle::ContainerT<Log,Beagle::GP::Primitive::Bag>
	Bag;

	explicit Log(std::string inName="LOG");
	virtual ~Log()
	{ }

#ifdef BEAGLE_HAVE_RTTI
virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(Beagle::GP::Datum& outResult, Beagle::GP::Context& ioContext);

};

}
//}


#endif // Beagle_GP_Log_hpp
