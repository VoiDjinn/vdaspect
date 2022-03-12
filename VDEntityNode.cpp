#include "VDEntityNode.h"
#include "VDEntity.h"

VDEntityNode::VDEntityNode() {
  entity_node = NULL;
}

void VDEntityNode::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_composer", "composer"), &VDEntityNode::set_composer);
  ClassDB::bind_method(D_METHOD("get_composer"),             &VDEntityNode::get_composer);

  ClassDB::bind_method(D_METHOD( "add_aspect",    "aspect" ), &VDEntityNode::add_aspect);
  ClassDB::bind_method(D_METHOD( "remove_aspect", "aspect" ), &VDEntityNode::remove_aspect);

  ClassDB::bind_method(D_METHOD( "has_aspect", "name" ), &VDEntityNode::has_aspect);
  ClassDB::bind_method(D_METHOD( "get_aspect", "name" ), &VDEntityNode::get_aspect);

  ClassDB::bind_method(D_METHOD( "get_data", "name" ), &VDEntityNode::get_data);

  ClassDB::bind_method(D_METHOD("set_entity_path", "path"),  &VDEntityNode::set_entity_path);
  ClassDB::bind_method(D_METHOD("get_entity_path"),          &VDEntityNode::get_entity_path);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "aspects", PROPERTY_HINT_RESOURCE_TYPE, "VDAspectComposer"), "set_composer", "get_composer");
  ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "entity"), "set_entity_path", "get_entity_path");
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
  if(new_entity != entity_node) {
    if(entity_node != NULL && VDEntity::get_singleton()->is_entity(entity_node)) {
      VDEntity::get_singleton()->unregister_entity(this);
    }
    if(new_entity != NULL) {
      VDEntity::get_singleton()->register_entity(this, new_entity);
    }
    entity_node = new_entity;
  }
}

void VDEntityNode::recheck() {
  Node* new_owner = NULL;
  if(entity_path.operator String() != "") {
    new_owner = get_node_or_null(entity_path);
  } else {
    new_owner = get_parent();
  }
  if(new_owner != entity_node) {
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
  if(this->composer.is_valid()) {
    return this->composer->get_aspect(name);
  }
  return false;
}

Ref<VDAspectData> VDEntityNode::get_data(StringName name) {
  if(this->composer.is_valid()) {
    return this->composer->get_data(name);
  }
  return false;
}

void VDEntityNode::set_entity_path(NodePath path) {
  if(entity_path != path) {
    entity_path = path;
    recheck();
  }
}

NodePath VDEntityNode::get_entity_path() {
  return entity_path;
}

Node * VDEntityNode::get_entity_node() {
  return entity_node;
}
