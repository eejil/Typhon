#ifndef TYPHON_WORLD_H
#define TYPHON_WORLD_H

#include "body.h"
#include "forceGen.h"

namespace typhon {

	class World
	{
	public:
		struct BodyRegistration
		{
			RigidBody* body;
			BodyRegistration* next;
		};

		BodyRegistration* firstBody;

		void startFrame();

		void runPhysics(real duration);
	};
}

#endif	TYPHON_WORLD_H