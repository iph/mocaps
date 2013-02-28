#include <windows.h>
#include <regex>
#include "Root.h"
using namespace std;
using namespace Wm5;

Root::Root(Float3 pos, Float3 ori){
	SetPosition(pos);
	SetOrientation(ori);
	
}

Root * Root::get_root_from_file(string contents){
	string buf;	
	string pos_str = "position";
	string orient_str = "orientation";



    stringstream ss(contents); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf){
        tokens.push_back(buf);
	}

	int pos_location = 0;
	while (pos_location < tokens.size() && tokens[pos_location] != pos_str){ // Find the position vector.
		pos_location++;
	}

	int ori_location = 0;
	//Bone bone = bones[i];
	while(tokens[ori_location] != orient_str){
		ori_location++;
	}

	Float3 position (atof(tokens[pos_location+1].c_str()), atof(tokens[pos_location+2].c_str()), atof(tokens[pos_location+3].c_str()));
	Float3 orient (atof(tokens[ori_location+1].c_str()), atof(tokens[ori_location+2].c_str()), atof(tokens[ori_location+3].c_str()));
	Root * root = new Root(position, orient);

	return root;
}