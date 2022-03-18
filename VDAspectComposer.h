#ifndef VDASPECT_COMPOSER_H
#define VDASPECT_COMPOSER_H

#include "core/resource.h"
#include "core/oa_hash_map.h"
#include "core/script_language.h"

#include "modules/gdscript/gdscript.h"

#include "VDAspect.h"
#include "VDAspectNode.h"
#include "VDAspectData.h"

class VDAspectComposer : public Resource {
  GDCLASS(VDAspectComposer, Resource);

    OAHashMap< Ref<VDAspect>, Ref<VDAspectNode>, VariantHasher, VariantComparator > roots;
    OAHashMap< StringName, Ref<VDAspectData> > aspects;
    List< Ref<VDAspect> > aspect_order;

    void reg_node(Ref<VDAspectNode> node);
    void unreg_node(Ref<VDAspectNode> node);

	protected:
		static void _bind_methods();

	public:
    VDAspectComposer();

    bool add_aspect(Ref<VDAspect> aspect);
    bool remove_aspect(Ref<VDAspect> aspect);

    bool has_aspect(StringName name);
    Ref<VDAspect> get_aspect(StringName name) const;
    Ref<VDAspectData> get_data(StringName name) const;

    void set_aspects_open(Array aspects);
    Array get_aspects_open() const;

    void set_aspects(List< Ref<VDAspect> > aspects);
    List< Ref<VDAspect> > get_aspects() const;

  public:
    template<class T>
    _FORCE_INLINE_ void add(Ref<T> ref) {
        add_aspect(Object::cast_to<VDAspect>(ref.ptr()));
    }

    template<class T>
    _FORCE_INLINE_ Ref<T> remove() {
      return Object::cast_to<T>(remove_aspect(T::get_class_static()));
    }

    template<class T>
    _FORCE_INLINE_ bool has() const {
        return has_aspect(T::get_class_static());
    }

    template<class T>
    _FORCE_INLINE_ Ref<T> get() const {
        return get_aspect(T::get_class_static());
    }

    template<class T>
    _FORCE_INLINE_ operator T*() const {
        return *get_aspect(T::get_class_static());
    }

};

#endif
