#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include <string>
#include "../Test/BmpLoader.h"
// lx -0.000000 ly 0.087156 lz -0.996195
// centerx 1.000000 centery 1.087156 centerz 9.003805
// upx -0.000000 upy 0.996195 upz 0.087156
double Txval=0,Tyval=0,Tzval=0, arodure=60;
double eyex = 1;
double eyey = 1;
double eyez = 10;
double centerx = 1.0;
double centery = 1.087156;
double centerz = 9.003805;
double upx = 0;
double upy = 1;
double upz = 0;
double myval = 0;
double pi = 3.1416;
int idx = 0, k = 0;
double roll = 0, pitch = 0, yaw = 0;
int score = 0;
GLfloat roadInc = -40;
GLfloat zMove = 0, xMove = 0;
unsigned int numOfTexs = 8;
unsigned int * textures;
double windowHeight=1200, windowWidth=800;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;
GLboolean lt1 = true, lt2 = true;
bool visited[500] = { false };
bool carStart = false;
bool isGameOver = false;
bool isWelcomeDisplay = true;
bool isBirdEye = false;
static GLfloat cube[8][3] =
{
    {0.0, 0.0, 0.0},    //0
    {0.0, 0.0, 2.0},    //1
    {2.0, 0.0, 2.0},    //2
    {2.0, 0.0, 0.0},    //3
    {0.0, 2.0, 0.0},    //4
    {0.0, 2.0, 2.0},    //5
    {2.0, 2.0, 2.0},    //6
    {2.0, 2.0, 0.0}     //7
};

static GLubyte cubeIdx[6][4] =
{
    {0,3,2,1},
    {5,4,7,6},
    {5,1,2,6},
    {0,3,7,4},
    {5,4,0,1},
    {2,3,7,6}
};

typedef struct Material {
    GLfloat * no;
    GLfloat * ambient;
    GLfloat * diffuse;
    GLfloat * specular;
    GLfloat * shininess;
} Material;

typedef struct PlaneCoords {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} PlaneCoords;

PlaneCoords carCoords[4];
PlaneCoords objCoords[500][4];
Material roof_mat, cube_mat, fence_mat, grass_mat, cbody_mat, glass_mat, t_mat, b_mat;
static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void assignMaterial(Material mat) {
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat.ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat.diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat.specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat.shininess);
}

void drawCube(Material mat)
{

    assignMaterial(mat);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(cube[cubeIdx[i][0]][0],
                    cube[cubeIdx[i][0]][1],
                    cube[cubeIdx[i][0]][2],
                    cube[cubeIdx[i][1]][0],
                    cube[cubeIdx[i][1]][1],
                    cube[cubeIdx[i][1]][2],
                    cube[cubeIdx[i][2]][0],
                    cube[cubeIdx[i][2]][1],
                    cube[cubeIdx[i][2]][2]);
        glVertex3fv(&cube[cubeIdx[i][0]][0]);
        glTexCoord2f(1, 1);
        glVertex3fv(&cube[cubeIdx[i][1]][0]);
        glTexCoord2f(0, 1);
        glVertex3fv(&cube[cubeIdx[i][2]][0]);
        glTexCoord2f(1, 0);
        glVertex3fv(&cube[cubeIdx[i][3]][0]);
        glTexCoord2f(0, 0);
    }
    glEnd();

}

