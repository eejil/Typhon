#ifndef TYPHON_COLLISION_H
#define TYPHON_COLLISION_H

#include "contacts.h"

#include <iostream>

namespace typhon {

	struct BoundingSphere
	{
		Vector3 centre;
		real radius;

	public:
		BoundingSphere();

		BoundingSphere(const Vector3& centre, real radius);

		BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);

		bool overlaps(const BoundingSphere* other) const;
		
		bool overlaps(const BoundingSphere* first, const BoundingSphere* sec) const;

		real getGrowth(const BoundingSphere& other) const;

		real getSize() const
		{
			return ((real)1.333333) * 3.14 * radius * radius * radius;
		}
	};

	// Primitive des collisions
	class ColPrimitive
	{
	public:
		// Le rigidbody associ� � la collision
		RigidBody body;
		// La matrice d'offset � partir de la matrice du rigidbody
		Matrix4 offset;

		Vector3 getAxis(unsigned i) const {
			return transform.getAxisVector(i);
		}

		const Matrix4& getTransform()const {
			return body.getTransform();
		}

	private:
		Matrix4 transform;
	};

	class ColPlane :public ColPrimitive
	{
	public:
		// Normale du plan
		Vector3 normal;
		// Offset p/r � l'origine
		real offset;
	};

	class ColBox :public ColPrimitive
	{
	public:
		// Moiti� de dimensions permettant de retrouver tous les sommets
		Vector3 halfSize;
	};

	// Structure contenant les informations essentielles lors de collision(s)
	struct CollisionData
	{
		// Premier contact de la liste (sert d'acc�s)
		Contact* firstContact;
		// Tableau des contacts
		Contact* contacts;
		// Nombre de contacts restant
		int contactsLeft;
		// Nombre total de contacts enregistr�s
		unsigned contactCount;
		// Valeur de friction sortant d'une collision
		real friction;
		// Valeur de restitution sortant d'une collision
		real restitution;

		// Reste-t-il des contacts � g�rer
		bool hasContactsLeft() { return contactsLeft > 0; }
		// Ajoute � la structure une quantit� donn�e de contact
		void addContacts(unsigned nb) {
			// Mise � jour des donn�es
			contactsLeft -= nb;
			contactCount += nb;

			// D�placement dans le tableau
			contacts += nb;
		}

		void reset(unsigned maxContacts)
		{
			contactsLeft = maxContacts;
			contactCount = 0;
			contacts = firstContact;
		}
	};

	// D�tecteur de collision contenant des m�thodes de g�n�ration de "collision data"
	// typiques entre primitives
	class CollisionDetector
	{
	public:
		// Collision entre bo�te et semi plan
		unsigned boxXhalfSpace(const ColBox& box, const ColPlane& plane, CollisionData* data);
	};
}

#endif TYPHON_COLLISION_H
