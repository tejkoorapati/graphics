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

SceneNode* SceneNode::findNode(std::string name)
{
    if (m_name == name) {
        return this;
    }
    for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
        SceneNode *node = (*it)->findNode(name);
        if (node!=NULL) return node;
    }
    return NULL;
}

void SceneNode::rotate(char axis, double angle)
{

    //    std::cerr << "Stub: Rotate " << m_name << " by " << angle ;

    QMatrix4x4 temp;
    temp.setToIdentity();

    JointNode* myNode = dynamic_cast<JointNode*>(this);

    switch(axis){

    case 'x':
        if(myNode->curXangle + angle > myNode->m_joint_x.max){
            angle =  myNode->m_joint_x.max - myNode->curXangle;
        }
        if(myNode->curXangle + angle < myNode->m_joint_x.min){
            angle =  myNode->m_joint_x.min - myNode->curXangle;
        }
        myNode->curXangle += angle;
        std::cerr << "axis x with angle"<< angle << std::endl;
        temp.rotate(angle,1,0,0);
        break;
    case 'y':
        if(myNode->curYangle + angle > myNode->m_joint_y.max){
            angle =  myNode->m_joint_y.max - myNode->curYangle;
        }
        if(myNode->curYangle + angle < myNode->m_joint_y.min){
            angle =  myNode->m_joint_y.min - myNode->curYangle;
        }
        myNode->curYangle += angle;
        std::cerr << "axis y with angle"<< angle << std::endl;
        temp.rotate(angle,0,1,0);
        break;
    case 'z' :
        std::cerr << "axis z with angle"<< angle << std::endl;
        temp.rotate(angle,0,0,1);
        break;
    }

    set_transform(m_trans*temp);
}

void SceneNode::scale(const Vector3D& amount)
{
    //  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;

    QMatrix4x4 temp;
    temp.scale(amount[0],amount[1],amount[2]);
    set_transform(m_trans*temp);
}

void SceneNode::translate(const Vector3D& amount)
{
    //  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
    QMatrix4x4 temp;
    int scaleAmount = 1;
    temp.translate(amount[0]*scaleAmount,amount[1]*scaleAmount,amount[2]*scaleAmount);

    set_transform(m_trans*temp);
}

bool SceneNode::is_joint() const
{
    return false;
}

void SceneNode::storeUndo()
{
    undoStack.push_back(m_trans);
}

void SceneNode::storeRedo()
{
    redoStack.push_back(m_trans);
}

void SceneNode::undo()
{
    if(undoStack.size() == 0){
        std::cerr << "No transformations to undo." << std::endl;
        return;
    }
    storeRedo();
    QMatrix4x4 undoMat = undoStack[undoStack.size()-1];
    m_trans = undoMat;
    undoStack.pop_back();

}


void SceneNode::redo(){
    if(redoStack.size() == 0){
        std::cerr << "No transformations to redo." << std::endl;
        return;
    }
    storeUndo();
    QMatrix4x4 redoMat = redoStack[redoStack.size()-1];
    m_trans = redoMat;
    redoStack.pop_back();
}

void SceneNode::resetInitTrans()
{
    m_resetTrans = m_trans;
}

void SceneNode::reset()
{
    m_trans = m_resetTrans;
}

void SceneNode::resetInitRecurse()
{
    resetInitTrans();
    for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
        (*it)->resetInitRecurse();
    }
}

void SceneNode::resetRecurse()
{
    reset();
    undoStack.clear();
    redoStack.clear();
    for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
        (*it)->resetRecurse();
    }
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
    curXangle = init;
    m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
    m_joint_y.min = min;
    m_joint_y.init = init;
    curYangle - init;
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
    //    std::cerr << "Printing: " << m_name <<  std::endl;

    Colour color = Colour(1.0,1.0,0.0);

    if(m_material != NULL && m_material){
        color = m_material->getColor();
    }
    m_primitive->walk_gl(m_viewer,m_trans, color, m_name );

    if(picking)
    {
        glPopName();
    }

}

