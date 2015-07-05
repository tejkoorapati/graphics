#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(Viewer* viewer, QMatrix4x4 trans, Colour color, std::string name) const
{

  //viewer->drawSphere(QColor((int)(color.R()*255),(int)(color.G()*255),(int)(color.B()*255)),trans, name);
  viewer->drawSphere(QColor(100,50,0),trans, name);
}
