#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);
  Intersection calcInterection(Ray ray);

  typedef std::vector<int> Face;
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  Intersection triIntersect(Vector3D p1, Vector3D p2, Vector3D p3, Ray r);

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
