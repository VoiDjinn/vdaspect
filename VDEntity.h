#ifndef VDENTITY_H
#define VDENTITY_H

#include "core/oa_hash_map.h"

#include "VDAspectComposer.h"
#include "VDEntityNode.h"

#include "scene/main/node.h"

class VDEntity : public Object {
    GDCLASS(VDEntity, Object);

    static VDEntity *singleton;

    OAHashMap<ObjectID, ObjectID> registered_nodes;
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

  public:
    template<class A>
    _FORCE_INLINE_ static bool add(Ref<VDAspectComposer>& composer, Ref<A>& ref) {
      return composer -> add_aspect ( Object::cast_to <VDAspect> (ref.ptr()) );
    }

    template<class A>
    _FORCE_INLINE_ static bool remove(Ref<VDAspectComposer>& composer, Ref<A>& ref) {
      return composer -> remove_aspect ( Object::cast_to <VDAspect> (ref.ptr()) );
    }

    template<class A>
    _FORCE_INLINE_ static bool has(Ref<VDAspectComposer>& composer) {
      return composer -> has_aspect ( A::get_class_static() );
    }

    template<class A>
    _FORCE_INLINE_ static Ref<A> get(Ref<VDAspectComposer>& composer) {
      return composer -> get_aspect ( A::get_class_static() );
    }

    template<class A>
    _FORCE_INLINE_ static Ref<A> lookup(Ref<VDAspectComposer>& composer, Ref<A>& ref) {
      ref = composer -> get_aspect ( A::get_class_static() );
      return ref;
    }

  public:
    template<class A>
    _FORCE_INLINE_ static bool add(Node* node, Ref<A>& ref) {
      return add <A> (singleton->get_composer(node), ref);
    }

    template<class A>
    _FORCE_INLINE_ static bool remove(Node* node, Ref<A>& ref) {
      return remove <A> (singleton->get_composer(node), ref);
    }

    template<class A>
    _FORCE_INLINE_ static bool has(Node* node) {
      return has <A> (singleton->get_composer(node));
    }

    template<class A>
    _FORCE_INLINE_ static Ref<A> get(Node* node) {
      return get <A> (singleton->get_composer(node));
    }

    template<class A>
    _FORCE_INLINE_ static Ref<A> lookup(Node* node, Ref<A>& ref) {
      return lookup <A> (singleton->get_composer(node), ref);
    }

};

#endif