void drawCar(Material bodyMat, Material glassMat, Material tyreMat)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glPushMatrix();
    glScalef(2.0, 0.4, 0.7);
    glTranslatef(0.0, 1.9, 1.0);
    drawCube(bodyMat);
    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glPushMatrix();
    glScalef(1, 0.4, 0.7);
    glTranslatef(0.75, 4.0, 1.0);
    drawCube(glassMat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    assignMaterial(tyreMat);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatef(2, 2, 8);
    glutSolidTorus(0.5, 1.0, 10, 50);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatef(2, 2, 4);
    glutSolidTorus(0.5, 1.0, 10, 50);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatef(14, 2, 4);
    glutSolidTorus(0.5, 1.0, 10, 50);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatef(14, 2, 8);
    glutSolidTorus(0.5, 1.0, 10, 50);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawQuad()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1, 1,1 , 1.0 };
    GLfloat mat_shininess[] = {60};
    GLfloat mat_emission[] = {1.0, 0, 0.5, 1.0};
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_QUADS);
        glVertex2f(1.0, 1.5);
        glVertex2f(0.0, 1.5);
        glVertex2f(0.0, -1.5);
        glVertex2f(1.0, -1.5);
    glEnd();

}

void drawCylinder()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.0, 0.1, 0.5, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.3, 0.5, 1.0 };
    GLfloat mat_specular[] = { 1, 1,1 , 1.0 };
    GLfloat mat_shininess[] = {60};
    GLfloat mat_emission[] = {1.0, 0, 0.5, 1.0};
//front phase niye kaaj hoche
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic,0.2f,0.2f,5.0f,30,30);
}

GLfloat fAng = 0.0;
GLboolean fRot = true;

void drawBlades()
{
    glPushMatrix();
    //glRotatef(90.0, 1.0, 0.0, 0.0);
    //glRotatef(90.0, 0.0, 1.0, 0.0);
    //glTranslatef(-6.2, -0.7, 3.4);
    glPushMatrix();
    glScalef(2, 1, 1);
    glRectf(0.0, 0.0, 1.5, -0.5);
    glPopMatrix();
    glPushMatrix();

    glTranslatef(1.25, -1.75, 0.0);
    glRotatef(90.0, 0, 0, 1);
    glScalef(2, 1, 1);
    glRectf(0.0, 0.0, 1.5, -0.5);
    glPopMatrix();
    //glRectf(1.5, 0.5, 4.0, 0.0);
    glPopMatrix();
}

void drawTree(Material barkMat)
{

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glPushMatrix();
    glTranslatef(5, 5, -1);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(1.5, 4, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5, 3, -1);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(1.3, 3, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5, 4, -1);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(1.4, 3.5, 30, 10);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);

    assignMaterial(barkMat);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glPushMatrix();
    glTranslated(5, 0, -1);
    glRotatef(-90, 1, 0, 0);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic,0.2f,0.2f,5.0f,30,30);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}


void drawGround(Material f_mat, GLfloat zInc)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glPushMatrix();
    roadInc += zInc;
    glScalef(40.0, 0.15, roadInc);
    glTranslatef(-0.75, -55, 0);
    drawCube(f_mat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawSky(Material s_mat)
{

    assignMaterial(s_mat);
    glPushMatrix();
    glTranslatef(0, 0, -60);
    //glScalef(40.0, 0.15, -50.0);
    glutSolidSphere(300, 300, 300);
    //glTranslatef(1, 5, 0);
    //drawCube(s_mat);
    glPopMatrix();
}
void drawRoad(Material r_mat, Material s_mat, GLfloat zInc)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[8]);
    glPushMatrix();
    roadInc += zInc;
    glScalef(0.2, 0.2, roadInc);
    //glRotatef(1.5, 0, 1, 0);
    glTranslatef(-30, -38, 0);
    drawCube(s_mat);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.2, 0.2, roadInc);
    //glRotatef(1.5, 0, 1, 0);
    glTranslatef(48, -38, 0);
    drawCube(s_mat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    glPushMatrix();
    glScalef(8.0, 0.2, roadInc);
    //glTranslatef(-0.75, 0, 0);
    //glRotatef(1.5, 0, 1, 0);
    glTranslatef(-0.75, -40, 0);
    drawCube(r_mat);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
void light0() {
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.7, 0.7, 0.7, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 40.0, 0, 1.0 };

    glEnable( GL_LIGHT0);
    //fv=floot vector
    //GL_LIGHT0=0th light
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
}

