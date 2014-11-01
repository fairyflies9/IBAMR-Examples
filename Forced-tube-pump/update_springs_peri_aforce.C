#include "update_springs_peri_aforce.h"
#include <ibamr/IBSpringForceSpec.h>

void
update_springs_peri_aforce(
	       tbox::Pointer<hier::PatchHierarchy<NDIM> > hierarchy,
	       LDataManager* const l_data_manager,
	       const double current_time,
	       const double dt)
{
  static const double pi = 4*atan(1);
  const int finest_ln = hierarchy->getFinestLevelNumber();
  int N = 512;		//finest grid spacing
  double lperi = 0.3;	//length of peristaltic section (m)
  double ds = 1.0/(2.0*N);	//space between boundary points (m)
  int Nperi = ceil(lperi/ds);	//number peristalsis springs
  double kappa_spring = 1.0; // spring stiffness used in generate mesh
  double kmax = (kappa_spring/10)*ds/(ds*ds);  // maximum spring stiffness for muscles 

  double freq = 1.0;
  double period = 1.0/freq;	//period of oscillation (s)
  double ww = 0.005;	//width of the Gaussian wave packet (m)
  double speed = 1.3*(lperi)/period;	//wave speed (m/s)
  int pulse_number = floor(current_time/period);		//number of pulses thus far
  double pulse_time = current_time-pulse_number*period;		//time since beginning of current pulse
  double xc = speed*pulse_time-0.15*lperi;          //center of wave packet

  // Find out the Lagrangian index ranges. 
  const std::pair<int,int>& muscle_lag_idxs = l_data_manager->getLagrangianStructureIndexRange(0, finest_ln);
    
  // Get the LMesh (which we assume to be associated with the finest level of
  // the patch hierarchy).  Note that we currently need to update both "local"
  // and "ghost" node data.
  Pointer<LMesh> mesh = l_data_manager->getLMesh(finest_ln);
  vector<LNode*> nodes;
  nodes.insert(nodes.end(), mesh->getLocalNodes().begin(), mesh->getLocalNodes().end());
  nodes.insert(nodes.end(), mesh->getGhostNodes().begin(), mesh->getGhostNodes().end());

  // Update the spring lengths in their associated spring specs.
  tbox::Pointer<hier::PatchLevel<NDIM> > level = hierarchy->getPatchLevel(finest_ln);
  for (vector<LNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      LNode* node_idx = *it;
      IBSpringForceSpec* spring_spec = node_idx->getNodeDataItem<IBSpringForceSpec>();
		
      if (spring_spec == NULL) continue;  // skip to next node

      // Here we update the multiplier that determines the magnitude of the force
      //
      // NOTES: lag_idx      is the "index" of the Lagrangian point (lag_idx = 0, 1, ..., N-1, where N is the number of Lagrangian points)

      const int lag_idx = node_idx->getLagrangianIndex();
      double& phase = spring_spec->getParameters()[0][0];
     

    if (muscle_lag_idxs.first <= lag_idx && lag_idx < muscle_lag_idxs.second)
	{
	  //phase = 0.0;
	  //for (int i=0; i<pulse_number+1; i++) { 
	    double xp = (lag_idx-muscle_lag_idxs.first)*ds;
	    //phase+=exp(-(xp-(xc+speed*i*period))*(xp-(xc+speed*i*period))/(ww*ww));
	    phase=kmax*exp(-(xp-(xc))*(xp-(xc))/(ww*ww));
	    // }
	}
    }

return;
}// update_springs
