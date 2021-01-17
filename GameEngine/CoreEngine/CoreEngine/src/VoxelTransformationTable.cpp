#include "VoxelTriangulation.h"

namespace Engine
{
	void VoxelTriangulation::InitializeTransformations()
	{
		const int left = 0;
		const int right = 1;
		const int down = 2;
		const int up = 3;
		const int front = 4;
		const int back = 5;

		Axis[0].Set(-1,  0,  0); // left
		Axis[1].Set( 1,  0,  0); // right
		Axis[2].Set( 0, -1,  0); // down
		Axis[3].Set( 0,  1,  0); // up
		Axis[4].Set( 0,  0,  1); // front
		Axis[5].Set( 0,  0, -1); // back

		Transformations.push_back(TransformationData( right,	up,		front )); // Euler___0P___0Y___0R
		Transformations.push_back(TransformationData( up,		left,	front )); // Euler___0P___0Y__90R
		Transformations.push_back(TransformationData( left,		down,	front )); // Euler___0P___0Y_180R
		Transformations.push_back(TransformationData( down,		right,	front )); // Euler___0P___0Y_270R
		
		Transformations.push_back(TransformationData( back,		up,		right )); // Euler___0P__90Y___0R
		Transformations.push_back(TransformationData( up,		front,	right )); // Euler___0P__90Y__90R
		Transformations.push_back(TransformationData( front,	down,	right )); // Euler___0P__90Y_180R
		Transformations.push_back(TransformationData( down,		back,	right )); // Euler___0P__90Y_270R
		
		Transformations.push_back(TransformationData( left,		up,		back )); // Euler___0P_180Y___0R
		Transformations.push_back(TransformationData( up,		right,	back )); // Euler___0P_180Y__90R
		Transformations.push_back(TransformationData( right,	down,	back )); // Euler___0P_180Y_180R
		Transformations.push_back(TransformationData( down,		left,	back )); // Euler___0P_180Y_270R
		
		Transformations.push_back(TransformationData( front,	up,		left )); // Euler___0P_270Y___0R
		Transformations.push_back(TransformationData( up,		back,	left )); // Euler___0P_270Y__90R
		Transformations.push_back(TransformationData( back,		down,	left )); // Euler___0P_270Y_180R
		Transformations.push_back(TransformationData( down,		front,	left )); // Euler___0P_270Y_270R
		
		Transformations.push_back(TransformationData( right,	front,	down )); // Euler__90P___0Y___0R
		Transformations.push_back(TransformationData( front,	left,	down )); // Euler__90P___0Y__90R
		Transformations.push_back(TransformationData( left,		back,	down )); // Euler__90P___0Y_180R
		Transformations.push_back(TransformationData( back,		right,	down )); // Euler__90P___0Y_270R
		
		Transformations.push_back(TransformationData( right,	back,	up ));	 // Euler_270P___0Y___0R
		Transformations.push_back(TransformationData( back,		left,	up ));	 // Euler_270P___0Y__90R
		Transformations.push_back(TransformationData( left,		front,	up ));	 // Euler_270P___0Y_180R
		Transformations.push_back(TransformationData( front,	right,	up ));	 // Euler_270P___0Y_270R
	}												  
}