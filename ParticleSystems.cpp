// Geometric Tools, LLC
// Copyright (c) 1998-2012
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#include "ParticleSystems.h"
#include "BloodCellController.h"
#include "Root.h"

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
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 1.0f, 1000.0f);
    APoint camPosition(16.730f, 0.0f, 0.0f);
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
        mWireState->Enabled = !mWireState->Enabled;
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

	Node * root = Bone::build_man_from_file(mRenderer);
    VertexFormat* vformat = VertexFormat::Create(2,
        VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
        VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    int vstride = vformat->GetStride();
	StandardMesh sm(vformat);



	std::string baseName = Environment::GetPathR("Flower.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
	
    Texture2DEffect* effect = new0 Texture2DEffect(Shader::SF_LINEAR);
	TriMesh * cyl = sm.Cylinder(10, 10, .10f, 1.0f, false);
	HMatrix rot, incr;
	Float3 nope(1.0, 0.0, 0.0);
	AVector laaa(nope);
	cyl->LocalTransform.SetScale(APoint(1.0f, 1.0f, 7.0f));
	rot = cyl->LocalTransform.GetRotate();
	incr.MakeRotation(AVector::UNIT_X, 90.0 * Mathf::DEG_TO_RAD);
	cyl->LocalTransform.SetRotate(incr*rot);
	/**
		Do the rotation of the matrix using 
		incr.makeRotation(crossproduct(Unit::Y, direction), 
	**/

	mScene->AttachChild(root);
}
//----------------------------------------------------------------------------