void light1() {
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_spot[] = {-1.0,-1.0,0};

    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 50.0, 0.0, 0.0, 1.0 };

    glEnable( GL_LIGHT1);
    //fv=floot vector
    //GL_LIGHT0=0th light
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
    glLightf( GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
    glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, light_spot);
    glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
}

void lightCtrl()
{
    if(lt1 == false) glDisable(GL_LIGHT0);
    else glEnable(GL_LIGHT0);

    if(lt2 == false) glDisable(GL_LIGHT1);
    else glEnable(GL_LIGHT1);
}


void setCoords(PlaneCoords *coords, GLfloat x, GLfloat y, GLfloat z)
{
    coords->x = x;
    coords->y = y;
    coords->z = z;
}


void setMaterial(Material *mat, GLfloat *no, GLfloat *amb, GLfloat *diff, GLfloat *spec,GLfloat *shin)
{
    mat->no = no;
    mat->ambient = amb;
    mat->diffuse = diff;
    mat->specular = spec;
    mat->shininess = shin;
    //printf("shininess : %f", mat->shininess[0]);
}
void updateLookAtValues()
{
    double lx = sin(-1 * yaw / 180 * pi) * cos(pitch / 180 * pi);
    double ly = cos(yaw / 180 * pi) * sin(pitch / 180 * pi);
    double lz =  -1 * cos(pitch / 180 * pi) * cos(yaw / 180 * pi);

    double ux = cos(pitch / 180 * pi) * sin(-1 * roll / 180 * pi);
    double uy = cos(pitch / 180 * pi) * cos(roll / 180 * pi);
    double uz = sin(pitch / 180 * pi) * cos(roll / 180 * pi);
    centerx = eyex + lx;
    centery = eyey + ly;
    centerz = eyez + lz;

    upx = ux;
    upy = uy;
    upz = uz;

    printf(" lx %f ly %f lz %f\n", lx, ly, lz);
    printf(" centerx %f centery %f centerz %f\n", centerx, centery, centerz);
    printf(" upx %f upy %f upz %f\n", upx, upy, upz);
}

void updateRoll()
{
    double ux = cos(pitch) * sin(-1 * roll);
    double uy = cos(pitch) * cos(roll);
}

