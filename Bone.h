// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef BONE_H
#define BONE_H
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

class Root;
class Bone
{
  
public:
	Bone::Bone(int id, std::string name, Float3 direction, Float3 axis, float length);	
	static Node * build_man_from_file(Renderer * render);
	static void build_bone_from_file(std::string contents, std::map<std::string, Bone *> & mapper);
	static Bone * build_node_from_file(std::string contents);
	GETSET(int, Id);
	GETSET(std::string, Name);
	GETSET(Float3, Direction);
	GETSET(Float3, Axis);
	GETSET(float, Length);
	static std::string get_file_contents(const char *filename);
	static void printDebug(std::string text);


protected:

private:
   static void build_root_from_file(); 
};


}

#endif
