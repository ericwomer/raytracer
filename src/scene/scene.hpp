#if !defined(__SCENE_H__)
#define __SCENE_H__

#include "types/vec3.h"

class scene {
public:
	scene() { root = vec3(0, 0, 0); }
	vec3 root; // Should always be 0,0,0
}
#endif // __SCENE_H__