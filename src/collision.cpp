#include "typhon/collision.h"
#include "typhon/intersection.h"

using namespace typhon;

unsigned CollisionDetector::sphereXsphere(const ColSphere& first, const ColSphere& sec, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// Positions
	Vector3 posFirst = first.getAxis(3);
	Vector3 posSec = sec.getAxis(3);

	// Vecteur s�parant les 2 sph�res
	Vector3 between = posFirst - posSec;
	real magn = between.magnitude();

	// Si la s�paration des 2 sph�res est trop petite ou trop grande
	if (magn <= 0.0f || magn >= first.radius + sec.radius) return 0;

	// G�n�ration de la normale
	Vector3 normal = between * (((real)1.0) / magn);

	// G�n�ration du contact
	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->contactPoint = between * (real)0.5 + posFirst;
	contact->penetration = first.radius + sec.radius - magn;
	contact->setBodyData(first.body, sec.body, data->friction, data->restitution);

	// Mise � jour du pointeur
	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::sphereXhalfSpace(const ColSphere& sphere, const ColPlane& plane, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// Position de la sph�re
	Vector3 position = sphere.getAxis(3);

	// Distance de la sph�re p/r au plan
	real dist = plane.normal * position - sphere.radius;

	// Si la sph�re ne touche pas le plan
	if (dist >= plane.offset) return 0;

	// G�n�ration du contact
	Contact* contact = data->contacts;
	contact->contactNormal = plane.normal;
	// Point de contact p/r � l'intersection rayon-plan
	contact->contactPoint = position - plane.normal * (dist - sphere.radius);
	contact->penetration = -dist;
	contact->setBodyData(sphere.body, NULL, data->friction, data->restitution);

	// Mise � jour du pointeur
	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::boxXhalfSpace(const ColBox& box, const ColPlane& plane, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// S'il y a bien intersection
	if (!IntersectionTest::boxXhalfSpace(box, plane)) return 0;

	// Trouver le point d'intersection
	static real vertices[8][3] = {
		{1,1,1}, {-1,1,1}, {1,-1,1}, {-1,-1,1},
		{1,1,-1}, {-1,1,-1}, {1,-1,-1}, {-1,-1,-1} };

	Contact* contact = data->contacts;

	unsigned used = 0;
	for (unsigned i = 0; i < 8; i++) {
		// Positions de chaque sommet
		Vector3 vertexPos(vertices[i][0], vertices[i][1], vertices[i][2]);
		// Mise � l'�chelle
		vertexPos.componentProductUpdate(box.halfSize);
		// Transform�e dans le monde
		vertexPos = box.getTransform().transform(vertexPos);

		// Distance p/r au plan
		real dist = vertexPos * plane.normal;

		// Si le sommet est dans le plan
		if (dist <= plane.offset) {
			// G�n�ration du contact
			contact->contactNormal = plane.normal;
			// Point de contact � mis-chemin entre le sommet et le plan
			contact->contactPoint = vertexPos + plane.normal * (dist - plane.offset);
			contact->penetration = plane.offset - dist;

			contact->setBodyData(box.body, NULL, data->friction, data->restitution);

			// Prochain contact
			contact++;
			used++;
			if (used == data->contactsLeft) return used;
		}
	}

	data->addContacts(used);
	return used;
}