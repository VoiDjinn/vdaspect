#ifndef VDASPECT_DATA_H
#define VDASPECT_DATA_H

#include "core/resource.h"
#include "VDAspectNode.h"

class VDAspectData : public Resource {
	GDCLASS(VDAspectData, Resource);

    int active_index;

    Vector< Ref<VDAspectNode> > nodes;

  protected:
    static void _bind_methods();

  public:
    VDAspectData();

    Ref<VDAspect> get_active();

    Ref<VDAspect> get_aspect(int index);

    int get_aspect_size();

    void set_active_index(int index);
    int get_active_index();

    Ref<VDAspectNode> get_node(int index);

    void add(Ref<VDAspectNode> node);
    bool remove(Ref<VDAspectNode> node);

    bool is_data_valid();

};

#endif
