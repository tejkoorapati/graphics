#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(Viewer* viewer, QMatrix4x4 trans, Colour color, std::string name) const
{

  viewer->drawSphere(QColor(color.R(),color.G(),color.B()),trans, name);
}
