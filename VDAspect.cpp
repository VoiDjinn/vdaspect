#include "VDAspect.h"
#include "VDAspectComposer.h"
#include "VDEntityNode.h"

VDAspect::VDAspect() {}

void VDAspect::_bind_methods() {
  ClassDB::bind_method(D_METHOD("add_subaspect",    "aspect"), &VDAspect::add_subaspect);
  ClassDB::bind_method(D_METHOD("remove_subaspect", "aspect"), &VDAspect::remove_subaspect);
  ClassDB::bind_method(D_METHOD("get_subaspect", "index"), &VDAspect::get_subaspect);
  ClassDB::bind_method(D_METHOD("get_subaspect_size"), &VDAspect::get_subaspect_size);
  ClassDB::bind_method(D_METHOD("get_aspect_name"), &VDAspect::get_aspect_name);
}

Ref<VDAspect> VDAspect::get_subaspect(int index) {
  return subaspects[index];
}

int VDAspect::get_subaspect_size() {
  return subaspects.size();
}

StringName VDAspect::get_aspect_name() {
  String s_class_name = call("get_class"); //get_class_name();
  return StringName(s_class_name);
}

bool VDAspect::add_subaspect(Ref<VDAspect> aspect) {
  if(aspect.is_valid() && subaspects.find(aspect) < 0) {
    subaspects.push_back(aspect);
    return true;
  }
  return false;
}

bool VDAspect::remove_subaspect(Ref<VDAspect> aspect) {
  int index = subaspects.find(aspect);
  if(index >= 0) {
    subaspects.remove(index);
    return true;
  }
  return false;
}

/// Singleton
VDAspectGlobal *VDAspectGlobal::singleton = nullptr;

VDAspectGlobal::VDAspectGlobal() {
  ERR_FAIL_COND_MSG(singleton, "Singleton in VDAspectGlobal already exist.");
  singleton = this;
}

void VDAspectGlobal::_bind_methods() {
	ClassDB::bind_method(D_METHOD("from_node", "node"), &VDAspectGlobal::from_node);
	ClassDB::bind_method(D_METHOD("from_entity", "entity"), &VDAspectGlobal::from_entity_open);
	ClassDB::bind_method(D_METHOD("is_entity", "node"), &VDAspectGlobal::is_entity);
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
