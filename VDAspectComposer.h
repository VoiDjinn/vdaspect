#ifndef VDASPECT_COMPOSER_H
#define VDASPECT_COMPOSER_H

#include "core/resource.h"
#include "core/script_language.h"

#include "modules/gdscript/gdscript.h"

#include "VDAspect.h"
#include "VDAspectNode.h"
#include "VDAspectData.h"

class VDAspectComposer : public Resource {
  GDCLASS(VDAspectComposer, Resource);

    HashMap< Ref<VDAspect>, Ref<VDAspectNode>, VariantHasher, VariantComparator > roots;
    HashMap< StringName, Ref<VDAspectData> > aspects;
    List< Ref<VDAspect> > aspect_order;

    void reg(Ref<VDAspectNode> node);
    void unreg(Ref<VDAspectNode> node);

	protected:
		static void _bind_methods();

	public:
    VDAspectComposer();

    bool add(Ref<VDAspect> aspect);
    bool remove(Ref<VDAspect> aspect);

    bool has(StringName name);
    Ref<VDAspect> get(StringName name);
    Ref<VDAspectData> get_data(StringName name);

    void set_aspects_open(Array aspects);
    Array get_aspects_open() const;

    void set_aspects(List< Ref<VDAspect> > aspects);
    List< Ref<VDAspect> > get_aspects() const;

};

#endif
