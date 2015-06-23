#include "primitive.hpp"
#define EPS 0.000001
Primitive::~Primitive()
{
}

Intersection Primitive::calcInterection(Ray ray){
    Intersection intersection;
    return intersection;
}

Sphere::Sphere(){
    m_sphere = new NonhierSphere(Point3D(0,0,0),1);
}


Sphere::~Sphere()
{
    delete m_sphere;
}

Intersection Sphere::calcInterection(Ray ray)
{
    Intersection intersection = m_sphere->calcInterection(ray);
    return intersection;
}

Cube::Cube()
{
   m_cube = new NonhierBox(Point3D(0,0,0), 1.0);
}

Cube::~Cube()
{
    delete m_cube;
}

NonhierSphere::~NonhierSphere()
{
}


Intersection NonhierSphere::calcInterection(Ray ray)
{
    //using algo found on https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
    Intersection intersection;
    double t;
    double radius = m_radius;
    double Xc = m_pos[0];
    double Yc = m_pos[1];
    double Zc = m_pos[2];
    double X0 = ray.origin[0];
    double Y0 = ray.origin[1];
    double Z0 = ray.origin[2];
    double Xd = ray.direction[0];
    double Yd = ray.direction[1];
    double Zd = ray.direction[2];
    double a =  pow(Xd,2)+ pow(Yd,2) + pow(Zd,2);
    double b = 2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc));
    double c = pow((X0 - Xc),2) + pow((Y0 - Yc),2) + pow((Z0 - Zc),2) - pow(m_radius,2);
    //quad formula
    double check = b * b - 4.0 * a * c;

    if (check < 0.0) {
        intersection.valid = false;
    } else {


        t = (-b - sqrt(check)) / (2.0 * a);

        if (t > EPS) {
            intersection.t0 = t;
            intersection.location = ray.origin + (ray.direction * t);
            intersection.normal = intersection.location - m_pos;
            intersection.normal.normalize();
            intersection.valid = true;
        }

        t = (-b + sqrt(check)) / (2.0 * a);
        if (t > EPS) {
            intersection.t0 = t;
            intersection.location = ray.origin + (ray.direction * t);
            intersection.normal = intersection.location - m_pos;
            intersection.normal.normalize();
            intersection.valid = true;
        }
    }

    return intersection;
}

NonhierBox::NonhierBox(const Point3D &pos, double size)
    : m_pos(pos), m_size(size)
{

    m_verts.push_back(Point3D(m_pos[0],m_pos[1],m_pos[2])       );
    m_verts.push_back(Point3D(m_pos[0],m_pos[1],m_pos[2]+size)     );
    m_verts.push_back(Point3D(m_pos[0]+size,m_pos[1],m_pos[2]+size)   );
    m_verts.push_back(Point3D(m_pos[0]+size,m_pos[1],m_pos[2])     );
    m_verts.push_back(Point3D(m_pos[0],m_pos[1]+size,m_pos[2]+size)   );
    m_verts.push_back(Point3D(m_pos[0]+size,m_pos[1]+size,m_pos[2]+size) );
    m_verts.push_back(Point3D(m_pos[0]+size,m_pos[1]+size,m_pos[2])   );
    m_verts.push_back(Point3D(m_pos[0],m_pos[1]+size,m_pos[2])     );

    int facesArraged[6][4] =
    {
        {0,3,2,1}, //bottom
        {1,4,5,2}, //front
        {2,5,6,3}, //right
        {0,7,6,3}, //back
        {1,4,7,0}, //left
        {4,7,6,5}  //top
    };


    for (int f = 0 ;f < 6; f++) {
        std::vector<int> singleFace;
        for (int v = 0; v < 4; v++) {
            singleFace.push_back(facesArraged[f][v]);
        }
        m_faces.push_back(singleFace);
    }
}


//algo described on https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm

Intersection NonhierBox::calcInterection(Ray ray)
{


  double tnear = -9999999999999999;
  double tfar = 9999999999999999;
  Intersection intersection;
  intersection.valid = false;
  double t1,t2,o,d,l,h;

  for(int i = 0; i < 3; i++){
      o = ray.origin[i];
      d = ray.direction[i];
      l = m_faces[0][i];
      h = m_faces[5][i];
      if(d == 0 && (o < l || o > h)){
          return intersection;
      }
      t1 = (l-o)/d;
      t2 = (h-o)/d;

      if(t1 > t2){
          double temp = t1;
          t1 = t2;
          t2 = t2;
      }

      if(t1 > tnear){
          tnear = t1;
      }
      if(t2 < tfar){
          tfar = t2;
      }
      if (tnear > tfar || tfar < EPS){
          return intersection;
      }
      double t = tnear - 0.1f;
      intersection.t1 = tfar + 0.1f;
      intersection.t0 = t;
      intersection.location =  ray.origin + (ray.direction * t);
      intersection.valid = true;
      return intersection;

      //TODO: need to find the normal;


  }

}

NonhierBox::~NonhierBox()
{
}
