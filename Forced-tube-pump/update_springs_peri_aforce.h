#ifndef included_update_springs_peri_aforce
#define included_update_springs_peri_aforce

#include <ibamr/app_namespaces.h>
#include <ibtk/LDataManager.h>

/*
 * Update the positions of the target point specifications.
 */
void
update_springs_peri_aforce(
    Pointer<PatchHierarchy<NDIM> > hierarchy,
    LDataManager* l_data_manager,
    const double current_time,
    const double dt);

#endif //#ifndef included_update_target_point_positions
