#if !defined(SCENE_H)
#define SCENE_H

#include "types/vec3.h"

class scene {
  public:
  scene() { root = Vec3(0, 0, 0); }
  Vec3 root;  // Should always be 0,0,0
};

#endif  // SCENE_H