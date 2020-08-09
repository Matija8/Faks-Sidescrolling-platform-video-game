#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "image.h"
#include "functions.h"

#include "constants.h"

static GLuint names[4];            //teksture.
static float x_cam=0;            //pozicija kamere i igraca na x osi.
static float dx_cam=0.1;        //promena pozicije igraca po x osi u jedinici vremena.
static float y_cam=0.3;            //pozicija kamere po y osi.
//static float dy_cam=0.05;        //promena pozicije kamere po y osi u jedinici vremena. Debug mod.
static float y_jump;            //promena pozicije igraca na y osi usled skoka.
static float current_floor;        //vrednost y-koordinate podloge ispod igraca(ne nuzno poda).
static float lava_floor;
static int animation_ongoing;
static float y_speed=JUMP_SPEED;        //brzina skoka.
static float g=0.07;                //gravitacija.
static float t=0;                //vreme
static int movement_flag=0;        //flag za kretnju
static int jump_flag=0;            //flag za skok
static int fall_flag=0;            //flag za pad
static float jump_begin_y;        //y koordinata pozicije igraca na pocetku skoka
static int death_flag=0;
float* podaci;                    //informacije o podlozi
static float min_floor;
static float max_floor;
static int d=0;
static int n;                    //broj podloga
static int d_change_flag=1;
static float current_right_x;    //pozicije ivica trenutnog bloka
static float current_left_x;
static float next_left_x;        //pozicije ivica proslih blokova
static float last_right_x;
static float next_floor;        //vrednost y-koordinate sledece podloge
static float last_floor;

//funkcije

static void initialize_lights(void);
static void initialize_textures(void);
static void on_display(void);
static void on_timer(int value);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);





int main(int argc, char** argv){
    glutInit(&argc,argv);    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutCreateWindow("Game Running");
    glutFullScreen();
            
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    
    podaci=malloc(sizeof(double));    
    nivo(&podaci,&n,&min_floor,&max_floor);
    y_jump=podaci[2];        //Igrac pocinje na visini prve podloge.
    animation_ongoing = 0;
    
    
    initialize_textures();
    initialize_lights();
    glClearColor(0.75,0.75,0.75,0);
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 0;
}





