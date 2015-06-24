#include "primitive.hpp"

Primitive::~Primitive() {
}

Intersection Primitive::calcInterection(Ray ray) {
    Intersection intersection;
    return intersection;
}

Sphere::Sphere() {
    m_sphere = new NonhierSphere(Point3D(0, 0, 0), 1);
}


Sphere::~Sphere() {
    delete m_sphere;
}

Intersection Sphere::calcInterection(Ray ray) {
    Intersection intersection = m_sphere->calcInterection(ray);
    return intersection;
}

Cube::Cube() {
    m_cube = new NonhierBox(Point3D(0, 0, 0), 1.0);
}

Cube::~Cube() {
    delete m_cube;
}

NonhierSphere::~NonhierSphere() {
}


Intersection NonhierSphere::calcInterection(Ray ray) {
//using algo found on https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm

    Intersection intersection;
    intersection.valid = false;
    double t = 0;
//    double Xc = m_pos[0];
//    double Yc = m_pos[1];
//    double Zc = m_pos[2];

//    double X0 = ray.origin[0];
//    double Y0 = ray.origin[1];
//    double Z0 = 10;

    double Xd = ray.direction[0];
    double Yd = ray.direction[1];
    double Zd = ray.direction[2];
    double a =  pow(Xd, 2) + pow(Yd, 2) + pow(Zd, 2);
    double b = 2 * (ray.origin - m_pos).dot(ray.direction);
    double c = (ray.origin - m_pos).dot(ray.origin - m_pos) - pow(m_radius, 2);
//quad formula

    double roots[2];
    int numRoots = quadraticRoots(a, b, c, roots);

    if(numRoots < 1)   return intersection;

    if(numRoots == 1)
        t = roots[0];


    else if(numRoots == 2) {
        if(roots[0] < roots[1])
            t = roots[0];


        else if(roots[0] > roots[1])
            t = roots[1];
    }

    if(t > M_EPS) {
        intersection.t0 = t;
        intersection.location = ray.origin + (ray.direction * t);
        intersection.normal = intersection.location - m_pos;
        intersection.normal.normalize();
        intersection.valid = true;
    }


    return intersection;



}

NonhierBox::NonhierBox(const Point3D &pos, double size)
    : m_pos(pos), m_size(size) {


    m_verts.push_back(Point3D(m_pos[0], m_pos[1], m_pos[2]));
    m_verts.push_back(Point3D(m_pos[0], m_pos[1], m_pos[2] + size));
    m_verts.push_back(Point3D(m_pos[0] + size, m_pos[1], m_pos[2] + size));
    m_verts.push_back(Point3D(m_pos[0] + size, m_pos[1], m_pos[2]));
    m_verts.push_back(Point3D(m_pos[0], m_pos[1] + size, m_pos[2] + size));
    m_verts.push_back(Point3D(m_pos[0] + size, m_pos[1] + size, m_pos[2] + size));
    m_verts.push_back(Point3D(m_pos[0] + size, m_pos[1] + size, m_pos[2]));
    m_verts.push_back(Point3D(m_pos[0], m_pos[1] + size, m_pos[2]));
}


//algo described on https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm

Intersection NonhierBox::calcInterection(Ray ray) {
    double tnear = M_MIN;
    double tfar = M_MAX;
    Intersection intersection;
    intersection.valid = false;
    double t1, t2, o, d, l, h;
    for(int i = 0; i < 3; i++) {
        o = ray.origin[i];
        d = ray.direction[i];
        l = m_verts[0][i];
        h = m_verts[5][i];
        assert(l < h);
        if(d == 0 && (o < l || o > h)) {
            return intersection;
        }
        t1 = (l - o) / d;
        t2 = (h - o) / d;

        if(t1 > t2) {
            double temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if(t1 > tnear) {
            tnear = t1;
            Vector3D vec ;
            vec[i] = -ray.direction[i];
            intersection.normal = vec;
            intersection.normal.normalize();
        }

        if(t2 < tfar)
            tfar = t2;

        if(tnear > tfar || tfar < M_EPS)
            return intersection;
    }

//TODO: need to find the normal;
    double t = tnear - 0.1f;
    intersection.t1 = tfar + 0.1f;
    intersection.t0 = t;
    intersection.location =  ray.origin + (ray.direction * t);
    intersection.valid = true;
    return intersection;

}

NonhierBox::~NonhierBox() {
}
