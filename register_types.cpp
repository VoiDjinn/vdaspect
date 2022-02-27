#include "register_types.h"
#include "core/class_db.h"
#include "core/engine.h"

#include "VDAspect.h"
#include "VDAspectNode.h"
#include "VDAspectData.h"
#include "VDAspectComposer.h"
#include "VDEntityNode.h"

void register_vdaspect_types() {
//  ClassDB::register_virtual_class<VDAspect>();
  ClassDB::register_class<VDAspect>();
  ClassDB::register_class<VDAspectNode>();
  ClassDB::register_class<VDAspectData>();
  ClassDB::register_class<VDAspectComposer>();
  ClassDB::register_class<VDEntityNode>();
}

void unregister_vdaspect_types() {
}