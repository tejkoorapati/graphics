#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "Viewer.hpp"

class Viewer;

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(Viewer* viewer, QMatrix4x4 trans, Colour color, std::string name) const = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(Viewer* viewer, QMatrix4x4 trans, Colour color, std::string name) const;
};

#endif
