
#ifndef SKELETOR_H
#define SKELETOR_H
#include "Wm5ParticleController.h"
#include <iostream>
#include <sstream>

///////////////////MAKE LIFE EASIER!!!//////////////////////////
#define GETSET(type, var) \
private: \
   type _##var; \
public: \
   type Get##var() \
   {\
     return _##var; \
   }\
   void Set##var(type val) \
   {\
      _##var = val; \
   }
////////////////////////////////////////////

namespace Wm5
{

class Node;
class Bone;
class Skeleton
{
  
public:

	static void makeSkeleton(std::string content, Root * root, std::map<std::string, Bone *> & mappings, std::map<std::string, Node *> & mapz, Renderer * render);

protected:

private:
    
};


}

#endif
