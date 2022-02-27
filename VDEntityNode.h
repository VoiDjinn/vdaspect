#ifndef VDENTITY_NODE_H
#define VDENTITY_NODE_H

#include "scene/main/node.h"

#include "VDAspectComposer.h"

class VDEntityNode : public Node {
	GDCLASS(VDEntityNode, Node);

  Ref<VDAspectComposer> composer;

  protected:
    static void _bind_methods();

  public:
    VDEntityNode();

    void set_composer(Ref<VDAspectComposer> composer);
    Ref<VDAspectComposer> get_composer();

};

#endif
