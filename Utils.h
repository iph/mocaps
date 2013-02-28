

// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef UTILS_H
#define UTILS_H
#include "Wm5ParticleController.h"
#include <iostream>
#include <sstream>

namespace Wm5
{

class Bone;

class Util{
	public:
		static std::string get_file_contents(const char *filename);
		static void printDebug(std::string);
		static HMatrix build_rotation(Bone * bone);
		static HMatrix rotation_x(float angle);
		static HMatrix rotation_y(float angle);
		static HMatrix rotation_z(float angle);
};
}
#endif
