#include <windows.h>
#include <regex>
#include "ParticleSystems.h"
#include "Bone.h"
#include "Root.h"
#include "Skeleton.h"
using namespace std;
using namespace Wm5;

void Skeleton::makeSkeleton(string file_contents, Root * rt, map<string, Bone *> & mapping, map<string, Node *> & wm_map, Renderer * mRenderer){
    WireState * mWireState = new0 WireState();
    mRenderer->SetOverrideWireState(mWireState);

    VertexFormat* vformat = VertexFormat::Create(2,
        VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
        VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    int vstride = vformat->GetStride();
	StandardMesh smesh(vformat);

	std::string baseName = Environment::GetPathR("Flower.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
	
    Texture2DEffect* effect = new0 Texture2DEffect(Shader::SF_LINEAR);

	regex parse_begin_end("begin\r\n([()-.a-zA-Z0-9\n\r\t ]*?)end");
	regex parse_new_lines("(.*)\r\n");
	regex parse_heirarchy(":hierarchy([^:]*)");
	
    Node * root = new0 Node();
	root->LocalTransform.SetTranslate(APoint(rt->GetPosition()));
	Node * cur = root;
	wm_map["root"] = root;
	smatch sm;    

	//Read in the hierarchical data.
	string temp_file_contents3 (file_contents);
	regex_search(temp_file_contents3, sm, parse_heirarchy);
	string h_content = sm[1];
	regex_search(h_content, sm, parse_begin_end);

	string line_content = sm[1];
	while (regex_search (line_content, sm, parse_new_lines)) {
		for(int i = 1; i < sm.size(); i+=2){
			
			string buf; // Have a buffer string
			stringstream ss(sm[i]); // Insert the string into a stream
			vector<string> tokens; // Create vector to hold our words
			
			// Get the awesome shit into token shit.
			while (ss >> buf){
				tokens.push_back(buf);
				if(mapping[buf] && mapping[buf]->GetName() != ""){
					OutputDebugString(buf.c_str());
				}
			}
			OutputDebugString("\n");
			cur = wm_map[tokens[0]];
			for(int i = 1; i < tokens.size(); i++){
					Node * temp = new0 Node();
					TriMesh * tempMesh = smesh.Cylinder(10, 10, .10f, 1.0f, false);
					wm_map[tokens[i]] = temp;

					cur->AttachChild(temp);
					float len = mapping[buf]->GetLength();
					APoint p ( mapping[buf]->GetDirection()[0], mapping[buf]->GetDirection()[1], mapping[buf]->GetDirection()[2]);
					AVector other (mapping[buf]->GetDirection());
					p *= len;
					AVector y = AVector::UNIT_Z;
					AVector cross = other.Cross(y);
					float deg = Mathf::ACos(y.Dot(other));

					HMatrix rot, incr;
					rot = tempMesh->LocalTransform.GetRotate();
					incr.MakeRotation(cross, deg);
					tempMesh->LocalTransform.SetRotate(incr*rot);
					tempMesh->LocalTransform.SetScale(APoint(1.0, 1.0, len));
					tempMesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(baseTexture, Shader::SF_LINEAR,Shader::SC_CLAMP_EDGE, Shader::SC_CLAMP_EDGE));

					temp->LocalTransform.SetTranslate(p);
					temp->AttachChild(tempMesh);
			}
		}
		line_content = sm.suffix().str();
	}
	OutputDebugString("\n");

}