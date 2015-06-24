#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
    : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

Intersection SceneNode::calcIntersection(Ray ray)
{
    Intersection intersection;
    intersection.valid = false;
    //make ray hit the transform nodes
    ray.direction = m_invtrans * ray.direction;
    ray.origin = m_invtrans * ray.origin;
    for (ChildList::const_iterator it = m_children.begin();it != m_children.end(); ++it) //check intersections for children
    {
        SceneNode *currentNode = *it;
        Intersection childIntersection = currentNode->calcIntersection(ray);
        if (childIntersection.valid)
        {
            if (!intersection.valid || childIntersection.t0 < intersection.t0) //get closest intersection
                intersection = childIntersection;
        }
    }
    //apply the tansformations to return to the original ray and calc transformed normal
    if (intersection.valid)
    {
        intersection.location = m_trans * intersection.location;
        intersection.normal = m_invtrans.transpose() * intersection.normal;
    }
    return intersection;
}

void SceneNode::rotate(char axis, double angle)
{
    std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
    Matrix4x4 r;

    //code from a2
    Vector4D r1,r2,r3,r4;
    angle = angle * M_PI/ 180.0;

    double c = cos(angle);
    double s = sin(angle);

    switch(axis) {
    case 'x':
        r1 = Vector4D(1, 0, 0, 0);
        r2 = Vector4D(0, c, -s, 0);
        r3 = Vector4D(0, s, c, 0);
        r4 = Vector4D(0, 0, 0, 1);
        break;
    case 'y':
        r1 = Vector4D(c, 0, s, 0);
        r2 = Vector4D(0, 1, 0, 0);
        r3 = Vector4D(-s, 0, c, 0);
        r4 = Vector4D(0, 0, 0, 1);
        break;
    case 'z':
        r1 = Vector4D(c, -s, 0, 0);
        r2 = Vector4D(s, c, 0, 0);
        r3 = Vector4D(0, 0, 1, 0);
        r4 = Vector4D(0, 0, 0, 1);
        break;
    }
    r = Matrix4x4(r1,r2,r3,r4);
    set_transform(m_trans*r);

}

void SceneNode::scale(const Vector3D& amount)
{
    std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
    //code from a2
    Matrix4x4 s;
    Vector4D r1(amount[0], 0, 0, 0);
    Vector4D r2(0, amount[1], 0, 0);
    Vector4D r3(0, 0, amount[2], 0);
    Vector4D r4(0, 0, 0, 1);

    s = Matrix4x4(r1,r2,r3,r4);
    set_transform(m_trans*s);

}

void SceneNode::translate(const Vector3D& amount)
{
    std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
    //code from a2
    Matrix4x4 t;
    Vector4D r1(1, 0, 0, amount[0]);
    Vector4D r2(0, 1, 0, amount[1]);
    Vector4D r3(0, 0, 1, amount[2]);
    Vector4D r4(0, 0, 0, 1);

    t = Matrix4x4(r1,r2,r3,r4);
    set_transform(m_trans*t);

}

bool SceneNode::is_joint() const
{
    return false;
}

JointNode::JointNode(const std::string& name)
    : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
    return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
    m_joint_x.min = min;
    m_joint_x.init = init;
    m_joint_x.max = max;
}
void JointNode::set_joint_y(double min, double init, double max)
{
    m_joint_y.min = min;
    m_joint_y.init = init;
    m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
    : SceneNode(name),
      m_primitive(primitive)
{
}


GeometryNode::~GeometryNode()
{
}

Intersection GeometryNode::calcIntersection(Ray ray)
{
    //make ray hit the transform nodes
    ray.direction = m_invtrans * ray.direction;
    ray.origin = m_invtrans * ray.origin;
    Intersection intersection = m_primitive->calcInterection(ray);
    intersection.material = m_material;

    for (ChildList::const_iterator it = m_children.begin();it != m_children.end(); ++it) //check intersections for children
    {
        SceneNode *currentNode = *it;
        Intersection childIntersection = currentNode->calcIntersection(ray);
        if (childIntersection.valid)
        {
            if (!intersection.valid || childIntersection.t0 < intersection.t0) //get closest intersection
                intersection = childIntersection;
        }
    }
    //apply the tansformations to return to the original ray and calc transformed normal
    if (intersection.valid)
    {
        intersection.location = m_trans * intersection.location;
        intersection.normal = m_invtrans.transpose() * intersection.normal;
    }
    return intersection;
}

