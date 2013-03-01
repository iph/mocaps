// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#include "ParticleSystems.h"
#include "Root.h"
#include "Skeleton.h"
#include "Keyframe.h"
#include <regex>
#include "Utils.h"
WM5_WINDOW_APPLICATION(ParticleSystems);

//----------------------------------------------------------------------------
ParticleSystems::ParticleSystems ()
    :
    WindowApplication3("SampleGraphics/ParticleSystems", 0, 0, 640, 480,
        Float4(0.5f, 0.0f, 1.0f, 1.0f)),
        mTextColor(1.0f, 1.0f, 1.0f, 1.0f)
{
}
//----------------------------------------------------------------------------
bool ParticleSystems::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }
	it_on = false;
	frame = 0;
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 1.0f, 1000.0f);
    APoint camPosition(26.730f, 0.0f, 0.0f);
    AVector camDVector(-1.0f, 0.0f, 0.0f);
    AVector camUVector(0.0f, 0.0f, 1.0f);
    AVector camRVector = camDVector.Cross(camUVector);
    mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);

    CreateScene();

    // Initial update of objects.
    mScene->Update();

    // Initial culling of scene.
    mCuller.SetCamera(mCamera);
    mCuller.ComputeVisibleSet(mScene);

    InitializeCameraMotion(0.001f, 0.001f);
    InitializeObjectMotion(mScene);
    return true;
}
//----------------------------------------------------------------------------
void ParticleSystems::OnTerminate ()
{
    mScene = 0;
    mWireState = 0;

    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void ParticleSystems::OnIdle ()
{
    MeasureTime();
	if(it_on){
		apply_keyframe();
	}
    MoveCamera();
    MoveObject();
    mScene->Update(GetTimeInSeconds());
    mCuller.ComputeVisibleSet(mScene);

    if (mRenderer->PreDraw())
    {
        mRenderer->ClearBuffers();
        mRenderer->Draw(mCuller.GetVisibleSet());
        DrawFrameRate(8, GetHeight()-8, mTextColor);
        mRenderer->PostDraw();
        mRenderer->DisplayColorBuffer();
    }

    UpdateFrameCount();
}

void ParticleSystems::apply_keyframe(){
	HMatrix L, L_inv;
	Keyframe * keyframe = keyframes[frame];
	frame++;
	frame %= keyframes.size();
	map<string, HMatrix> rotations = keyframe->bone_rots;

   for (std::map<string, HMatrix>::iterator it = rotations.begin(); it!=rotations.end(); ++it){
	   if(it->first == "root") continue;
		Bone * bone = bone_map[it->first];
		Node * node = wm_map[it->first];
		L = Util::rotation_x(bone->GetAxis()[0] * Mathf::DEG_TO_RAD) * Util::rotation_y(bone->GetAxis()[1] * Mathf::DEG_TO_RAD) * Util::rotation_z(bone->GetAxis()[2] * Mathf::DEG_TO_RAD);
		L_inv = L.Inverse();
		HMatrix final_mat = L_inv * it->second * L;
		node->LocalTransform.SetRotate(L_inv * it->second * L);

   }

}

//----------------------------------------------------------------------------
bool ParticleSystems::OnKeyDown (unsigned char key, int x, int y)
{
    if (WindowApplication3::OnKeyDown(key, x, y))
    {
        return true;
    }

    switch (key)
    {
    case 'w':
    case 'W':
		it_on = !it_on;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void ParticleSystems::CreateScene ()
{
    mScene = new0 Node();
    mWireState = new0 WireState();
    mRenderer->SetOverrideWireState(mWireState);


	mRoot = build_man_from_file(mRenderer, bone_map, wm_map);

	keyframes = Keyframe::build_from_file("02_01.amc", bone_map);

	/**
		Do the rotation of the matrix using 
		incr.makeRotation(crossproduct(Unit::Y, direction), 
	**/

	mScene->AttachChild(mRoot);
}
//----------------------------------------------------------------------------

 Node * ParticleSystems::build_man_from_file(Renderer * mRender, map<string, Bone *> & bone_map, map<string, Node *> & wm_map){
	string herp = "herp.txt";
	const char * filename = herp.c_str();
	string file_contents = Util::get_file_contents(filename);

	//This cryptic line is brought to you by: parsing bonedata code!
	regex parse_data(":bonedata([^:]*)");
	regex parse_root(":root([^:]*)");

	smatch sm;    
	// bone data that will be needed for the heirarchy.
	
	//Read in the bone data information
	string temp_file_contents (file_contents);
	regex_search(temp_file_contents, sm, parse_data);
	Bone::build_bone_from_file(sm[1], bone_map);			

	//Pull in the root information.
	string temp_file_contents2 (file_contents);
	regex_search(temp_file_contents2, sm, parse_root);
	Root * root = Root::get_root_from_file(sm[1]);

	//Read in the hierarchical data.
	string temp_file_contents3 (file_contents);
	Skeleton::makeSkeleton(temp_file_contents3, root, bone_map, wm_map, mRender);
	return wm_map["root"];

 }
