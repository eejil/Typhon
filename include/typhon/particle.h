#ifndef TYPHON_PARTICLE_H
#define TYPHON_PARTICLE_H

#include "core.h"

namespace typhon {
	class Particle {
	protected:
		real inverseMass;
		real damping;

		Vector3 position;
		Vector3 velocity;

		Vector3 forceAccum;
		Vector3 acceleration;

		bool onGround = true;
		bool onWater = false;

		int indexBuo = 0;


	public:
		void integrate(real duration);

		void setMass(const real mass);
		real getMass() const;
		void setInverseMass(const real inverseMass);
		real getInverseMass() const;

		bool hasFiniteMass() const;

		void setDamping(const real damping);
		real getDamping() const;

		void setPosition(const Vector3& position);
		void setPosition(const real x, const real y, const real z);

		void getPosition(Vector3* position) const;
		Vector3 getPosition() const;

		void setVelocity(const Vector3& velocity);
		void setVelocity(const real x, const real y, const real z);

		void getVelocity(Vector3* velocity) const;
		Vector3 getVelocity() const;

		void setAcceleration(const Vector3& acceleration);
		void setAcceleration(const real x, const real y, const real z);
		void getAcceleration(Vector3* acceleration) const;
		Vector3 getAcceleration() const;

		void clearAccumulator();
		void addForce(const Vector3& force);

		void setFlaque();
		void setFlaque(bool flaque);
		bool getFlaque();

		void setGround();
		void setGround(bool ground);
		bool getGround();

		void setIndexBuo(int indexbuo);
		int getIndexBuo();

	};
}
#endif  TYPHON_PARTICLE_H