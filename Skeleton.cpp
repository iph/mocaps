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

	string baseName = Environment::GetPathR("SunFire.wmtf");
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

	Node herp;
	string line_content = sm[1];
	while (regex_search(line_content, sm, parse_new_lines)) {
		for(int i = 1; i < sm.size(); i+=2){
			
			string buf; // Have a buffer string
			stringstream ss(sm[i]); // Insert the string into a stream
			vector<string> tokens; // Create vector to hold our words

			// Get the awesome shit into token shit.
			while (ss >> buf){
				tokens.push_back(buf);
			}
			cur = wm_map[tokens[0]];

			for(int i = 1; i < tokens.size(); i++){
				string name = tokens[i];
				Node * temp = new0 Node();
				TriMesh * tempMesh = smesh.Cylinder(10, 10, .10f, 1.0f, false);
				wm_map[name] = temp;

				// Setup of all the stuff we will need
				float len = mapping[name]->GetLength();
				float old_len = 0.0f;
				APoint translate_amt;
				if( tokens[0] != "root"){
					translate_amt = APoint(mapping[tokens[0]]->GetDirection());
					old_len = mapping[tokens[0]]->GetLength();
				}
				translate_amt *= old_len;
				APoint translate_bone_amt(mapping[name]->GetDirection());
				translate_bone_amt *= (len / 2);

				AVector dir (mapping[name]->GetDirection());
				AVector z = AVector::UNIT_Z;
				AVector cross = z.Cross(dir);
				float deg = Mathf::ACos(z.Dot(dir));
				HMatrix rot, incr;

				// Scale, translate and rotate only the bone mesh.
				tempMesh->LocalTransform.SetScale(APoint(1.0, 1.0, len));
				tempMesh->LocalTransform.SetTranslate(translate_bone_amt);
				rot = tempMesh->LocalTransform.GetRotate();
				incr.MakeRotation(cross, deg);
				tempMesh->LocalTransform.SetRotate(incr * rot);
				tempMesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(baseTexture, Shader::SF_LINEAR,Shader::SC_CLAMP_EDGE, Shader::SC_CLAMP_EDGE));

				temp->LocalTransform.SetTranslate(translate_amt);
				cur->AttachChild(temp);
				temp->AttachChild(tempMesh);
			}
		}
		line_content = sm.suffix().str();
	}
}