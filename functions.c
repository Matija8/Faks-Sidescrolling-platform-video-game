#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "image.h"

		//podaci za materijal
	GLfloat light_position[] = { 1, 1, 1, 0 };
    GLfloat no_material[] = { 0, 0, 0, 1 };
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };
    GLfloat material_diffuse[] = { 0.1, 0.5, 0.8, 1 };
    GLfloat material_specular[] = { 1, 1, 1, 1 };
    GLfloat no_shininess[] = { 0 };
    GLfloat low_shininess[] = { 5 };
    GLfloat high_shininess[] = { 100 };
    GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };



void funcMakeBlock(GLuint names, float min_x, float max_x, float floor_y){

		//pravimo podlogu
		
		glBindTexture(GL_TEXTURE_2D, names);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y,-1);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y, 2);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y, 2);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y,-1);
    glEnd();
    
    	//pravimo prednju ivicu podloge
    	
    glBindTexture(GL_TEXTURE_2D, names);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y  , 2);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y-1, 2);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y-1, 2);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y  , 2);
    glEnd();
    
    	//leva ivica
    
    glBindTexture(GL_TEXTURE_2D, names);
    glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);

        glTexCoord2f(12, 6);
        glVertex3f(min_x,floor_y  , 2);

        glTexCoord2f(12, 0);
        glVertex3f(min_x,floor_y-1, 2);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y-1,-1);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y  ,-1);
    glEnd();
    
    	//desna ivica
    
    glBindTexture(GL_TEXTURE_2D, names);
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y  , 2);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y-1, 2);

        glTexCoord2f(0, 0);
        glVertex3f(max_x,floor_y-1,-1);

        glTexCoord2f(0, 6);
        glVertex3f(max_x,floor_y  ,-1);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);	//iskljucujemo aktivnu teksturu
}






void funcMakeBackground(GLuint name1, GLuint name2, float min_x, float max_x,
						 float min_y, float max_y, float far_z, float near_z){
		//pravimo pozadinu
		
		glBindTexture(GL_TEXTURE_2D, name1);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,max_y,far_z);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,min_y,far_z);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,min_y,far_z);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,max_y,far_z);
    glEnd();
    
    	//pravimo lavu
    	
    glBindTexture(GL_TEXTURE_2D, name2);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(12, 6);
        glVertex3f(max_x, min_y ,near_z);

        glTexCoord2f(12, 0);
        glVertex3f(max_x, min_y, far_z);

        glTexCoord2f(0, 0);
        glVertex3f(min_x, min_y, far_z);

        glTexCoord2f(0, 6);
        glVertex3f(min_x, min_y, near_z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);	//iskljucujemo aktivnu teksturu
}





		//Pravimo igraca

void funcMakePlayer(void){


	
	GLdouble head_radius=0.2;
	GLdouble body_height=0.4;		//Sirina tela je bh/2 = 0.2.
	
	
	glPushMatrix();
		glTranslatef(0, body_height/2, 0);
		
		
		glPushMatrix();		//glava
				glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
				glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
			glColor3f(0.8, 0.1, 0.1);
			glTranslatef(0 ,head_radius*2, 0);
			glutSolidSphere(head_radius, 50, 50);		
		glPopMatrix();
	
		
		glPushMatrix();		//telo
				glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
				glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
			glColor3f(0, 0.5, 1);
			glutSolidCube(body_height);		
		glPopMatrix();
	
		
		glPushMatrix();		//ruke
				glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
			glColor3f(0, 0, 1);
			glTranslatef(0, 0, -body_height);
			glScalef(1, 2, 1);
			glutSolidCube(body_height/3);		
		glPopMatrix();
		glPushMatrix();
				glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
			glTranslatef(0, 0, body_height);			
			glScalef(1, 2, 1);
			glutSolidCube(body_height/3);
		glPopMatrix();
		
		
		
	glPopMatrix();
	

	
	glColor3f(0,0,0);//todo ovo postaje lava
}




void nivo(float** podaci,int* br, float* min_floor, float* max_floor){
	
	FILE* f=fopen(".nivo.txt","r");
	
	if(f==NULL){
		fprintf(stderr,"fopen failed- functions.c line 241 %d\n",__LINE__);
		exit(EXIT_FAILURE);
	}
		
	int n;
	fscanf(f,"%d",&n);	//prvo vidimo broj podataka
	if(n<2){
		fprintf(stderr,"n<2- functions.c %d\n",__LINE__);
		exit(EXIT_FAILURE);
	}
	*br=n;	
	int fl=sizeof(float);
	
	if((*podaci=realloc(*podaci,3*n*fl))==NULL){		//min_x,max_x,floor
		fprintf(stderr,"realloc failed- functions.c %d\n",__LINE__);
		exit(EXIT_FAILURE);
	}
	
	
	
		//ucitavanje
		
	
	fscanf(f,"%f",&(*podaci)[0]);		//min_x
	fscanf(f,"%f",&(*podaci)[1]);		//max_x
	fscanf(f,"%f",&(*podaci)[2]);		//floor
	*min_floor=(*podaci)[2];
	*max_floor=(*podaci)[2];
		
	int i;
	for(i=1;i<n;i++){
		fscanf(f,"%f",&(*podaci)[i*3]);			//min_x
		fscanf(f,"%f",&(*podaci)[i*3+1]);		//max_x
		fscanf(f,"%f",&(*podaci)[i*3+2]);		//floor
		
		if((*podaci)[i*3+2]<*min_floor)
			*min_floor=(*podaci)[i*3+2];
			
		if((*podaci)[i*3+2]>*max_floor)
			*max_floor=(*podaci)[i*3+2];			
	}
	
	
	
	
	fclose(f);
	return;
}












void funcMakeFinishSign(GLuint name, float min_x, float max_x, float floor_y){
		
	glBindTexture(GL_TEXTURE_2D, name);
	
    glBegin(GL_QUADS);		//prednji deo
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y  , -0.5);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y, -0.5);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y+2, -0.5);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y+2  , -0.5);
    glEnd();
    
    
    glBegin(GL_QUADS);		//gornji deo
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y+2,	-0.5);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y+2,	-0.5);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y+2,	-1);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y+2,	-1);
    glEnd();
    
    
    glBegin(GL_QUADS);			//levi deo
        glNormal3f(-1, 0, 0);

        glTexCoord2f(12, 6);
        glVertex3f(min_x,floor_y,	-0.5);

        glTexCoord2f(12, 0);
        glVertex3f(min_x,floor_y+2, -0.5);

        glTexCoord2f(0, 0);
        glVertex3f(min_x,floor_y+2,	-1);

        glTexCoord2f(0, 6);
        glVertex3f(min_x,floor_y,	-1);
    glEnd();
    
    
    glBegin(GL_QUADS);			//desni deo
        glNormal3f(1, 0, 0);

        glTexCoord2f(12, 6);
        glVertex3f(max_x,floor_y, 	-0.5);

        glTexCoord2f(12, 0);
        glVertex3f(max_x,floor_y+2,	-0.5);

        glTexCoord2f(0, 0);
        glVertex3f(max_x,floor_y+2,	-1);

        glTexCoord2f(0, 6);
        glVertex3f(max_x,floor_y,	-1);
    glEnd(); 
    
    
    glBindTexture(GL_TEXTURE_2D, 0);	//iskljucujemo aktivnu teksturu	
}










