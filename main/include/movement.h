#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "processor.h"

class Movement : public Processor {
 public:
  Movement(EntityManager& entity_manager);
  virtual void update(long ms);
};

#endif // MOVEMENT_H_

