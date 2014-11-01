#ifndef included_update_target_point_positions_peri
#define included_update_target_point_positions_peri

#include <ibamr/app_namespaces.h>
#include <ibtk/LDataManager.h>

/*
 * Update the positions of the target point specifications.
 */
void
update_target_point_positions_peri(
    Pointer<PatchHierarchy<NDIM> > hierarchy,
    LDataManager* l_data_manager,
    const double current_time,
    const double dt);

#endif //#ifndef included_update_target_point_positions_peri

