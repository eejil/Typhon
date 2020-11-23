#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time time = Time::getInstance();
Vector3 cam = world.myCar.body.getPosition(); 

enum TypeCam
{
	car1,
	car2,
	global,
};

TypeCam typeCam = global;

void DrawCar()
{
	//Voiture 1 -----------------------------------
	Matrix4 transform = world.myCar.body.getTransform();
	GLfloat gl_transform[16];
	transform.fillGLArray(gl_transform);
	glPushMatrix();
	glMultMatrixf(gl_transform);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
	//Voiture 1 -----------------------------------

	//Voiture 2 -----------------------------------
	Matrix4 transform2 = world.myCar2.body.getTransform();
	GLfloat gl_transform2[16];
	transform2.fillGLArray(gl_transform2);
	glPushMatrix();
	glMultMatrixf(gl_transform2);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
	//Voiture 2 -----------------------------------

}

void update() 
{
	world.startFrame();
	time.update();

	float duration = (float)time.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	//--------------------------------------------------------

	world.myCar.body.clearAccumulators();

	world.myCar.registry.updateForces(duration);

	world.myCar.body.integrate(duration);

	world.myCar2.body.clearAccumulators();

	world.myCar2.registry.updateForces(duration);

	world.myCar2.body.integrate(duration);

	if (abs(world.myCar.body.position.z + 1 - world.myCar2.body.position.z - 1) < 1)
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();

		world.myCar.body.setVelocity(0, 0, -world.myCar.body.getVelocity().z);
		world.myCar.body.setAcceleration(0, 0, -world.myCar.body.getAcceleration().z);

		world.myCar2.body.setVelocity(0, 0, -world.myCar2.body.getVelocity().z);
		world.myCar2.body.setAcceleration(0, 0, -world.myCar2.body.getAcceleration().z);


		ForceGenerator* carambolage = new Carambolage(world.myCar.body.getInverseInertiaTensor(), 0);
		world.myCar.registry.add(&world.myCar.body, carambolage);

		ForceGenerator* carambolage2 = new Carambolage(world.myCar2.body.getInverseInertiaTensor(), 1);
		world.myCar2.registry.add(&world.myCar2.body, carambolage2);
	}

	//--------------------------------------------------------

	world.runPhysics(duration);
	glutPostRedisplay();
}


void reshape(int w, int h) 
{
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	switch (typeCam)
	{
	case car1:
		gluLookAt(world.myCar.body.getPosition().x - 20, world.myCar.body.getPosition().y + 10, world.myCar.body.getPosition().z + 5 ,
			world.myCar.body.getPosition().x, world.myCar.body.getPosition().y, world.myCar.body.getPosition().z + 5,
			0.0, 1.0, 0.0);
		break;
	case car2:
		gluLookAt(world.myCar2.body.getPosition().x - 20, world.myCar2.body.getPosition().y + 10, world.myCar2.body.getPosition().z - 5 ,
			world.myCar2.body.getPosition().x, world.myCar2.body.getPosition().y, world.myCar2.body.getPosition().z + 5, 
			0.0, 1.0, 0.0);
		break;
	case global:
		gluLookAt( - 50, 30, 0 , 0, 0, 0, 0.0, 1.0, 0.0);
		break;

	default:
		break;
	}



	// Draw ground
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();

	glColor3f(0, 0, 0);
	DrawCar();

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void special(int key, int xx, int yy) 
{
	
}

void keyboard(unsigned char key, int x, int y) {
	std::cout << key;
	switch (key) {
		
	case 27: // Code ASCII de �chap
		exit(EXIT_SUCCESS);
		break;

	case 'z':
	{
		ForceGenerator* rotationCCW = new RotationCCW(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() - Vector3(0, 1, 1));
		world.myCar.registry.add(&world.myCar.body, rotationCCW);
		
	}
		break;

	case 's':
	{
	ForceGenerator* rotationCW = new RotationCW(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() - Vector3(0, 1, 1));
	world.myCar.registry.add(&world.myCar.body, rotationCW);
	}
		break;

	case 'n':
	{
		ForceGenerator* goRight = new Gravity(Vector3(0,0,1)*2);
		world.myCar.registry.add(&world.myCar.body, goRight);
		ForceGenerator* goLeft = new Gravity(Vector3(0,0,-1)*2);
		world.myCar2.registry.add(&world.myCar2.body, goLeft);
	}
	break;


	case 't':
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();

		world.myCar.body.setVelocity(0, 0, -world.myCar.body.getVelocity().z);
		world.myCar.body.setAcceleration(0, 0, -world.myCar.body.getAcceleration().z);

		world.myCar2.body.setVelocity(0, 0, -world.myCar2.body.getVelocity().z);
		world.myCar2.body.setAcceleration(0, 0, -world.myCar2.body.getAcceleration().z);


		ForceGenerator* carambolage = new Carambolage(world.myCar.body.getInverseInertiaTensor(), 0);
		world.myCar.registry.add(&world.myCar.body, carambolage);

		ForceGenerator* carambolage2 = new Carambolage(world.myCar2.body.getInverseInertiaTensor(), 1);
		world.myCar2.registry.add(&world.myCar2.body, carambolage2);
	}
		break;

	case 'm':
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();
	}
		break;

	case 'r':
	{
		world.myCar.reset(-20);
		world.myCar2.reset(20);
	}
		break;

	case '&':
		typeCam = car1;

		break;

	case '�':
		typeCam = car2;

		break;

	case '"':
		typeCam = global;

		break;

	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	time.init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("TP3");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glutMainLoop();
	return EXIT_SUCCESS;
}