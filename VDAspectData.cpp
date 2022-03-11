#include "VDAspectData.h"

VDAspectData::VDAspectData() {}

void VDAspectData::_bind_methods()
{

  ClassDB::bind_method(D_METHOD( "get_active" ), &VDAspectData::get_active);

  ClassDB::bind_method(D_METHOD( "get_aspect", "index" ), &VDAspectData::get_aspect);
  ClassDB::bind_method(D_METHOD( "get_aspect_size"     ), &VDAspectData::get_aspect_size);

  ClassDB::bind_method(D_METHOD( "set_active_index", "index" ), &VDAspectData::set_active_index);
  ClassDB::bind_method(D_METHOD( "get_active_index"          ), &VDAspectData::get_active_index);

  ClassDB::bind_method(D_METHOD( "get_node", "index" ), &VDAspectData::get_node);

  ClassDB::bind_method(D_METHOD( "add",    "node" ), &VDAspectData::add);
  ClassDB::bind_method(D_METHOD( "remove", "node" ), &VDAspectData::remove);

  ClassDB::bind_method(D_METHOD( "is_data_valid" ), &VDAspectData::is_data_valid);

}

Ref<VDAspect> VDAspectData::get_active() {
  Ref<VDAspect> result;
  if (active_index >= 0) {
    result = get_aspect(active_index);
  }
  return result;
}

Ref<VDAspect> VDAspectData::get_aspect(int index) {
  return get_node(index)->get_aspect();
}

int VDAspectData::get_aspect_size() {
  return nodes.size();
}

void VDAspectData::set_active_index(int index) {
  if (index < 0) {
    ERR_PRINT("new active index < 0");
  }
  if (index >= get_aspect_size()) {
    ERR_PRINT("new active index out of bounds");
  } else {
    active_index = index;
  }
}

int VDAspectData::get_active_index() {
  return active_index;
}

Ref<VDAspectNode> VDAspectData::get_node(int index) {
  return nodes[index];
}

void VDAspectData::add(Ref<VDAspectNode> node) {
  nodes.push_back(node);
  if (active_index == -1 && nodes.size() > 0) {
    active_index = 0;
  }
}

bool VDAspectData::remove(Ref<VDAspectNode> node) {
  int index = nodes.find(node);
  if (index >= 0) {
    nodes.remove(index);
    if (index <= active_index) {
      active_index-=1;
    }
    if (active_index == -1 && nodes.size() > 0) {
      active_index = 0;
    }
    return true;
  }
  else return false;
}

bool VDAspectData::is_data_valid() {
  return active_index > -1;
}
