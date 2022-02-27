#include "VDEntityNode.h"

VDEntityNode::VDEntityNode() {}

void VDEntityNode::_bind_methods()
{

  ClassDB::bind_method(D_METHOD("set_composer", "composer"), &VDEntityNode::set_composer);
  ClassDB::bind_method(D_METHOD("get_composer"               ), &VDEntityNode::get_composer);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "aspects", PROPERTY_HINT_RESOURCE_TYPE, "VDAspectComposer"), "set_composer", "get_composer");

}

void VDEntityNode::set_composer(Ref<VDAspectComposer> composer)
{
  this->composer = composer;
}

Ref<VDAspectComposer> VDEntityNode::get_composer()
{
  return this->composer;
}
