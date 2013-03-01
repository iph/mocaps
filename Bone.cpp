#include <windows.h>
#include <regex>
#include "Bone.h"
#include "ParticleSystems.h"
#include "Root.h"
#include "Skeleton.h"
#include "Utils.h"

using namespace std;
using namespace Wm5;

Bone::Bone(int id, std::string name, Float3 direction, Float3 axis, float length){
	SetId(id);
	SetName(name);
	SetDirection(direction);
	SetAxis(axis);
	SetLength(length);
	SetRx(false);
	SetRy(false);
	SetRz(false);
}


 /**
 * String must be in the form of:
 *	begin
 *	(contents)
 *	end
 */
 void Bone::build_bone_from_file(string contents, map<string, Bone *> & mapper){
	 
	regex parse("begin([()-.a-zA-Z0-9\n\r\t ]*?)end");
	smatch sm; 

	while (std::regex_search (contents,sm,parse)) {
		for(int i = 1; i < sm.size(); i+=2){
			string check = sm[i];

			//OutputDebugString(check.c_str());	
			Bone * bonez = build_node_from_file(check);
			mapper[bonez->GetName()] = bonez;
		}
		contents = sm.suffix().str();

	}
 }

 Bone * Bone::build_node_from_file(string contents){
    string buf; // Have a buffer string
    stringstream ss(contents); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words
	vector<string> dof;

    while (ss >> buf){
        tokens.push_back(buf);
	}
	Float3 direction(atof(tokens[5].c_str()), atof(tokens[6].c_str()), atof(tokens[7].c_str()));
	Float3 axis(atof(tokens[11].c_str()), atof(tokens[12].c_str()), atof(tokens[13].c_str()));

	Bone * got_milk = new Bone(atoi(tokens[1].c_str()), tokens[3], direction, axis, atof(tokens[9].c_str()));
	for(int i = 16; i < 19 && i < tokens.size(); i++){
		if(tokens[i] == "rx"){
			got_milk->SetRx(true);
		}
		if(tokens[i] == "ry"){
			got_milk->SetRy(true);
		}
		if(tokens[i] == "rz"){
			got_milk->SetRz(true);
		}
	}
	return got_milk;
 }