#include "update_target_point_positions.h"
#include <ibamr/IBTargetPointForceSpec.h>

void
update_target_point_positions(
    tbox::Pointer<hier::PatchHierarchy<NDIM> > hierarchy,
    LDataManager* const l_data_manager,
    const double current_time,
    const double dt)
{
    const int finest_ln = hierarchy->getFinestLevelNumber();

    static const double pi = 4*atan(1);
    static const double V = 0.2; //velocity of the wing during translation (meters/sec)
	
	////////////////////////////////////////////////////////////////////////////////////////
	// these parameters require modification to match the desired geometry and motion
	
    static const double L1 = 1; // length of computational domain (meters)
    static const int N1 = 512; // number of cartesian grid meshwidths at the finest level of the AMR grid
	static const double diameter = 0.1;			// diameter of tube
	static const double R2 = 0.1;				// distance from middle of domain to inner wall
	static const double R1 = R2+diameter;		// distance from middle of domain to outer wall
	static const double pamp = 0.8;				//percent occlusion of the tube
	static const double amp = pamp*diameter/2.0;	//amplitude of contraction of each piece of the actuator
	static const double freq = 1.0;
 
 ////////////////////////////////////////////////////////////////////////////////////////////////

    // Find out the Lagrangian index ranges.
    const std::pair<int,int>& actuator_top_idxs = l_data_manager->getLagrangianStructureIndexRange(0, finest_ln);
	const std::pair<int,int>& actuator_bot_idxs = l_data_manager->getLagrangianStructureIndexRange(1, finest_ln);
	
    // Get the LMesh (which we assume to be associated with the finest level of
    // the patch hierarchy).  Note that we currently need to update both "local"
    // and "ghost" node data.
    Pointer<LMesh> mesh = l_data_manager->getLMesh(finest_ln);
    vector<LNode*> nodes;
    nodes.insert(nodes.end(), mesh->getLocalNodes().begin(), mesh->getLocalNodes().end());
    nodes.insert(nodes.end(), mesh->getGhostNodes().begin(), mesh->getGhostNodes().end());

    // Update the target point positions in their associated target point force
    // specs.
    tbox::Pointer<hier::PatchLevel<NDIM> > level = hierarchy->getPatchLevel(finest_ln);
    for (vector<LNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        LNode* node_idx = *it;
        IBTargetPointForceSpec* force_spec = node_idx->getNodeDataItem<IBTargetPointForceSpec>();
        if (force_spec == NULL) continue;  // skip to next node

        // Here we update the position of the target point.
        //
        // NOTES: lag_idx      is the "index" of the Lagrangian point (lag_idx = 0, 1, ..., N-1, where N is the number of Lagrangian points)
        //        X_target     is the target position of the target point
        //        X_target[0]  is the x component of the target position
        //        X_target[1]  is the y component of the target position
        //        X_target[2]  is the z component of the target position (for a 3D simulation)
        //
        // The target position is shifted to the left or right by the
        // increment dt*V

        const int lag_idx = node_idx->getLagrangianIndex();

	//Depending on the version of IBAMR, you need to select one of the ways of accessing target point positions
    //TinyVector<double,NDIM>& X_target = force_spec->getTargetPointPosition();
	//IBTK::Vector<double,NDIM>& X_target = force_spec->getTargetPointPosition();
	Point& X_target = force_spec->getTargetPointPosition();
	
		//move the top piece
	    if (actuator_top_idxs.first <= lag_idx && lag_idx < actuator_top_idxs.second)
	      {
				X_target[1] = -R2-(amp/2)*(1+sin(2*pi*freq*current_time - pi/2));
	      }
		//move the bottom piece
		if (actuator_bot_idxs.first <= lag_idx && lag_idx < actuator_bot_idxs.second)
	      {
				X_target[1] = -R1+(amp/2)*(1+sin(2*pi*freq*current_time - pi/2));
	      }
    }
    return;
}// update_target_point_positions
