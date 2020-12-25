#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include "player.h"
#include "enemy.h"
#include "printer.h"
using namespace std;
int nyawa = 5;
float var1 = 1*1000;
float var2;
char text_nyawa[1000];
float x, y;
float mouseX,mouseY;
// x1 y1 untuk enemy
float x1 = -30;
float y1 = (rand() % 40 + 1);
Player player;
Enemy enemy;
Printer printer;
float cooldown = 10;
bool live =false;
// Collider
float arenaX[2] = {0, 50};
float arenaY[2] = {0, 50};

float windowSize[2] = {400,400};
float orthoSize[4] = {
    0, // xStart
    50, // xEnd
    0, // yStart
    50 // yEnd
};
// Posisi titik
int tombol[4] = {
    15, // xStart
    20, // xEnd
    25, // yStart
    35// yEnd
};
bool tombolActive;


void Colliderarena(){ // Collider bentuk kotak
    glPushMatrix();
    glBegin(GL_POLYGON);
	glVertex2f(arenaX[0], arenaY[0]);
	glVertex2f(arenaX[1], arenaY[0]);
	glVertex2f(arenaX[1], arenaY[1]);
	glVertex2f(arenaX[0], arenaY[1]);
	glEnd();
    glPopMatrix();
}
// 5-662
float RandomFloat(float min, float max)
{
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
}

void diplayenemy(void){
        glPushMatrix();
        //glTranslated(x1,y1,0);
        enemy.GambarPersegi();
        glPopMatrix();
    }
void text_draw(void){
    sprintf(text_nyawa, "nyawa %d", nyawa );
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    printer.drawText(0,47,text_nyawa);
    glPopMatrix();
}

void page1(void)
{
    glEnable(GL_COLOR_MATERIAL);
    // kotak
    // locking bentuk
    glPushMatrix();
    glTranslatef(x, y, 0);
    player.GambarPlayer();
    glPopMatrix();

    glPushMatrix();
    enemy.ColliderPersegi(x1,y1);
    glTranslatef(x1,y1,0);
    diplayenemy();
    glPopMatrix();
    text_draw();
    glFlush();
}
void page2(void){
    glColor3f(1.0f,0.0f,1.0f);
    glPushMatrix();
    glTranslatef(25, 25, 0);
    player.GambarPlayer();
    glPopMatrix();
    glPushMatrix();
    printer.drawText(25,47,"YOUR LIVE IS 0");
    glPopMatrix();
    glFlush();
}
void menupage(void){
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);
    printer.drawText(15,22,"Pejet bang");
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,1.0f);
    glTranslatef(15,25,0);
    enemy.GambarPersegi();
    glPopMatrix();
    glFlush();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    if(nyawa>0 && live){
        page1();
    } else if(nyawa<=0 &&nyawa==false) {
        page2();
    }else{
        menupage();
    }
    glutSwapBuffers();
}

//timer yang disinkronasikan dengan gerakan dan collision
void timer(int data)
{
    if (nyawa<=0){
            live=false;
        }
    // enemy spawn di posisi berbeda ketika keluar window
    srand((unsigned) time(0)); //srand supaya tiap pemanggilan random valuenya selalu berubah
    if (x1 <= arenaX[1] && live) {
        x1 += 0.03f;
        //cout << "x1 = " << x1 << endl;
    } else if (x1 > arenaX[1] &&live) {
        x1 = -20;
        y1 = (rand() % 40);
        cout << "y1 = " << y1 << endl;
    }
    // end of random spawn
    //colisin
    if (var2>0){
        var2--;
    }
    if (var2<=0){
        if(player.posisiX[0]< x1 +10 &&
            player.posisiX[0] + 10 > x1 &&
            player.posisiY[0] < y1 + 10 &&
            player.posisiY[0] + 10 > y1&&nyawa)
        {
        nyawa--;
        cout<<"Collision Detected"<<endl;
        cout<<nyawa<<endl;
        var2=var1;
        }

    }

    // Jika menekan tombol panah kiri
    if(GetAsyncKeyState(VK_LEFT)){
        if (player.posisiX[0]>arenaX[0]&&live) {
                player.posisiX[0]-=0.1f;
                player.posisiX[1]-=0.1f;
                x-=0.1f;
                cout<<"left"<<x<<" "<<y<<"\n";
            }
    }
    // Jika menekan tombol panah kanan
    else if(GetAsyncKeyState(VK_RIGHT)){
        if (player.posisiX[1]<arenaX[1]&&live) {
            x+=0.1f;
            player.posisiX[0]+=0.1f;
            player.posisiX[1]+=0.1f;
             cout<<"right"<<x<<" "<<y<<"\n";
        }
    }

    // Jika menekan tombol panah atas
    if(GetAsyncKeyState(VK_UP)){
         if (player.posisiY[0]>=arenaY[0]&& player.posisiY[1]<arenaY[1]&&live) {
            y+=0.1f;
            player.posisiY[0]+=0.1f;
            player.posisiY[1]+=0.1f;
             cout<<"up"<<x<<" "<<y<<"\n";            }
        else {
            y=40;
            player.posisiY[0]=40;
            player.posisiY[1]=50;
         }
    }
    // Jika menekan tombol panah bawah
    else if (GetAsyncKeyState(VK_DOWN)){
        if (player.posisiY[1]<=arenaY[1] && player.posisiY[0]>arenaY[0] &&live ){
            y-=0.1f;
            player.posisiY[0]-=0.1f;
            player.posisiY[1]-=0.1f;
             cout<<"down"<<x<<" "<<y<<"\n";
           }
        else{
            y=0;
            player.posisiY[0]=0;
            player.posisiY[1]=10;
        }
    }

    glutPostRedisplay();
	glutTimerFunc(1,timer,0);
}
void getMouseActivePos(int button, int state, int rawPosX, int rawPosY) {
    // Mendapatkan posisi mouse setelah diproses
    int mousePositionX = rawPosX / 10; // Posisi raw dibagi ratio ortho dengan window size
	int mousePositionY = orthoSize[3] - (rawPosY / 10); // Posisi raw dibagi ratio ortho dengan window size

	if (button == GLUT_LEFT_BUTTON && // apabila menekan tombol mouse kiri
     state == GLUT_ENTERED){ // apabila mouse sedang berada diatas window
        cout << "Posisi X: " << mousePositionX << "\n";
        cout << "Posisi Y: " << mousePositionY << "\n";
        cout << "==========\n";

        // Collide dengan tombol
        if (mousePositionX >= tombol[0] && mousePositionX <= tombol[1]){
            if (mousePositionY >= tombol[2] && mousePositionY <= tombol[3]){
                // Code Here
                cout << "hello there!!\n";
                tombolActive = !tombolActive;
                live=true;
            }
        }
	}
}int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Dodge The Enemies");
    //cahaya();
    //pemanggilan timer
    glutTimerFunc(1,timer,0);
    gluOrtho2D(0, 50, 0, 50);
    glutDisplayFunc(display);
    glutMouseFunc(getMouseActivePos);
    glutMainLoop();
    return 0;
}
