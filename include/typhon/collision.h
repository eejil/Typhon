#ifndef TYPHON_COLLISION_H
#define TYPHON_COLLISION_H

#include "contacts.h"

namespace typhon {
	// Primitive des collisions
	class ColPrimitive
	{
	public:
		// Le rigidbody associ� � la collision
		RigidBody* body;
		// La matrice d'offset � partir de la matrice du rigidbody
		Matrix4 offset;

		Vector3 getAxis(unsigned i) const {
			return transform.getAxisVector(i);
		}

		const Matrix4& getTransform()const {
			return transform;
		}

	private:
		Matrix4 transform;
	};

	// Primitive sph�rique
	class ColSphere : public ColPrimitive
	{
	public:
		// Rayon de la sph�re
		real radius;
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
	};

	// D�tecteur de collision contenant des m�thodes de g�n�ration de "collision data"
	// typiques entre primitives
	class CollisionDetector
	{
	public:
		// Collision entre 2 sph�res
		unsigned sphereXsphere(const ColSphere& first, const ColSphere& sec, CollisionData* data);
		// Collision entre sph�re et semi plan
		unsigned sphereXhalfSpace(const ColSphere& sphere, const ColPlane& plane, CollisionData* data);
		// Collision entre bo�te et semi plan
		unsigned boxXhalfSpace(const ColBox& box, const ColPlane& plane, CollisionData* data);
	};
}

#endif TYPHON_COLLISION_H
