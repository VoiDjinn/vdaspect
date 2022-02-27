#ifndef VDASPECT_H
#define VDASPECT_H

#include "core/resource.h"

class VDAspect : public Resource {
	GDCLASS(VDAspect, Resource);

  protected:
    static void _bind_methods();

    Vector< Ref<VDAspect> > subaspects;

  public:
    VDAspect();

    Ref<VDAspect> get_subaspect(int index);
    int get_subaspect_size();

    StringName get_aspect_name();

    bool add(Ref<VDAspect> aspect);
    bool remove(Ref<VDAspect> aspect);

};

#endif