void displayString(int x, int y,int z, char *str, int font=2)
{
    int len,i;
    //glColor3f(0.8,0.8,1.0);
    glRasterPos3f(x, y,z);
    len = (int) strlen(str);
    for (i = 0; i < len; i++)
    {
        if(font==1)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str[i]);
        }
        if(font==2)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
        }
        if(font==3)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[i]);
        }
        if(font==4)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,str[i]);
        }
    }
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, 4, 1000.0);
     //gluPerspective(60,1,5,100);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyex,eyey,eyez, centerx,centery,centerz, upx,upy,upz); // camera , ,

    glViewport(0, 0, windowHeight, windowWidth);

    //glRotatef(30, 0, 0, 1 );
    //glScalef(1.5,1.5,1.5);

     glRotatef( alpha,axis_x, axis_y, 0.0 );
     glRotatef( theta, axis_x, axis_y, 0.0 );

      lightCtrl();

    drawSky(roof_mat);
    glPushMatrix();
    glTranslated(8, 10, 0);
    glScaled(0.01, 0.01, 0.01);
    drawCube(b_mat);
    char *scoreStr = (char*)malloc(11 * sizeof(char));
    sprintf(scoreStr, "Score : %d", score);
    displayString(50.0, 90.0, 0.0, scoreStr);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, zMove);
    drawGround(grass_mat, 0);
    drawRoad(cube_mat, roof_mat, 0);
    for(int i = idx; i >= -40+roadInc; i-=5)
    {
        glPushMatrix();
        glTranslatef(10, -8, i);
        drawTree(b_mat);
        glPopMatrix();
    }
    for(int i = idx; i >= -40+roadInc; i-=5)
    {
        glPushMatrix();
            glTranslatef(-15, -8, i);
            drawTree(b_mat);
        glPopMatrix();
    }

    int tempIdx = 0;
    for(int i = -10; i >= -40+roadInc; i-=10)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[6]);
        glPushMatrix();
        int xInc = 0;
        if(tempIdx % 2 == 0) {glTranslatef(3, -7, i); xInc = 3; }
        else {glTranslatef(-1 * 3, -7, i); xInc = -3; }
        setCoords(&objCoords[tempIdx][0], 0.0*2.3+xInc, -7.0, 0.0 + (zMove+i));
        setCoords(&objCoords[tempIdx][1], 2.0*2.3+xInc, -7.0, 0.0 + (zMove+i));
        setCoords(&objCoords[tempIdx][2], 2.0*2.3+xInc, -7.0, 2.0 + (zMove+i));
        setCoords(&objCoords[tempIdx][3], 0.0*2.3+xInc, -7.0, 2.0 + (zMove+i));

        tempIdx++;

        glScalef(2.3, 1.0, 1.0);
        drawCube(fence_mat);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();

    for(int i = 0; i < 4; i++) {}

    setCoords(&carCoords[0], 0.0 + xMove, 0.0-7.0, 0.0);
    setCoords(&carCoords[1], 2.0 + xMove, 0.0-7.0, 0.0);
    setCoords(&carCoords[2], 2.0 + xMove, 0.0-7.0, 2.0);
    setCoords(&carCoords[3], 0.0 + xMove, 0.0-7.0, 2.0);

    bool flagz = false, flagx = false;
    for(int i = 0; i < tempIdx; i++)
    {
        if(objCoords[i][0].z > carCoords[0].z) visited[i] = true;
        if(visited[i]) continue;
        for(int j = 0; j < 4; j++) {
            float distz = (float)abs(objCoords[i][j].z - carCoords[j].z);
            float distx = (float)abs(objCoords[i][j].x - carCoords[j].x);
            if(distz <= 5.0) flagz = true;
            if(distx <= 0.9) flagx = true;
//            printf("dist %d obj z %f\n", i, objCoords[i][j].z - carCoords[j].z);
//            printf("dist %d obj x %f\n", i, objCoords[i][j].x - carCoords[j].x);
        }

        if(flagx && flagz) { printf("collision %d\n", i); isGameOver = true; break;}
        flagx = false;
        flagz = false;
    }

    glPushMatrix();
    glTranslated(xMove, -7, 0);
    glScalef(0.9, 0.9, 0.9);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    drawCar(cbody_mat, t_mat, cube_mat);
    glPopMatrix();

    int tempScore = 0;
    for(int i = 0; i < tempIdx; i++) {
        if(visited[i]) tempScore+=10;
    }
    score = tempScore;
    glutSwapBuffers();
}

