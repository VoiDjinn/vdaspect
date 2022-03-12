#include "VDAspectNode.h"

VDAspectNode::VDAspectNode() {}


void VDAspectNode::_bind_methods() {

  ClassDB::bind_method(D_METHOD( "init", "aspect"     ), &VDAspectNode::init);
  ClassDB::bind_method(D_METHOD( "get_aspect"         ), &VDAspectNode::get_aspect);
  ClassDB::bind_method(D_METHOD( "get_parent"         ), &VDAspectNode::get_parent);
  ClassDB::bind_method(D_METHOD( "get_child", "index" ), &VDAspectNode::get_child);
  ClassDB::bind_method(D_METHOD( "get_child_size"     ), &VDAspectNode::get_child_size);
//  ClassDB::bind_method(D_METHOD( "get_classes"        ), &VDAspectNode::get_classes);

}


void VDAspectNode::init(Ref<VDAspect> aspect) {
  CRASH_COND_MSG(this->aspect.is_valid(), "VDAspectNode already in use.");
  HashMap< Ref<VDAspect>, Ref<VDAspectNode>, VariantHasher, VariantComparator > parents;
  init_node(aspect, parents);
}

void VDAspectNode::init_node(Ref<VDAspect> aspect, HashMap< Ref<VDAspect>, Ref<VDAspectNode>, VariantHasher, VariantComparator > parents) {
  //CRASH_COND_MSG(parents.has(aspect), "VDAspect already in parents.");
  this->aspect = aspect;
  parents.set(aspect, this);
  
  if (!(aspect->get_aspect_name() == "VDAspect")) { // Ignor VDAspect (fuer NULL-Object)
    String error_msg = collect_classes(aspect->get_aspect_name(), classes);
    if (!error_msg.empty()) {
      WARN_PRINT(error_msg);
    }
  }

  for (int i = 0; i < aspect->get_subaspect_size(); i++) {
    Ref<VDAspect> sub_aspect = aspect->get_subaspect(i);
    if(parents.has(sub_aspect)) {
      WARN_PRINT("Subaspect loop detected.");
      //childs.push_back(parents.get(sub_aspect));//TODO: Villeicht auskommentieren
      //TODO:Warn liste Doppelte Aspekte
    } else {
      Ref<VDAspectNode> sub_node = Ref<VDAspectNode>(memnew(VDAspectNode));
      sub_node->init_node(sub_aspect, parents);
      sub_node->parent = Ref<VDAspectNode>(this);
      childs.push_back(sub_node);
    }
  }
}

String VDAspectNode::collect_classes(StringName class_name, List<StringName>& classes) const {
  if (class_name == "Object") {
    return "Aspect is of type Object.";//Wenn Object, dann erbte nicht von VDAspect
  }
  classes.push_back(class_name);
  StringName parent_class_name;
  if (ClassDB::class_exists(class_name)) {
    parent_class_name = ClassDB::get_parent_class(class_name);
  } else if (ScriptServer::is_global_class(class_name)) {
    parent_class_name = ScriptServer::get_global_class_base(class_name);
  } else {
    return "Aspect is unknown: "+class_name;
  }
  if (parent_class_name == "VDAspect") {
    return "";// all OK
  } else {
    String result = collect_classes(parent_class_name, classes);// collect parent
    if (!result.empty()) {
      result = result + ", " + class_name;
    }
    return result;
  }
}

Ref<VDAspect> VDAspectNode::get_aspect() const {
  return aspect;
}

Ref<VDAspectNode> VDAspectNode::get_parent() const {
  return parent;
}

Ref<VDAspectNode> VDAspectNode::get_child(int index) const {
  return childs[index];
}

int VDAspectNode::get_child_size() const {
  return childs.size();
}

List<StringName> VDAspectNode::get_classes() const {
  return classes;
}
