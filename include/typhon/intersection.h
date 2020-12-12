#ifndef TYPHON_INTERSECTION_H
#define TYPHON_INTERSECTION_H

#include "collision.h"

namespace typhon {
	class IntersectionTest
	{
	public:
		// Transform�e d'une bo�te sur un axe (renvoie le rayon de la bo�te dnas la direction de l'axe)
		static real transformToAxis(const ColBox& box, const Vector3& axis);
		
		// Test d'intersection bo�te-plan
		static bool boxXhalfSpace(const ColBox& box, const ColPlane& plane);
	};
}

#endif TYPHON_INTERSECTION_H
