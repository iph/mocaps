#include <windows.h>
#include <regex>
#include "ParticleSystems.h"
#include "Bone.h"
#include "Root.h"
#include "Skeleton.h"
using namespace std;
using namespace Wm5;

Bone::Bone(int id, std::string name, Float3 direction, Float3 axis, float length){
	SetId(id);
	SetName(name);
	SetDirection(direction);
	SetAxis(axis);
	SetLength(length);
}

string Bone::get_file_contents(const char *filename)
{
  ifstream in(filename, ios::in | ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);

}

 void Bone::printDebug(string text){
	OutputDebugString(text.c_str());
 }

 Node * Bone::build_man_from_file(Renderer * mRender){
	string herp = "herp.txt";
	const char * filename = herp.c_str();
	string file_contents = get_file_contents(filename);

	//This cryptic line is brought to you by: parsing bonedata code!
	regex parse_data(":bonedata([^:]*)");
	regex parse_root(":root([^:]*)");

	smatch sm;    
	// bone data that will be needed for the heirarchy.
	map<string, Bone *>  bone_map;
	map<string, Node *>  wm_map;
	
	//Read in the bone data information
	string temp_file_contents (file_contents);
	regex_search(temp_file_contents, sm, parse_data);
	build_bone_from_file(sm[1], bone_map);			

	//Pull in the root information.

	string temp_file_contents2 (file_contents);
	regex_search(temp_file_contents2, sm, parse_root);
	Root * root = Root::get_root_from_file(sm[1]);

	//Read in the hierarchical data.
	string temp_file_contents3 (file_contents);
	Skeleton::makeSkeleton(temp_file_contents3, root, bone_map, wm_map, mRender);
	return wm_map["root"];

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

    while (ss >> buf){
        tokens.push_back(buf);
	}
	Float3 direction(atof(tokens[5].c_str()), atof(tokens[6].c_str()), atof(tokens[7].c_str()));
	Float3 axis(atof(tokens[11].c_str()), atof(tokens[12].c_str()), atof(tokens[13].c_str()));
	
	Bone * got_milk = new Bone(atoi(tokens[1].c_str()), tokens[3], direction, axis, atof(tokens[9].c_str()));
	return got_milk;


 }