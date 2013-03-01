// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef PARTICLESYSTEMS_H
#define PARTICLESYSTEMS_H

#include "Wm5WindowApplication3.h"
#include "Bone.h"
#include "Keyframe.h"
#include <string>
using namespace Wm5;

class ParticleSystems : public WindowApplication3
{
    WM5_DECLARE_INITIALIZE;
    WM5_DECLARE_TERMINATE;

public:
    ParticleSystems ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();
    virtual bool OnKeyDown (unsigned char key, int x, int y);
	Node * build_man_from_file(Renderer *, std::map<std::string, Bone *> &,  std::map<std::string, Node *> & );
	void apply_keyframe();


protected:
    void CreateScene ();
	Node * mRoot;
	std::map<std::string, Bone *> bone_map;
	std::map<std::string, Node *> wm_map;

	bool it_on;
	std::vector<Keyframe *> keyframes;
	int frame;
    NodePtr mScene;
    WireStatePtr mWireState;
    Culler mCuller;
    Float4 mTextColor;
};

WM5_REGISTER_INITIALIZE(ParticleSystems);
WM5_REGISTER_TERMINATE(ParticleSystems);

#endif
