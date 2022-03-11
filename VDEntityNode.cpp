#include "VDEntityNode.h"

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
    if(entity_node != NULL && VDAspectGlobal::get_singleton()->is_entity(entity_node)) {
      VDAspectGlobal::get_singleton()->unregister_entity(this);
    }
    if(new_entity != NULL) {
      VDAspectGlobal::get_singleton()->register_entity(this, new_entity);
    }
    entity_node = NULL;
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

/// Singleton
VDAspectGlobal *VDAspectGlobal::singleton = nullptr;

VDAspectGlobal::VDAspectGlobal() {
  ERR_FAIL_COND_MSG(singleton, "Singleton in VDAspectGlobal already exist.");
  singleton = this;
}

void VDAspectGlobal::_bind_methods() {
	ClassDB::bind_method(D_METHOD("from_node", "node"),     &VDAspectGlobal::from_node);
	ClassDB::bind_method(D_METHOD("from_entity", "entity"), &VDAspectGlobal::from_entity_open);
	ClassDB::bind_method(D_METHOD("is_entity", "node"),     &VDAspectGlobal::is_entity);
}

bool VDAspectGlobal::register_entity(VDEntityNode* entity, Node* node) {
  ERR_FAIL_COND_V_MSG(entity == nullptr, false, "Entity is null.");
  Node* registering_node = nullptr;
  if(node == nullptr) {
    registering_node = entity->get_entity_node();
  } else {
    registering_node = node;
  }
  ERR_FAIL_COND_V_MSG(registering_node == nullptr, false, "To be registered node is null.");
  ObjectID reg_id = registering_node->get_instance_id();
  ObjectID ent_id = entity->get_instance_id();
  registered_nodes.set(reg_id, ent_id);
  return true;
}

bool VDAspectGlobal::unregister_entity(VDEntityNode* entity) {
  ERR_FAIL_COND_V_MSG(entity == nullptr, false, "Entity is null.");
  if(is_entity(entity)) {
    return registered_nodes.erase(entity->get_instance_id());
  }
  return false;
}

Ref<VDAspectComposer> VDAspectGlobal::from_node(Node* node) {
  Ref<VDAspectComposer> result;
  ERR_FAIL_COND_V_MSG(node == nullptr, result, "Node is null.");
  if(is_entity(node)) {
    Object* entity_instance = ObjectDB::get_instance(node->get_instance_id());
    ERR_FAIL_COND_V_MSG(entity_instance == nullptr, result, "Entity-instance is null.");
    VDEntityNode* entity_node = Object::cast_to<VDEntityNode>(entity_instance);
    ERR_FAIL_COND_V_MSG(entity_node == nullptr, result, "Instance is not convertible to VDEntityNode.");
    result = entity_node->get_composer();
  }
  return result;
}

Ref<VDAspectComposer> VDAspectGlobal::from_entity(VDEntityNode* entity) {
  Ref<VDAspectComposer> result;
  ERR_FAIL_COND_V_MSG(entity == nullptr, result, "Entity is null.");
  result = from_node(entity->get_entity_node());
  return result;
}

Ref<VDAspectComposer> VDAspectGlobal::from_entity_open(Node* entity) {
  Ref<VDAspectComposer> result;
  ERR_FAIL_COND_V_MSG(entity == nullptr, result, "Node is null.");
  VDEntityNode* entity_node = Object::cast_to<VDEntityNode>(entity);
  ERR_FAIL_COND_V_MSG(entity_node == nullptr, result, "Node is not of type VDEntityNode.");
  result = from_entity(entity_node);
  return result;
}

bool VDAspectGlobal::is_entity(Node* node) {
  if(node == nullptr) {
    return false;
  }
  return registered_nodes.has(node->get_instance_id());
}
