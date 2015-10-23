#ifndef __SKETCH_H__
#define __SKETCH_H__

/* Abstract Class for Sketch
 *
 * This class defines several common
 * interfaces shared by all Sketches
 * in this library.
 */

template <typename T>   
class Sketch {
public:
  virtual void processItem(const T &item, double weight) = 0;
};

#endif
