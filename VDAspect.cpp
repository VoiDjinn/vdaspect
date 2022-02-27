#include "VDAspect.h"

VDAspect::VDAspect() {}

void VDAspect::_bind_methods()
{

  ClassDB::bind_method(D_METHOD( "get_subaspect", "index" ), &VDAspect::get_subaspect);
  ClassDB::bind_method(D_METHOD( "get_subaspect_size"     ), &VDAspect::get_subaspect_size);
  ClassDB::bind_method(D_METHOD( "get_aspect_name"        ), &VDAspect::get_aspect_name);

  ClassDB::bind_method(D_METHOD( "add_sub_aspect",    "aspect" ), &VDAspect::add);
  ClassDB::bind_method(D_METHOD( "remove_sub_aspect", "aspect" ), &VDAspect::remove);

}

Ref<VDAspect> VDAspect::get_subaspect(int index)
{
  return subaspects[index];
}

int VDAspect::get_subaspect_size()
{
  return subaspects.size();
}

StringName VDAspect::get_aspect_name()
{
  return StringName(call("get_class")); //get_class_name();
}

bool VDAspect::add(Ref<VDAspect> aspect)
{
  if ( aspect.is_valid() && subaspects.find(aspect) < 0 )
  {
    subaspects.push_back(aspect);
    return true;
  }
  return false;
}

bool VDAspect::remove(Ref<VDAspect> aspect)
{
  int index = subaspects.find(aspect);
  if ( index >= 0 )
  {
    subaspects.remove(index);
    return true;
  }
  return false;
}
