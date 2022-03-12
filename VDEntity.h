#ifndef VDENTITY_H
#define VDENTITY_H

#include "VDAspectComposer.h"
#include "VDEntityNode.h"

#include "scene/main/node.h"

class VDEntity : public Object {
  GDCLASS(VDEntity, Object);

  static VDEntity *singleton;

  HashMap<ObjectID, ObjectID> registered_nodes;
protected:
  static void _bind_methods();

public:
    VDEntity();

    static _FORCE_INLINE_ VDEntity *get_singleton() { return singleton; }

    bool register_entity(VDEntityNode* entity, Node* node = nullptr);
    bool unregister_entity(VDEntityNode* entity);

    Ref<VDAspect> get_aspect(Node* node, StringName aspect_name);
    Ref<VDAspectComposer> get_composer(Node* node);
    VDEntityNode* get_entity(Node* node);
    bool is_entity(Node* node);

    List<Node*> get_entities_with(StringName aspect_name);

};

#endif
