#ifndef VDENTITY_NODE_H
#define VDENTITY_NODE_H

#include "VDAspectComposer.h"
#include "scene/main/node.h"

class VDAspectGlobal;

class VDEntityNode : public Node {
	GDCLASS(VDEntityNode, Node);

	friend class VDAspectGlobal;

	Ref<VDAspectComposer> composer;
	Node* entity_node;
	NodePath entity_path;

	void reregister(Node* new_entity);
	void recheck();

  protected:
    static void _bind_methods();
	void _notification(int p_notification);

  public:
    VDEntityNode();

    void set_composer(Ref<VDAspectComposer> composer);
    Ref<VDAspectComposer> get_composer();

    bool add_aspect(Ref<VDAspect> aspect);
    bool remove_aspect(Ref<VDAspect> aspect);

    bool has_aspect(StringName name);
    Ref<VDAspect> get_aspect(StringName name);
    Ref<VDAspectData> get_data(StringName name);

	  void set_entity_path(NodePath path);
	  NodePath get_entity_path();
	  Node* get_entity_node();

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
