#ifndef VDASPECT_NODE_H
#define VDASPECT_NODE_H

#include "core/resource.h"
#include "core/script_language.h"
#include "modules/gdscript/gdscript.h"

#include "VDAspect.h"

class VDAspectNode : public Resource {
	GDCLASS(VDAspectNode, Resource);

    Ref<VDAspect> aspect;
    List<StringName> classes;

    Ref<VDAspectNode> parent;
    Vector< Ref<VDAspectNode> > childs;
  
    void init_node(Ref<VDAspect> aspect, HashMap< Ref<VDAspect>, Ref<VDAspectNode>, VariantHasher, VariantComparator > parents);

    String collect_classes(StringName class_name, List<StringName>& classes) const;

  protected:
    static void _bind_methods();

  public:
    VDAspectNode();

    void init(Ref<VDAspect> aspect);

    Ref<VDAspect> get_aspect() const;

    Ref<VDAspectNode> get_parent() const;

    Ref<VDAspectNode> get_child(int index) const;

    int get_child_size() const;

    List<StringName> get_classes() const;

};

/*
struct VariantHasher {
	static _FORCE_INLINE_ uint32_t hash(const Variant &p_variant) { return p_variant.hash(); }
};

struct VariantComparator {
	static _FORCE_INLINE_ bool compare(const Variant &p_lhs, const Variant &p_rhs) { return p_lhs.hash_compare(p_rhs); }
};*/

#endif
