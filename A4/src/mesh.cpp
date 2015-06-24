#include "mesh.hpp"
#include <iostream>

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
    : m_verts(verts),
      m_faces(faces) {
}

Intersection Mesh::calcInterection(Ray r) {
    Intersection intersect;
    intersect.valid = (false);
    Face currFace;
    Point3D  intersectionLocation;
    Vector3D normal;
    bool pointInsidePoly = false;
    double tmin = M_MAX;
    double t;
    bool pointDetected = false;

    for(std::vector<Face>::const_iterator x = m_faces.begin(); x != m_faces.end(); ++x) {
        currFace = (*x);
        Point3D vertices[ currFace.size() ];
        int count = 0;
        pointInsidePoly = true;

        for(Face::const_iterator it = currFace.begin(); it != currFace.end(); ++it) {
            vertices[count] = m_verts[ *it ];
            count++;
        }

        normal = (vertices[2] - vertices[1]).cross(vertices[0] - vertices[1]);
        normal.normalize();
        t = normal.dot(vertices[0] - r.origin) / normal.dot(r.direction);

        if(t < 0.01f || t > tmin)
            continue;

        intersectionLocation = r.origin + t * r.direction;

        for(int i = 0; i < count; i++) {
            Vector3D P1ToP2 = (vertices[(i + 1) % count] - vertices[i]);
            Vector3D VertexToIntersect = intersectionLocation - vertices[i];
            Vector3D perpVector = P1ToP2.cross(VertexToIntersect);
            double ioh = perpVector.dot(normal);

            if(ioh < 0) { //angle > 90 Deg
                pointInsidePoly = false;
                break;
            }
        }

        if(pointInsidePoly) {
            tmin = t;
            intersect.location = intersectionLocation;
            intersect.normal = (normal);
            intersect.t0 = (t);
            pointDetected = true;
        }
    }

    if(pointDetected)
        intersect.valid = (true);

    return intersect;
}

//Möller–Trumbore intersection algorithm

Intersection Mesh::triIntersect(Vector3D p1, Vector3D p2, Vector3D p3, Ray r) {
    // Vectors from p1 to p2/p3 (edges)
    Vector3D e1, e2;

    Vector3D p, q, T;
    float det, invDet, u, v, t;



    Intersection intersection;
    intersection.valid = false;


    //Find vectors for two edges sharing vertex/point p1
    e1 = p2 - p1;
    e2 = p3 - p1;

    // calculating determinant
    p = r.direction.cross(e2);

    //Calculate determinat
    det = e1.dot(p);

    //if determinant is near zero, ray lies in plane of triangle otherwise not
    if(det > -M_EPS && det < M_EPS)
        return intersection;

    invDet = 1.0f / det;

    //calculate distance from p1 to ray origin
    T = pToV(r.origin - p1);

    //Calculate u parameter
    u = T.dot(p) * invDet;

    //Check for ray hit
    if(u < 0 || u > 1)
        return intersection;

    //Prepare to test v parameter
    q = T.cross(e1);

    //Calculate v parameter
    v = r.direction.dot(q) * invDet;

    //Check for ray hit
    if(v < 0 || u + v > 1)
        return intersection;

    t = e2.dot(q) * invDet;

    if(t  > M_EPS) {

        intersection.location = r.origin + (t * r.direction);
        intersection.t0 = t;
        intersection.normal = (p3 - p2).cross(p1 - p2);
        intersection.normal.normalize();
        intersection.valid = true;

        return intersection;
    }

    // No hit at all
    return intersection;
}

std::ostream& operator<<(std::ostream & out, const Mesh & mesh) {
    std::cerr << "mesh({";

    for(std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
        if(I != mesh.m_verts.begin()) std::cerr << ",\n      ";

        std::cerr << *I;
    }

    std::cerr << "},\n\n     {";

    for(std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
        if(I != mesh.m_faces.begin()) std::cerr << ",\n      ";

        std::cerr << "[";

        for(Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
            if(J != I->begin()) std::cerr << ", ";

            std::cerr << *J;
        }

        std::cerr << "]";
    }

    std::cerr << "});" << std::endl;
    return out;
}
