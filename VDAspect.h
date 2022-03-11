#ifndef VDASPECT_H
#define VDASPECT_H

#include "core/resource.h"
#include "scene/main/node.h"

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

#endif
