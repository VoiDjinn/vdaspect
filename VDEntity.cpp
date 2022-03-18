#include "VDEntity.h"

/// Singleton
VDEntity *VDEntity::singleton = nullptr;

VDEntity::VDEntity() {
  ERR_FAIL_COND_MSG(singleton, "Singleton in VDEntity already exist.");
  singleton = this;
}

void VDEntity::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_aspect",   "node", "aspect_name"), &VDEntity::get_aspect);
  ClassDB::bind_method(D_METHOD("get_composer", "node"),                &VDEntity::get_composer);
  ClassDB::bind_method(D_METHOD("get_entity",   "node"),                &VDEntity::get_entity);
  ClassDB::bind_method(D_METHOD("is_entity",    "node"),                &VDEntity::is_entity);
  //ClassDB::bind_method(D_METHOD("get_entities_with", "aspect_name"),                &VDEntity::get_entities_with);
}

bool VDEntity::register_entity(VDEntityNode* entity, Node* node) {
  ERR_FAIL_COND_V_MSG(entity == nullptr, false, "Entity is null.");
  Node* registering_node = nullptr;
  if(node == nullptr) {
    registering_node = entity->get_tagged_node();
  } else {
    registering_node = node;
  }
  ERR_FAIL_COND_V_MSG(registering_node == nullptr, false, "To be registered node is null.");
  ObjectID reg_id = registering_node->get_instance_id();
  ObjectID ent_id = entity->get_instance_id();
  registered_nodes.set(reg_id, ent_id);
  return true;
}

bool VDEntity::unregister_entity(VDEntityNode* entity) {
  ERR_FAIL_COND_V_MSG(entity == nullptr, false, "Entity is null.");
  Node* tagged_node = entity->get_tagged_node();
  if(tagged_node != nullptr && registered_nodes.has(tagged_node->get_instance_id())) {
    registered_nodes.remove(tagged_node->get_instance_id());
    return true;
  }
  return false;
}

Ref<VDAspect> VDEntity::get_aspect(Node* node, StringName aspect_name) {
  Ref<VDAspectComposer> composer = get_composer(node);
  if(composer.is_valid()) {
    return composer->get_aspect(aspect_name);
  }
  return nullptr;
}

Ref<VDAspectComposer> VDEntity::get_composer(Node* node) {
  VDEntityNode* entity_node = get_entity(node);
  if(entity_node != nullptr) {
    return entity_node->get_composer();
  }
  return nullptr;
}

VDEntityNode* VDEntity::get_entity(Node* node) {
  ERR_FAIL_COND_V_MSG(node == nullptr, nullptr, "Node is null.");
  ObjectID id;
  if(registered_nodes.lookup(node->get_instance_id(), id)) {
    Object* entity_instance = ObjectDB::get_instance(id);
    ERR_FAIL_COND_V_MSG(entity_instance == nullptr, nullptr, "Entity-instance is null.");
    VDEntityNode* entity_node = Object::cast_to<VDEntityNode>(entity_instance);
    ERR_FAIL_COND_V_MSG(entity_node == nullptr, nullptr, "Instance is not convertible to VDEntityNode.");
    return entity_node;
  }
  return nullptr;
}

bool VDEntity::is_entity(Node* node) {
  if(node == nullptr) {
    return false;
  }
  return registered_nodes.has(node->get_instance_id());
}

List<Node*> VDEntity::get_entities_with(StringName aspect_name) {
  List<Node*> results;
  for (OAHashMap<ObjectID, ObjectID>::Iterator it = registered_nodes.iter(); it.valid; registered_nodes.next_iter(it) ) {
    Ref<VDAspectComposer> composer = Object::cast_to<VDEntityNode>(ObjectDB::get_instance(*(it.value)))->get_composer();
    if(composer != nullptr && composer->has_aspect(aspect_name)) {
      results.push_back(Object::cast_to<Node>(ObjectDB::get_instance(*(it.key))));
    }
  }

  // int size = registered_nodes.size();
  // const HashMap<ObjectID, ObjectID>::Pair* pairs = new HashMap<ObjectID, ObjectID>::Pair[size];
  // registered_nodes.get_key_value_ptr_array(&pairs);
  // for(int i = 0; i < size; i++) {
  //   Ref<VDAspectComposer> composer = Object::cast_to<VDEntityNode>(ObjectDB::get_instance(pairs[i].data))->get_composer();
  //   if(composer != nullptr && composer->has_aspect(aspect_name)) {
  //     results.push_back(Object::cast_to<Node>(ObjectDB::get_instance(pairs[i].key)));
  //   }
  // }
  // delete pairs;
  return results;
}
