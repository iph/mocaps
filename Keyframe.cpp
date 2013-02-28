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

void parse_floats_from_line(Keyframe * frame, string line, map<string, Bone *> & bone_map){
    string buf; // Have a buffer string
    stringstream ss(line); // Insert the string into a stream

	Float3 val;
    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf){
        tokens.push_back(buf);
	}
	if(tokens[0] == "root"){
		frame->root_trans[0] = atof(tokens[1].c_str());
		frame->root_trans[1] = atof(tokens[2].c_str());
		frame->root_trans[2] = atof(tokens[3].c_str());
		val[0] = atof(tokens[4].c_str());
		val[1] = atof(tokens[5].c_str());
		val[2] = atof(tokens[6].c_str());
	}
	else{
		int dof_iter = 0;
		for(int i = 1; i < tokens.size(); i++){
			//dof_iter = check_dof(dof_iter);
			//val[] = atof(tokens[i].c_str());
		}
	}


	//frame->bone_rots[tokens[0]] = val;
}

void Keyframe::build_from_file(string file_name){
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
			//parse_floats_from_line(current_frame, line);
		}
		line_content = sm.suffix().str();
	}

	Util::printDebug("\n");
	keyframes.erase(keyframes.begin(), keyframes.begin()+1);
}

