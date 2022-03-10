#ifndef VDASPECT_H
#define VDASPECT_H

#include "core/resource.h"
#include "scene/main/node.h"

class VDAspectComposer;
class VDEntityNode;

class VDAspect : public Resource {
	GDCLASS(VDAspect, Resource);

  protected:
    static void _bind_methods();

    Vector< Ref<VDAspect> > subaspects;

  public:
    VDAspect();

    bool add_subaspect(Ref<VDAspect> aspect);
    bool remove_subaspect(Ref<VDAspect> aspect);
    Ref<VDAspect> get_subaspect(int index);
    int get_subaspect_size();

    StringName get_aspect_name();
};

class VDAspectGlobal : public Object {
	GDCLASS(VDAspectGlobal, Object);

	static VDAspectGlobal *singleton;

	HashMap<ObjectID, ObjectID> registered_nodes;
protected:
	static void _bind_methods();

public:
    VDAspectGlobal();

	static _FORCE_INLINE_ VDAspectGlobal *get_singleton() { return singleton; }

	bool register_entity(VDEntityNode* entity, Node* node = nullptr);
	bool unregister_entity(VDEntityNode* entity);
	Ref<VDAspectComposer> from_node(Node* node);
	Ref<VDAspectComposer> from_entity(VDEntityNode* entity);
	Ref<VDAspectComposer> from_entity_open(Node* entity);
	bool is_entity(Node* node);
};

#endif
