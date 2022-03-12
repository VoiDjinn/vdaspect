#include "VDEntityNode.h"
#include "VDEntity.h"

VDEntityNode::VDEntityNode() {
  tagged_node = nullptr;
  set_name("Entity");
}

void VDEntityNode::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_composer", "composer"), &VDEntityNode::set_composer);
  ClassDB::bind_method(D_METHOD("get_composer"),             &VDEntityNode::get_composer);

  ClassDB::bind_method(D_METHOD("add_aspect",    "aspect"), &VDEntityNode::add_aspect);
  ClassDB::bind_method(D_METHOD("remove_aspect", "aspect"), &VDEntityNode::remove_aspect);

  ClassDB::bind_method(D_METHOD("has_aspect", "name"), &VDEntityNode::has_aspect);
  ClassDB::bind_method(D_METHOD("get_aspect", "name"), &VDEntityNode::get_aspect);

  ClassDB::bind_method(D_METHOD("get_data", "name"), &VDEntityNode::get_data);

  ClassDB::bind_method(D_METHOD("set_tagged_node_path", "path"),  &VDEntityNode::set_tagged_node_path);
  ClassDB::bind_method(D_METHOD("get_tagged_node_path"),          &VDEntityNode::get_tagged_node_path);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "aspects", PROPERTY_HINT_RESOURCE_TYPE, "VDAspectComposer"), "set_composer", "get_composer");
  ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "tagged_node"), "set_tagged_node_path", "get_tagged_node_path");
}

void VDEntityNode::_notification(int p_notification) {
  switch(p_notification) {
    case NOTIFICATION_READY: {
      recheck();
      break;
    }
  }
}

void VDEntityNode::reregister(Node* new_entity) {
  if(new_entity != tagged_node) {
    if(tagged_node != nullptr && VDEntity::get_singleton()->is_entity(tagged_node)) {
      VDEntity::get_singleton()->unregister_entity(this);
    }
    if(new_entity != nullptr) {
      VDEntity::get_singleton()->register_entity(this, new_entity);
    }
    tagged_node = new_entity;
  }
}

void VDEntityNode::recheck() {
  Node* new_owner = nullptr;
  if(tagged_node_path.operator String() != "") {
    new_owner = get_node_or_null(tagged_node_path);
  } else {
    new_owner = get_parent();
  }
  if(new_owner != tagged_node) {
    reregister(new_owner);
  }
}

void VDEntityNode::set_composer(Ref<VDAspectComposer> composer) {
  this->composer = composer;
}

Ref<VDAspectComposer> VDEntityNode::get_composer() {
  return this->composer;
}

bool VDEntityNode::add_aspect(Ref<VDAspect> aspect) {
  if(this->composer.is_valid()) {
    return this->composer->add_aspect(aspect);
  }
  return false;
}

bool VDEntityNode::remove_aspect(Ref<VDAspect> aspect) {
  if(this->composer.is_valid()) {
    return this->composer->remove_aspect(aspect);
  }
  return false;
}

bool VDEntityNode::has_aspect(StringName name) {
  if(this->composer.is_valid()) {
    return this->composer->has_aspect(name);
  }
  return false;
}

Ref<VDAspect> VDEntityNode::get_aspect(StringName name) {
  Ref<VDAspect> aspect;
  if(this->composer.is_valid()) {
    aspect = this->composer->get_aspect(name);
  }
  return aspect;
}

Ref<VDAspectData> VDEntityNode::get_data(StringName name) {
  Ref<VDAspectData> data;
  if(this->composer.is_valid()) {
    data = this->composer->get_data(name);
  }
  return data;
}

void VDEntityNode::set_tagged_node_path(NodePath path) {
  if(tagged_node_path != path) {
    tagged_node_path = path;
    recheck();
  }
}

NodePath VDEntityNode::get_tagged_node_path() {
  return tagged_node_path;
}

Node * VDEntityNode::get_tagged_node() {
  return tagged_node;
}
