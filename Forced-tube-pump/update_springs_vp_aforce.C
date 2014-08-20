#include "update_springs_vp_aforce.h"
#include <ibamr/IBSpringForceSpec.h>

void
update_springs_vp_aforce(
	       tbox::Pointer<hier::PatchHierarchy<NDIM> > hierarchy,
	       LDataManager* const l_data_manager,
	       const double current_time,
	       const double dt)
{
  static const double pi = 4*atan(1);
  const int finest_ln = hierarchy->getFinestLevelNumber();
  double freq = 1.0;
  double theta=freq*current_time*2*pi - pi/2;

 
  // Find out the Lagrangian index ranges. 
  const std::pair<int,int>& muscle_lag_idxs = l_data_manager->getLagrangianStructureIndexRange(0, finest_ln);
    
  // Get the LMesh (which we assume to be associated with the finest level of
  // the patch hierarchy).  Note that we currently need to update both "local"
  // and "ghost" node data.
  Pointer<LMesh> mesh = l_data_manager->getLMesh(finest_ln);
  vector<LNode*> nodes;
  nodes.insert(nodes.end(), mesh->getLocalNodes().begin(), mesh->getLocalNodes().end());
  nodes.insert(nodes.end(), mesh->getGhostNodes().begin(), mesh->getGhostNodes().end());

  // Update the spring specs.
  tbox::Pointer<hier::PatchLevel<NDIM> > level = hierarchy->getPatchLevel(finest_ln);
  for (vector<LNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      LNode* node_idx = *it;
      IBSpringForceSpec* spring_spec = node_idx->getNodeDataItem<IBSpringForceSpec>();
		
      if (spring_spec == NULL) continue;  // skip to next node

      // Here we update the multiplier that determines the magnitude of the force applied to the boundary
      //
      // NOTES: lag_idx      is the "index" of the Lagrangian point (lag_idx = 0, 1, ..., N-1, where N is the number of Lagrangian points)

      const int lag_idx = node_idx->getLagrangianIndex();
      double& phase = spring_spec->getParameters()[0][1];


      if (muscle_lag_idxs.first <= lag_idx && lag_idx < muscle_lag_idxs.second)
	{
	  phase=0.5*(1+sin(theta));
	}
    }

return;
}// update_springs