static void on_keyboard(unsigned char key, int x, int y){

    switch (key) {
    case 27:
        free(podaci);
        printf("escape pressed\n");
        exit(EXIT_SUCCESS);
        break;
    
    case 'A':    
    case 'a':
        movement_flag=-1;
        if (!animation_ongoing){
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        glutPostRedisplay();
        break;
        
    case 'D':
    case 'd':
        movement_flag=1;
        if (!animation_ongoing){
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        glutPostRedisplay();
        break;
    
    case 'W':
    case 'w':
        if(fall_flag!=1){
            jump_flag=1;
            t=0;
            jump_begin_y=current_floor;
            if (!animation_ongoing){
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                animation_ongoing = 1;
            }
        }
        break;
    
    case 'S':
    case 's':
        movement_flag=0;
        glutPostRedisplay();
        break;
    
    /*case 'I':        //Debug mod.
    case 'i':
        y_cam=y_cam+dy_cam;
        glutPostRedisplay();
        break;
    
    case 'K':    
    case 'k':
        y_cam=y_cam-dy_cam;
        glutPostRedisplay();
        break;*/
    
    }
}


static void on_timer(int value){  
    
    if (value != TIMER_ID){
        return;
    }
    
    if (y_jump <= lava_floor){                            //Game over.
        printf("GAME OVER! Play again?\n");
        free(podaci);
        exit(EXIT_SUCCESS);
    }
    
    
    if (movement_flag==1){                                //Kretnja levo, desno.
        x_cam=x_cam+dx_cam;
    }    
    if (movement_flag==-1){
        x_cam=x_cam-dx_cam;
    }
    
    
    
    if (jump_flag!=1 && (death_flag!=0 || y_jump>current_floor)){            //Propadanje. TODO: Bag. Propadanje je lose preko podloga.
        
        fall_flag=1;
        jump_flag=0;
        t=t+0.2;
        y_speed=g*t;        
        if(y_speed>TERMINAL_VELOCITY){
            y_speed=TERMINAL_VELOCITY;
        }
        y_jump=y_jump-y_speed;
        
    }    
    else{
        fall_flag=0;
        t=0;
    }
    
    if (jump_flag==1 && fall_flag!=1){                                        //skok
        t=t+0.2;
        y_speed=y_speed-g*t;
        y_jump=y_jump+y_speed*t;
        if(y_speed<=0){
            fall_flag=1;
            jump_flag=0;
            t=0;
        }
    }
    else{
        if(death_flag==0 && y_jump<=current_floor){
            fall_flag=0;
            jump_flag=0;
            t=0;
            y_jump=current_floor;
            y_speed=JUMP_SPEED;
            
        }
    }
    
    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}


static void on_reshape(int width, int height){        //Kod preuzet sa vezbi asistenta Ivana Cukica. Primer Cube.

    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 60);
    glMatrixMode(GL_MODELVIEW);
}



static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

        //Podesavamo kameru.
    
    if(y_cam+1<y_jump){
        y_cam=y_jump-1;
    }
    if(y_cam-1>y_jump){
        y_cam=y_jump+1;
    }
        
    gluLookAt(
            x_cam, 2+y_cam, 10,
            x_cam, y_cam, 0,
                0, 1, 0
    );
    
    
    
    
    
    
    
        //Pravimo podloge.    
            
    int i;
    for(i=0;i<n;i++){
        funcMakeBlock(names[1],podaci[3*i],podaci[3*i+1],podaci[3*i+2]);
        if(i==n-1){
            funcMakeFinishSign(names[0],podaci[3*i],podaci[3*i+1],podaci[3*i+2]);
        }    
    }
    
    
        //inicijalizacija podloge;
    
    if(d>=0 && d<n && d_change_flag==1){
        if(d==n-1){
            last_right_x=current_right_x;
            current_left_x=next_left_x;
            current_right_x=podaci[3*d+1];
            current_floor=podaci[3*d+2];
            next_left_x=current_right_x+1000;
            
            d_change_flag=0;
        }
        else{
            next_left_x=podaci[3*(d+1)];
            current_left_x=podaci[3*d];
            current_right_x=podaci[3*d+1];
            current_floor=podaci[3*d+2];
            next_floor=podaci[3*(d+1)+2];
        
            if(d==0){
                last_right_x=current_left_x-10;
                last_floor=lava_floor;
            }
            else{
                last_right_x=podaci[3*(d-1)+1];
                last_floor=podaci[3*(d-1)+2];
            }
            
            d_change_flag=0;
            
        }
        /*printf("promena vred: floor %.0f, last edge %.0f, left %.0f, right %.0f, next edge %.0f\n",
            current_floor, last_right_x, current_left_x, current_right_x, next_left_x);*/
    }
    
    
    
    
    
    
    
    
    
        // provere propadanja.Leva ivica igraca je x_cam-0.2.Desna ivica igraca je x_cam-0.2.
        
        
        
        //Na trenutnoj podlozi smo.
    
    if(x_cam+0.2>=current_left_x && x_cam-0.2<=current_right_x && y_jump >= current_floor){    
        death_flag=0;
    }
    
        
        //Propadamo levo.
        
    if(x_cam+0.2<current_left_x && x_cam-0.2>last_right_x){                
        death_flag=1;
    }    
    
        //Propadamo desno.
        
    if(x_cam-0.2>current_right_x && x_cam+0.2<next_left_x){                
        death_flag=1;
            
    }
    
        //provera da li se zakucavamo u trenutnu podlogu kad propadamo.
        
    if(x_cam+0.2>=current_left_x && x_cam-0.2<=current_right_x && y_jump<current_floor){
            
        if(y_jump>current_floor-1.8){
        
            if(movement_flag==1 && x_cam<current_left_x+0.2)
                x_cam=current_left_x-0.2;
                
            if(movement_flag==-1 && x_cam>current_right_x-0.2)
                x_cam=current_right_x+0.2;
        }
    }
    
    
    
        //Stigli smo do sledece podloge.
    
    if(x_cam+0.2>=next_left_x && y_jump>=next_floor){            
        d_change_flag=1;
        d++;
        //printf("next (floor%d)\n",d);
        glutPostRedisplay();        
    }
    if(x_cam+0.2>=next_left_x && y_jump<next_floor){
        if(y_jump>next_floor-1.8){                        //Igrac je visok 0.8. Podloga je visine 1.
            x_cam=next_left_x-0.2;
        }
    }
    
    
        //Vratili smo se na prethodnu podlogu.
    
    if(x_cam-0.2<=last_right_x && y_jump>=last_floor){
        d_change_flag=1;
        d--;
        //printf("previous (floor%d)\n",d);
        glutPostRedisplay();
    }
    if(x_cam-0.2<=last_right_x && y_jump<last_floor){
        if(y_jump>last_floor-1.8){
            x_cam=last_right_x+0.2;
        }
    }
    
    
        //Stigli smo na kraj
    
        
    if(d==(n-1) && y_jump==current_floor){
        free(podaci);
        printf("You WIN!\n");
        exit(EXIT_SUCCESS);
    }    
    
    
        //Model igraca.
    

    
    glPushMatrix();        
        glTranslatef(x_cam,y_jump,1);
        funcMakePlayer();
    glPopMatrix();
    
    
    
        //Dodajemo pozadinu.    
    
    
    lava_floor=min_floor-0.5;
    
    funcMakeBackground(names[2], names[3], podaci[0]-20, podaci[3*n-2]+30, lava_floor, max_floor+10, -6, 8);
    
    
    
    glutSwapBuffers();
    
}















    //Teksture. Namerno odvojene od ostatka koda.
    //kod preuzet sa vezbi asistenta Ivana Cukica i modifikovan.

static void initialize_textures(void)                
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    
    /* Generisu se identifikatori tekstura. */
    glGenTextures(4, names);
    
    
    /* Kreira se tekstura zid. */
    image_read(image, BMP_WALL);
    
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se tekstura trava. */
    image_read(image, BMP_GRASS);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se tekstura pozadina. */
    image_read(image, BMP_BACKGROUND);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Kreira se tekstura lava. */
    image_read(image, BMP_LAVA);

    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

    //Osvetljenje. Namerno odvojene od ostatka koda.
    //kod preuzet sa vezbi asistenta Ivana Cukica i modifikovan.

static void initialize_lights(void){
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 1, 10, 5, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    
    
}





