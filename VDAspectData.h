#ifndef VDASPECT_DATA_H
#define VDASPECT_DATA_H

#include "core/resource.h"
#include "VDAspectNode.h"

class VDAspectComposer;

class VDAspectData : public Resource {
	GDCLASS(VDAspectData, Resource);

    int active_index;

    Vector< Ref<VDAspectNode> > nodes;

    friend class VDAspectComposer;
    void add_node(Ref<VDAspectNode> node);
    bool remove_node(Ref<VDAspectNode> node);

  protected:
    static void _bind_methods();

  public:
    VDAspectData();

    Ref<VDAspect> get_active() const;

    Ref<VDAspect> get_aspect(int index) const;

    int get_aspect_size() const;

    void set_active_index(int index);
    int get_active_index() const;

    Ref<VDAspectNode> get_node(int index) const;

    bool is_data_valid() const;

};

#endif
