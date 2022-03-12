#include "VDAspectComposer.h"

VDAspectComposer::VDAspectComposer() {}

void VDAspectComposer::_bind_methods() {

  ClassDB::bind_method(D_METHOD( "add_aspect",    "aspect" ), &VDAspectComposer::add_aspect);
  ClassDB::bind_method(D_METHOD( "remove_aspect", "aspect" ), &VDAspectComposer::remove_aspect);

  ClassDB::bind_method(D_METHOD( "has_aspect", "name" ), &VDAspectComposer::has_aspect);
  ClassDB::bind_method(D_METHOD( "get_aspect", "name" ), &VDAspectComposer::get_aspect);

  ClassDB::bind_method(D_METHOD( "get_data", "name" ), &VDAspectComposer::get_data);

  ClassDB::bind_method(D_METHOD("set_aspects", "new_aspects"), &VDAspectComposer::set_aspects_open);
  ClassDB::bind_method(D_METHOD("get_aspects"               ), &VDAspectComposer::get_aspects_open);

  ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "aspects", PROPERTY_HINT_TYPE_STRING, "17/17:VDAspect"), "set_aspects", "get_aspects");

  ADD_SIGNAL(MethodInfo("aspect_added", PropertyInfo(Variant::OBJECT, "added_aspect", PROPERTY_HINT_RESOURCE_TYPE, "VDAspect")));
  ADD_SIGNAL(MethodInfo("aspect_removed", PropertyInfo(Variant::OBJECT, "removed_aspect", PROPERTY_HINT_RESOURCE_TYPE, "VDAspect")));
  ADD_SIGNAL(MethodInfo("aspects_changed"));
}

void VDAspectComposer::reg_node(Ref<VDAspectNode> node) {
  List<StringName> classes = node->get_classes();
  for (List<StringName>::Element* ele = classes.front(); ele; ele = ele->next()) {
    StringName name = ele->get();
    Ref<VDAspectData> data;
    if (aspects.has(name)) {
      data = aspects.get(name);
    } else {
      data = Ref<VDAspectData>(memnew(VDAspectData));
      aspects.set(name, data);
    }
    data->add_node(node);
  }
  for (int i = 0; i < node->get_child_size(); i++) {
    reg_node(node->get_child(i));
  }
}

void VDAspectComposer::unreg_node(Ref<VDAspectNode> node) {
  List<StringName> classes = node->get_classes();
  for (List<StringName>::Element* ele = classes.front(); ele; ele = ele->next()) {
    StringName name = ele->get();
    if (aspects.has(name)) {
      Ref<VDAspectData> data = aspects.get(name);
      data->remove_node(node);
      if (data->get_aspect_size() == 0) {
        aspects.erase(name);
      }
    } else {
      WARN_PRINT("aspects invalid");
    }
  }
  for (int i = 0; i < node->get_child_size(); i++) {
    unreg_node(node->get_child(i));
  }
}


bool VDAspectComposer::add_aspect(Ref<VDAspect> aspect) {
  if (!roots.has(aspect)) {
    Ref<VDAspectNode> node = Ref<VDAspectNode>(memnew(VDAspectNode));
    node->init(aspect);
    roots.set(aspect, node);
    aspect_order.push_back(aspect);
    reg_node(node);
    emit_signal("aspect_added", aspect);
    //property_list_changed_notify();
    return true;
  }
  return false;
}

bool VDAspectComposer::remove_aspect(Ref<VDAspect> aspect) {
  Ref<VDAspectNode> node = roots.get(aspect);
  if (node.is_valid()) {
    unreg_node(node);
    aspect_order.erase(aspect);
    roots.erase(aspect);
    emit_signal("aspect_removed", aspect);
    //property_list_changed_notify();
    return true;
  }
  return false;
}

bool VDAspectComposer::has_aspect(StringName name) {
  return aspects.has(name);
}

Ref<VDAspect> VDAspectComposer::get_aspect(StringName name) const {
  Ref<VDAspectData> data = get_data(name);
  if (data.is_valid()) {
    return data->get_active();
  } else {
    return nullptr;
  }
}

Ref<VDAspectData> VDAspectComposer::get_data(StringName name) const {
  Ref<VDAspectData> data;
  if(aspects.has(name)){
    data = aspects.get(name); // TODO: issue for get_default or get_or_null_ref
  }
  return data;
}

void VDAspectComposer::set_aspects_open(Array aspects) {
  List< Ref<VDAspect> > new_aspects;
  for(int i = 0; i < aspects.size(); i++) {
    Ref<VDAspect> aspect = aspects[i];
    if(aspect.is_null()) {
      aspect = Ref<VDAspect> (memnew(VDAspect));
    }
    new_aspects.push_back(aspect);
  }
  set_aspects(new_aspects);
}

Array VDAspectComposer::get_aspects_open() const {
  //List< Ref<VDAspect> > aspects = get_aspects();
  Array open_aspects;
  for (const List< Ref<VDAspect> >::Element* ele = aspect_order.front(); ele; ele = ele->next()) {
    open_aspects.push_back(ele->get());
  }
  return open_aspects;
}

void VDAspectComposer::set_aspects(List< Ref<VDAspect> > aspects) {
  //List< Ref<VDAspect> > old_aspects = get_aspects();
  int added = 0;
  int removed = 0;
  for (const List< Ref<VDAspect> >::Element* ele = aspects.front(); ele; ele = ele->next()) {
    Ref<VDAspect> new_aspect = ele->get();
    if (!roots.has(new_aspect)) {
      add_aspect(new_aspect);
      added++;
    }
  }
  for (const List< Ref<VDAspect> >::Element* ele = aspect_order.front(); ele; ele = ele->next()) {
    Ref<VDAspect> old_aspect = ele->get();
    if(aspects.find(old_aspect) == nullptr) {
      remove_aspect(old_aspect);
      removed++;
    }
  }
  if(removed > 0 || added > 0) {
    emit_signal("aspects_changed");
    property_list_changed_notify();
  }
}

List< Ref<VDAspect> > VDAspectComposer::get_aspects() const {
//  List< Ref<VDAspect> > keys;
//  roots.get_key_list(&keys);
//  return keys;
  return aspect_order;
}