void gameOverDisplay()
{
    glClearColor(0,0,0,0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
    //glClearColor(0.0,0.0,0.0,0.5);
    glClear(GL_COLOR_BUFFER_BIT);
    char *scoreStr = (char*)malloc(11 * sizeof(char));
    sprintf(scoreStr, "Score : %d", score);
    displayString(400, 300, 0, "Game Over!");
    displayString(400, 250, 0, scoreStr);
    glutPostRedisplay();
    glutSwapBuffers();
}

void welcomeDisplay()
{
    glClearColor(0,0,0,0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
    glClear(GL_COLOR_BUFFER_BIT);
    displayString(350, 300, 0, "Object Avoidance Game!");
    displayString(350, 250, 0, "Press 'space' to start the game");
    displayString(350, 230, 0, "Press 'x' to start the car");
    displayString(350, 220, 0, "Press 'a' to go left");
    displayString(350, 200, 0, "Press 'd' to go right");
    glutPostRedisplay();
    glutSwapBuffers();
}

GLvoid DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(isGameOver)
    {
        gameOverDisplay();
    }
    else if(isWelcomeDisplay)
    {
        welcomeDisplay();
    }
    else
    {
        display();
    }
}
/*
r => rotate
1 => light0 on off
2 => light1 on off
f, g => zoom in zoom out
x => car move
*/

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'r':
    case 'R':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=1.0;
        break;
    case 'x':
        carStart = !carStart;
        printf("x pressed\n");
        break;
    case 'z':
        eyez--;
        centerz--;
        break;
    case 'Z':
        eyez++;
        centerz++;
        break;
    case 'm':
        fRot = !fRot;
        break;
//    case 'x':
//        xMove += 0.5;
//        if(xMove > 10) xMove = -10;
//        break;
    case '1':
        lt1 = !lt1;
        break;
    case '2':
        lt2 = !lt2;
        break;
    case 'a':
        xMove -= 0.7;
        break;
    case 'd':
        xMove += 0.7;
        break;
//    case 'a':
//        eyex-=2;
//        centerx-=2;
//        break;
//    case 'd':
//        eyex+=2;
//        centerx+=2;
//        break;
//    case 'w':
//        eyey+=2;
//        break;
//    case 's':
//        eyey-=2;
//        break;
    case 'b':
        isBirdEye = !isBirdEye;
        if(isBirdEye == false)
        {
            eyex = 1;
            eyey = 1;
            eyez = 10;
        }
        else {
            GLfloat temp = eyez;
            eyez = eyex;
            eyey = temp;
        }
        break;
    case 'g':
        if(isBirdEye)
        {
            eyez-=0.5;
            glutPostRedisplay();
        }
        break;
    case 'G':
        if(isBirdEye)
        {
            eyez+=0.5;
            glutPostRedisplay();
        }
        break;
    case 'h':
        if(isBirdEye)
        {
            eyey-=0.5;
            glutPostRedisplay();
        }
        break;
    case 'H':
        if(isBirdEye)
        {
            eyey+=0.5;
            glutPostRedisplay();
        }
        break;
    case 'j':
        if(isBirdEye)
        {
            eyex-=0.5;
            glutPostRedisplay();
        }
        break;
    case 'J':
        if(isBirdEye)
        {
            eyex+=.5;
            glutPostRedisplay();
        }
        break;
    //pitch
    case '3':
        pitch += 1.5;
        updateLookAtValues();
        break;
    //roll
    case '4':
        roll += 1.5;
        updateLookAtValues();
        break;
    //yaw
    case '5':
        yaw += 1.5;
        updateLookAtValues();
        break;
        //pitch
    case '#':
        pitch -= 1.5;
        updateLookAtValues();
        break;
    //roll
    case '$':
        roll -= 1.5;
        updateLookAtValues();
        break;
    //yaw
    case '%':
        yaw -= 1.5;
        updateLookAtValues();
        break;
    case ' ':
        isWelcomeDisplay = false;
        break;
    case 27:	// Escape key
        exit(1);
    }
}



void animate()
{
    if (bRotate == true)
    {
        theta += 0.2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }
    if (fRot == true)
    {
        fAng += 2.0;
    }
    if (carStart == true)
    {
        zMove += 0.3;
        //printf("zMove %f\n", zMove);
        if((int)zMove % 30 == 0) roadInc -= 10.0;
    }
    glutPostRedisplay();

}

