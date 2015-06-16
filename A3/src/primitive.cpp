#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(Viewer* viewer, QMatrix4x4 trans) const
{
  viewer->drawCircle(QColor(Qt::GlobalColor::blue),trans);
}
