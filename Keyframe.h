// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef KEYFRAME_H
#define KEYFRAME_H
#include "Wm5ParticleController.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>

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
using namespace std;
namespace Wm5
{

class Keyframe
{
  
public:
	static void build_from_file(string);
	map<string, Float3> bone_rots;
	Float3 root_trans;
protected:

private:
};


}

#endif
