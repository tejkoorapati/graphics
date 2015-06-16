#include "scene.hpp"
#include <iostream>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
    m_trans.setToIdentity();
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking, Viewer* viewer) const
{
    ChildList::const_iterator i;
    //go throught children
    for ( i = m_children.begin(); i != m_children.end(); ++i )
    {
        (*i)->setViewer(m_viewer);
        (*i)->set_transform( m_trans * (*i)->get_transform() ); //carry transforms to child
        (*i)->walk_gl( picking );
        (*i)->set_transform(this->m_invtrans * (*i)->get_transform()); //undo after draw
    }
}

void SceneNode::rotate(char axis, double angle)
{

    std::cerr << "Stub: Rotate " << m_name << " by " << angle ;

    QMatrix4x4 temp;
    temp.setToIdentity();


    switch(axis){

        case 'x':
        std::cerr << "axis x" << std::endl;
        temp.rotate(angle,1,0,0);
        break;
        case 'y':
        std::cerr << "axis x" << std::endl;
         temp.rotate(angle,0,1,0);
        break;
        case 'z' :
        std::cerr << "axis x" << std::endl;
         temp.rotate(angle,0,0,1);
        break;
    }

  set_transform(m_trans*temp);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;

  QMatrix4x4 temp;
  temp.scale(amount[0],amount[1],amount[2]);
  set_transform(m_trans*temp);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  QMatrix4x4 temp;
  int scaleAmount = 1;
  temp.translate(amount[0]*scaleAmount,amount[1]*scaleAmount,amount[2]*scaleAmount);

  set_transform(m_trans*temp);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
    m_trans.setToIdentity();
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
    ChildList::const_iterator i;
    //go throught children
    for ( i = m_children.begin(); i != m_children.end(); ++i )
    {
        (*i)->setViewer(m_viewer);
        (*i)->set_transform( m_trans * (*i)->get_transform() ); //carry transforms to child
        (*i)->walk_gl( picking );
        (*i)->set_transform(this->m_invtrans * (*i)->get_transform()); //undo after draw
    }
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
    m_trans.setToIdentity();
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking, Viewer* viewer) const
{
    if(picking)
    {
        glPushName( m_id );
    }
    std::cerr << "Printing: " << m_name <<  std::endl;
   m_primitive->walk_gl(m_viewer,m_trans);

    if(picking)
    {
        glPopName();
    }

}
 
