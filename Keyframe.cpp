#include <windows.h>
#include <regex>
#include "ParticleSystems.h"
#include "Bone.h"
#include "Keyframe.h"
#include "Utils.h"

using namespace std;
using namespace Wm5;

bool isNumeric(char possible){
	const char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	for(int i = 0; i < 10; i++){
		if(possible == numbers[i]){
			return true;
		}
	}
	return false;
}

int iter_dof(int old_dof, Bone * bone){
	if( old_dof <= 0 && bone->GetRx() ){
		return 0;	
	}
	else if( old_dof <= 1 && bone->GetRy() ){
		return 1;
	}
	else if (old_dof <= 2 && bone->GetRz() ){
		return 2;
	}

	return -1;
}

void parse_floats_from_line(Keyframe * frame, string line, map<string, Bone *> & bone_map){
    string buf; // Have a buffer string
    stringstream ss(line); // Insert the string into a stream

	Float3 val (0.0, 0.0, 0.0);
    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf){
        tokens.push_back(buf);
	}
	if(tokens[0] == "root"){
		frame->root_trans[0] = atof(tokens[1].c_str());
		frame->root_trans[1] = atof(tokens[2].c_str());
		frame->root_trans[2] = atof(tokens[3].c_str());
		val[0] = atof(tokens[4].c_str()) * Mathf::DEG_TO_RAD;
		val[1] = atof(tokens[5].c_str()) * Mathf::DEG_TO_RAD;
		val[2] = atof(tokens[6].c_str()) * Mathf::DEG_TO_RAD;
	}
	else{
		int dof_iter = 0;
		for(int i = 1; i < tokens.size(); i++){
			dof_iter = iter_dof(dof_iter, bone_map[tokens[0]]);
			val[dof_iter] = atof(tokens[i].c_str()) * Mathf::DEG_TO_RAD;
			dof_iter++;
		}
	}
	HMatrix cur = HMatrix::IDENTITY;
	if(val[0] != 0.00){
		cur = Util::rotation_x(val[0]);
	}
	if(val[1] != 0.00){
		if( cur == HMatrix::IDENTITY){
			cur = Util::rotation_y(val[1]);
		}
		else{
			cur = cur * Util::rotation_y(val[1]);
		}
	}
	if(val[2] != 0.00){
		if( cur == HMatrix::IDENTITY){
			cur = Util::rotation_z(val[2]);
		}
		else{
			cur = cur * Util::rotation_z(val[2]);
		}
	}
	frame->bone_rots[tokens[0]] = cur;
}

vector<Keyframe *> Keyframe::build_from_file(string file_name, map<string, Bone *> & bone_map){
	vector<Keyframe *> keyframes;

	string full_contents = Util::get_file_contents(file_name.c_str());	

	regex parse_new_lines("(.*)\r\n");
	smatch sm;

	string line_content(full_contents);
	Keyframe * current_frame = new Keyframe();
	while (regex_search (line_content, sm, parse_new_lines)) {
		string line = sm[1];

		if(line[0] == '#' || line[0] == ':'){
			// Continue
		}
		else if (isNumeric(line[0])){
			keyframes.push_back(current_frame);
			current_frame = new Keyframe();
		}
		else{
			parse_floats_from_line(current_frame, line, bone_map);
		}
		line_content = sm.suffix().str();
	}

	Util::printDebug("\n");
	keyframes.erase(keyframes.begin(), keyframes.begin()+1);
	return keyframes;
}

