#ifndef __SKETCH_H__
#define __SKETCH_H__
#include "config.h"

/* Abstract Class for Sketch
 *
 * This class defines several common
 * interfaces shared by all Sketches
 * in this library.
 */
   
class Sketch {
public:
  virtual void processItem(const ItemType &item, double weight) = 0;
};

#endif
