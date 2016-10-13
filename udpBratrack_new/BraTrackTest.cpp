// BraTrackTest.cpp 
// Bratrack and OpenGL integration
// Author Jeronimo Grandi
// modified from Franscisco Pinto

#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include <string.h>
#include "BraTrackUDP.h"
using namespace std;
using namespace BraTrackUDP;

classInputProtocol udpReceiver;
vector<type_artifact> list_of_artifacts;



unsigned short int SERVICE_PORT;
unsigned int BUFFER_SIZE;


/*
m00 m01 m02 => m0 m3 m6
m10 m11 m12 => m1 m4 m7
m20 m21 m22 => m2 m5 m8
*/

// void quatToMatrix(float x,float y,float z,float w, double m[])
// {    

// 	double sqw = w*w;    
// 	double sqx = x*x;    
// 	double sqy = y*y;    
// 	double sqz = z*z;

// 	// invs (inverse square length) is only required if quaternion is not already normalised    
// 	double invs = 1 / (sqx + sqy + sqz + sqw);    
// 	m[0] = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs    
// 	m[4] = (-sqx + sqy - sqz + sqw)*invs ;    
// 	m[8] = (-sqx - sqy + sqz + sqw)*invs ;    

// 	double tmp1 = x*y;    
// 	double tmp2 = z*w;    
// 	m[1] = 2.0 * (tmp1 + tmp2)*invs ;    
// 	m[3] = 2.0 * (tmp1 - tmp2)*invs ;        

// 	tmp1 = x*z;    
// 	tmp2 = y*w;    
// 	m[2] = 2.0 * (tmp1 - tmp2)*invs ;    
// 	m[6] = 2.0 * (tmp1 + tmp2)*invs ;    
// 	tmp1 = y*z;    
// 	tmp2 = x*w;    
// 	m[5] = 2.0 * (tmp1 + tmp2)*invs ;    
// 	m[7] = 2.0 * (tmp1 - tmp2)*invs ;      
// }

//void callbackF( ot::CallbackNode & otnode, ot::State & event, void * data )
//{
//	cout << "CALLBACK: Data " << otnode.getName() << endl;
//	cout << "Buttons 0x" << hex << event.button << " ";
//	cout << "Pos x=" << event.position[0] << " ";
//	cout << "Pos y=" << event.position[1] << " ";
//	cout << "Pos z=" << event.position[2] << " ";
//	cout << "Ori i=" << event.orientation[0] << " ";
//	cout << "Ori j=" << event.orientation[1] << " ";
//	cout << "Ori k=" << event.orientation[2] << " ";
//	cout << "Ori s=" << event.orientation[3] << " ";
//
//	cout << endl;
//
//	quatToMatrix(event.orientation[0],event.orientation[1],event.orientation[2],event.orientation[3], list_of_artifacts[0].transform);
//
//	list_of_artifacts[0].transform[9]=event.position[0];
//	list_of_artifacts[0].transform[10]=event.position[1];
//	list_of_artifacts[0].transform[11]=event.position[2];
//
//	for (int a=0;a<12;++a)
//	{
//		cout << " " << list_of_artifacts[0].transform[a];
//
//	}
//	cout << endl;
//
//}

void idle(){
	
	unsigned long long int time_stamp;
	try{
		udpReceiver.receive_frame_not_blocking(time_stamp,list_of_artifacts);
	}
	catch(...){
		std::cout << "error" << std::endl;
	}
	glutPostRedisplay();
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);

	glColor3d(0.9,0.0,0.0);
	glVertex3d(-1500,-1500,0);
	glVertex3d(-1500,-1500,-5000);
	glVertex3d(-1500,1500,-5000);
	glVertex3d(-1500,1500,0);

	glColor3d(0.9,0.0,0.0);
	glVertex3d(1500,1500,0);
	glVertex3d(1500,1500,-5000);
	glVertex3d(1500,-1500,-5000);
	glVertex3d(1500,-1500,0);

	glColor3d(0.0,0.0,0.9);
	glVertex3d(-1500,-1500,0);
	glVertex3d(1500,-1500,0);
	glVertex3d(1500,-1500,-5000);
	glVertex3d(-1500,-1500,-5000);

	glColor3d(0.0,0.0,0.4);
	glVertex3d(-1500,1500,-5000);
	glVertex3d(1500,1500,-5000);
	glVertex3d(1500,1500,0);
	glVertex3d(-1500,1500,0);

	glColor3d(0.0,0.8,0.0);
	glVertex3d(-1500,-1500,-5000);
	glVertex3d(1500,-1500,-5000);
	glVertex3d(1500,1500,-5000);
	glVertex3d(-1500,1500,-5000);

	glEnd();

	glColor3d(0.5,1.0,0.8);
	glLineWidth(4.0);
	double transform[16];
	for(unsigned int i=0; i<list_of_artifacts.size(); i++){
		type_artifact & temp = list_of_artifacts[i];
		//std::cout << temp.id << std::endl;
		transform[0] = temp.transform[0];
		transform[1] = temp.transform[1];
		transform[2] = temp.transform[2];
		transform[3] = 0.0;
		transform[4] = temp.transform[3];
		transform[5] = temp.transform[4];
		transform[6] = temp.transform[5];
		transform[7] = 0.0;
		transform[8] = temp.transform[6];
		transform[9] = temp.transform[7];
		transform[10] = temp.transform[8];
		transform[11] = 0.0;
		transform[12] = temp.transform[9];
		transform[13] = temp.transform[10];
		transform[14] = temp.transform[11];
		transform[15] = 1.0;

		glPushMatrix();
		glTranslated(0.0,0.0,-2000.0);
		glMultMatrixd(transform);

		glBegin(GL_LINES);
		glColor3d(1.0,0.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(500.0,0.0,0.0);

		glColor3d(0.0,1.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,500.0,0.0);

		glColor3d(0.0,0.0,1.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,0.0,500.0);
		glEnd();
		glPopMatrix();
	}

	glutSwapBuffers();
}

void reshape(int w, int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,double(w)/double(h),250,5000);
}


int main(int argc, char **argv)
{

	BUFFER_SIZE = 1024;
	
	if (argc < 2)
	{
		printf("No port specified, running on port 3000 (default).\n");
		SERVICE_PORT = 3000;
	}
	else
		SERVICE_PORT = atoi(argv[1]);


	if(udpReceiver.init(SERVICE_PORT,BUFFER_SIZE))
		return 0;

	udpReceiver.set_package_limits(4); //maximum number of artifacts
	type_artifact art1;
	art1.id = "1";
	
	list_of_artifacts.push_back(art1);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,512);

	glutCreateWindow("UDP Bratrack Artifacts");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glClearColor(1.0,1.0,1.0,1.0);

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	
	return 0;
}

