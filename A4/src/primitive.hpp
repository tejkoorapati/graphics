#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual Intersection calcInterection(Ray ray);
};


class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  Intersection calcInterection(Ray ray);
  virtual ~NonhierSphere();

private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size);
  Intersection calcInterection(Ray ray);
  
  virtual ~NonhierBox();
  std::vector<std::vector<int> > m_faces;
  std::vector<Point3D> m_verts;
  Point3D minBound;
  Point3D maxBound;

private:
  Point3D m_pos;
  double m_size;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  virtual Intersection calcInterection(Ray ray);

private:
   NonhierSphere* m_sphere;
};

class Cube : public Primitive {
public:
  Cube();
  NonhierBox* m_cube;
  virtual ~Cube();
};
#endif
