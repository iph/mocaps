// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef ROOT_H
#define ROOT_H
#include "Wm5ParticleController.h"
#include <iostream>
#include <sstream>

///////////////////MAKE LIFE EASIER!!!//////////////////////////
#define GETSET(type, var) \
private: \
   type _##var; \
public: \
   type Get##var() \
   {\
     return _##var; \
   }\
   void Set##var(type val) \
   {\
      _##var = val; \
   }
////////////////////////////////////////////

namespace Wm5
{

class Root
{
  
public:
	Root(Float3, Float3);
	GETSET(Float3, Position);
	GETSET(Float3, Orientation);
	static Root * get_root_from_file(std::string contents);

protected:

private:
};


}

#endif
