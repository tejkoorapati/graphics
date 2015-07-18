#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "Viewer.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include <QOpenGLFunctions>
#include <vector>

class Primitive;
class Viewer;
class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(bool picking = false, Viewer* viewer = NULL) const;

  const QMatrix4x4& get_transform() const { return m_trans; }
  const QMatrix4x4& get_inverse() const { return m_invtrans; }
  SceneNode* findNode(std::string name);
  std::vector<Matrix4x4> matStack;

  QMatrix4x4 m_resetTrans;
  
  void set_transform(const QMatrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.inverted();
  }

  void set_transform(const QMatrix4x4& m, const QMatrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  void setViewer(Viewer* viewer){
      m_viewer = viewer;
  }

  int getId(){
      return m_id;
  }

  std::string getName(){
      return m_name;
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;

  std::vector<QMatrix4x4> undoStack;
  std::vector<QMatrix4x4> redoStack;

  void storeUndo();
  void storeRedo();
  void undo();
  void redo();
  void resetInitTrans();
  void reset();
  void resetInitRecurse();
  void resetRecurse();
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  QMatrix4x4 m_trans;
  QMatrix4x4 m_invtrans;
  Viewer* m_viewer;
  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false) const;

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  void rotate(char axis, double angle);

  float curXangle;
  float curYangle;


  struct JointRange {
    double min, init, max;
  };

  JointRange m_joint_x, m_joint_y;
  
private:

};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(bool picking = false, Viewer* viewer = NULL) const;

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
  }



protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
