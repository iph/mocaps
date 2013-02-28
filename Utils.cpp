#include <windows.h>
#include "ParticleSystems.h"
#include "Bone.h"
#include "Utils.h"
using namespace std;
using namespace Wm5;

 void Util::printDebug(string text){
	OutputDebugString(text.c_str());
 }


string Util::get_file_contents(const char *filename)
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

 HMatrix Util::rotation_x(float angle){
	return *(new HMatrix(
		1, 0, 0, 0,
		0, cos(angle), -sin(angle), 0,
		0, sin(angle), cos(angle), 0,
		0, 0, 0, 1)
		);
 }

 HMatrix Util::rotation_y(float angle){
	return *(new HMatrix(
		cos(angle), 0, sin(angle), 0,
		0, 1, 0, 0,
		-sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	));
 }

HMatrix Util::rotation_z(float angle){
	return *(new HMatrix(
		cos(angle), -sin(angle), 0, 0,
		sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	));
 }

HMatrix Util::build_rotation(Bone * bone){
	Float3 axis = bone->GetAxis();
	float alpha = axis[0];
	float beta = axis[1];
	float gamma = axis[2];

	HMatrix * rotation_mat = new HMatrix( 
		cos(beta) * cos(gamma), -cos(alpha)*sin(gamma)+sin(alpha)*sin(beta)*cos(gamma), sin(alpha)*sin(gamma)+cos(alpha)*sin(beta)*cos(gamma), 0, 
		cos(beta)*sin(gamma), cos(alpha)*cos(gamma)+sin(alpha)*sin(beta)*sin(gamma), -sin(alpha)*cos(gamma)+ cos(alpha)*sin(beta)*sin(gamma), 0,
		-sin(beta), sin(alpha)*cos(beta), cos(alpha)*cos(beta), 0,
		0, 0, 0, 1
	);
	return *rotation_mat; 
		/*



						   ;\
						  _' \_
						,' '  '`.
					   ;,)       \
	what the fuck     /          :
	 am i doing       (_         :
					   `--.       \
						  /        `.
						 ;           `.
						/              `.
					   :                 `.
					   :                   \
						\\                  \
						 ::                 :
						 || |               |
						 || |`._            ;
						_;; ; __`._,       (________
					   ((__/(_____(______,'______(___)    


	*/
 }