void LoadTexture(const char*filename, unsigned int id)
{

    glBindTexture(GL_TEXTURE_2D, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Object Avoidance Game");
    textures = (unsigned int *)malloc(numOfTexs * sizeof(unsigned int));
    glGenTextures(numOfTexs, textures);
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\grass.bmp", textures[0]); /// 1
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\car.bmp", textures[1]); /// 2
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\tire.bmp", textures[3]);
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\tree.bmp", textures[4]);
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\bark.bmp", textures[5]);
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\crate.bmp", textures[6]);
    LoadTexture("C:\\Users\\User\\Desktop\\GraphicsGame\\road.bmp", textures[7]);
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    light0();
    light1();
          // declare roof material
      GLfloat roof_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat roof_ambient[4] = { 0.529, 0.808, 0.922, 1.0};
      GLfloat roof_diffuse[4] = { 0.529, 0.808, 0.922, 1.0};
      GLfloat roof_specular[4] = { 0.529, 0.808, 0.922, 1.0};
      GLfloat roof_shininess[1] = {40};
      setMaterial(&roof_mat, roof_no, roof_ambient, roof_diffuse, roof_specular, roof_shininess);
      // declare cube material

      GLfloat cube_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat cube_ambient[4] = { 0.59, 0.59, 0.59, 1.0};
      GLfloat cube_diffuse[4] = { 0.55, 0.55, 0.55, 1.0 };
      GLfloat cube_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat cube_shininess[1] = {70};
      setMaterial(&cube_mat, cube_no, cube_ambient, cube_diffuse, cube_specular, cube_shininess);

      // declare fence material
      GLfloat fence_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat fence_ambient[4] = { 0.588, 0.294, 0.0, 1.0 };
      GLfloat fence_diffuse[4] = { 0.588, 0.294, 0.0, 1.0 };
      GLfloat fence_specular[4] = { 1, 1, 1, 1.0 };
      GLfloat fence_shininess[1] = {60};
      setMaterial(&fence_mat, fence_no, fence_ambient, fence_diffuse, fence_specular, fence_shininess);

      // declate grass material
      GLfloat grass_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat grass_ambient[4] = { 0.1, 0.43, 0.06, 1.0 };
      GLfloat grass_diffuse[4] = { 0.08, 0.37, 0.05, 1.0 };
      GLfloat grass_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat grass_shininess[1] = {50};
      setMaterial(&grass_mat, grass_no, grass_ambient, grass_diffuse, grass_specular, grass_shininess);

      //car body material
      GLfloat cbody_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat cbody_ambient[4] = { 0.662, 0.665, 0.624, 1.0 };
      GLfloat cbody_diffuse[4] = { 0.6, 0.7, 0.2, 1.0 };
      GLfloat cbody_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat cbody_shininess[1] = {70};
      setMaterial(&cbody_mat,
                  cbody_no,
                  cbody_ambient,
                  cbody_diffuse,
                  cbody_specular,
                  cbody_shininess);

      //glass material
      GLfloat glass_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat glass_ambient[4] = { 0.964, 0.996, 1.0, 1.0 };
      GLfloat glass_diffuse[4] = { 0.9, 0.9, 1.0, 1.0 };
      GLfloat glass_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat glass_shininess[1] = {90};
      setMaterial(&glass_mat,
                  glass_no,
                  glass_ambient,
                  glass_diffuse,
                  glass_specular,
                  glass_shininess);
      //tyre material
      GLfloat t_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat t_ambient[4] = { 0.21, 0.22, .27, 1.0 };
      GLfloat t_diffuse[4] = { 0.2, 0.2, 0.1, 1.0 };
      GLfloat t_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat t_shininess[1] = {90};
      setMaterial(&t_mat,
                  t_no,
                  t_ambient,
                  t_diffuse,
                  t_specular,
                  t_shininess);

      // bark material
      GLfloat b_no[4] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat b_ambient[4] = { 0.517, 0.494, .462, 1.0 };
      GLfloat b_diffuse[4] = { 0.5, 0.4, 0.45, 1.0 };
      GLfloat b_specular[4] = { 1, 1,1 , 1.0 };
      GLfloat b_shininess[1] = {90};
      setMaterial(&b_mat,
                  b_no,
                  b_ambient,
                  b_diffuse,
                  b_specular,
                  b_shininess);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(DrawGLScene);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}

