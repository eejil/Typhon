#ifndef TYPHON_FGEN_H
#define TYPHON_FGEN_H

#include "rigidBody.h"
#include "particleForceGen.h"
#include <vector>

namespace typhon {

	// G�n�rateur de forces pour les corps
	class ForceGenerator
	{
	public:
		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

	class ForceRegistry
	{
	public:
		struct ForceRegistration {
			RigidBody* body;
			ForceGenerator* fg;
		};

	public:
		typedef std::vector<ForceRegistration> Registry;
		Registry registrations;

	public:
		// Ajoute une entr�e
		void add(RigidBody* body, ForceGenerator* fg);

		void updateForces(real duration);
	};

	class Gravity : public ForceGenerator
	{
	private:
		Vector3 gravity;

	public:
		// Constructeur
		Gravity(const Vector3& gravity) : gravity(gravity) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class Spring : public ForceGenerator
	{
	private:
		Vector3 connectionPoint;
		Vector3 otherConnectionPoint;

		RigidBody* other;

		real springConstant;
		real restLength;

	public:
		// Constructeur
		Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength)
			: connectionPoint(localConnectionPt), otherConnectionPoint(otherConnectionPt),
			other(other), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class RotationCW : public ForceGenerator
	{
	private:
		Matrix3 tensor;

		Vector3 position;

		Quaternion orientation;
	public:

		RotationCW(const Matrix3 tensor, const Vector3 position) : tensor(tensor), position(position) {}

		virtual void updateForce(RigidBody* body, real duration);
	};
}

#endif	TYPHON_FGEN_H