#include <GL/glut.h>
#include <stdio.h>
#include "constants.h"

int last_frame_time = 0;

void timer(int);

struct Player {
    float x;
    float y;
    float width;
    float height;
    float velocityX;
    float velocityY;
    char lastKey;
    bool aKeyPressed;
    bool dKeyPressed;
    bool wKeyPressed;
    float attackBoxPositionX;
    float attackBoxPositionY;
    float attackBoxWidth;
    float attackBoxHeight;
    bool isAttacking;
    // Add a variable to keep track of whether the attack timer is running
    bool attackTimerRunning;
    //Health Bar
    float healthBarWidth;
    float healthBarHeight;
} player;

struct Enemy {
    float x;
    float y;
    float width;
    float height;
    float velocityX;
    float velocityY;
    char lastKey;
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool upKeyPressed;
    float attackBoxPositionX;
    float attackBoxPositionY;
    float attackBoxWidth;
    float attackBoxHeight;
    bool isAttacking;
    // Add a variable to keep track of whether the attack timer is running
    bool attackTimerRunning;
    //Health Bar
    float healthBarWidth;
    float healthBarHeight;
} enemy;

void setup() {
    player.x = 0;
    player.y = 0;
    player.width = 50;
    player.height = 150;
    player.velocityY = 400;
    player.velocityX = 0;
    player.aKeyPressed = false;
    player.dKeyPressed = false;
    player.wKeyPressed = false;
    player.attackBoxHeight = 50;
    player.attackBoxWidth = 100;
    player.attackBoxPositionX = player.x;
    player.attackBoxPositionY = player.y;
    player.isAttacking = false;
    player.attackTimerRunning = false;//attack timer for toggle
    player.healthBarHeight = 50;
    player.healthBarWidth = 500;

    enemy.x = 400;
    enemy.y = 0;
    enemy.width = 50;
    enemy.height = 150;
    enemy.velocityX = 0;
    enemy.velocityY = 400;
    enemy.leftKeyPressed = false;
    enemy.rightKeyPressed = false;
    enemy.attackBoxHeight = 50;
    enemy.attackBoxWidth = 100;
    enemy.attackBoxPositionX = enemy.x;
    enemy.attackBoxPositionY = enemy.y;
    enemy.isAttacking = false;
    enemy.attackTimerRunning = false;//attack timer for toggle
    enemy.healthBarHeight = 50;
    enemy.healthBarWidth = 500;
}

// Function to toggle the isAttacking variable
void PlayertoggleIsAttacking(int value) {
    player.isAttacking = !player.isAttacking;
    player.attackTimerRunning = false; // Reset the timer flag
}

void EnemytoggleIsAttacking(int value) {
    enemy.isAttacking = !enemy.isAttacking;
    enemy.attackTimerRunning = false; // Reset the timer flag
}

void process_input(unsigned char key, int, int) {
    switch (key) {
    case 'd':
        player.dKeyPressed = true;
        player.lastKey = 'd';
        break;
    case 'a':
        player.aKeyPressed = true;
        player.lastKey = 'a';
        break;
    case 'w':
        player.wKeyPressed = true;
        player.lastKey = 'w';
        player.velocityY = -2000;
        break;
    case ' ':
        //printf("spacebar pressed\n");
        player.isAttacking = true;
        if (!player.attackTimerRunning) {
            glutTimerFunc(100, PlayertoggleIsAttacking, 0); // Call toggleIsAttacking after 100 milliseconds
            player.attackTimerRunning = true; // Set the timer flag
        }
        //player.isAttacking = true;
        break;
    }
}

void special(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT: // Left arrow key
        enemy.leftKeyPressed = true;
        enemy.lastKey = 'l';
        break;
    case GLUT_KEY_RIGHT: // Right arrow key
        enemy.rightKeyPressed = true;
        enemy.lastKey = 'r';
        break;
    case GLUT_KEY_UP: // Up arrow key
        enemy.upKeyPressed = true;
        enemy.lastKey = 'u';
        enemy.velocityY -= 2000;
        break;
    case GLUT_KEY_DOWN: // down arrow key
        enemy.isAttacking = true;
        if (!enemy.attackTimerRunning) {
            glutTimerFunc(100, EnemytoggleIsAttacking, 0); // Call toggleIsAttacking after 100 milliseconds
            enemy.attackTimerRunning = true; // Set the timer flag
        }
        break;
    }
}

void update() {
    int time_now = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (time_now - last_frame_time) / 1000.0f;
    last_frame_time = time_now;

    player.x += player.velocityX * delta_time;
    player.attackBoxPositionX = player.x;

    player.y += player.velocityY * delta_time;
    player.attackBoxPositionY = player.y;
    if (player.y + player.height + player.velocityY * delta_time >= WINDOW_HEIGHT) {
        player.velocityY = 0;
    }
    else {
        player.velocityY += GRAVITY;
    }

    player.velocityX = 0;
    if (player.aKeyPressed == true) {
        player.velocityX = -400;
    }
    else if (player.dKeyPressed == true) {
        player.velocityX = 400;
    }

    //Player-Attack Colision Detection
    bool xCollisionCondition = player.attackBoxPositionX + player.attackBoxWidth >= enemy.x && player.attackBoxPositionX <= enemy.x + enemy.width;
    bool yCollisionCondition = player.attackBoxPositionY + player.attackBoxHeight >= enemy.y && player.attackBoxPositionY <= (enemy.y + enemy.height);
    if (xCollisionCondition && yCollisionCondition && player.isAttacking == true) {
        //Reduce enemy health width
        enemy.healthBarWidth -= (float)(enemy.healthBarWidth * 0.10);
        //printf("Player attacked enemy!! \n");

        //printf("Enemy Health: %f \n",enemy.healthBarWidth);
        if (enemy.healthBarWidth <= 0.03) {
            printf("Player Won!!\n");
            exit(0);
        }
    }

    //Enemy Logic
    enemy.x += enemy.velocityX * delta_time;
    enemy.y += enemy.velocityY * delta_time;
    if (enemy.y + enemy.height + enemy.velocityY * delta_time >= WINDOW_HEIGHT) {
        enemy.velocityY = 0;
    }
    else {
        enemy.velocityY += GRAVITY;
    }

    enemy.velocityX = 0;
    if (enemy.leftKeyPressed == true) {
        enemy.velocityX = -400;
    }
    else if (enemy.rightKeyPressed == true) {
        enemy.velocityX = 400;
    }

    //Enemy-Attack Collision Detection
    bool xCollisionCondition2 = (enemy.x - 50) >= player.x && (enemy.x-50) <= player.x + player.width;
    bool yCollisionCondition2 = enemy.y+50 <= (player.y+player.height) && (enemy.attackBoxPositionY) <= (player.y + player.height);
    if (xCollisionCondition2 && yCollisionCondition2 && enemy.isAttacking == true) {
        //Reduce player health width
        player.healthBarWidth -= (float)(player.healthBarWidth * 0.10);
        //printf("Enemy attacked Player!! \n");

        if (player.healthBarWidth <= 0.03) {
            printf("Enemy Won!!\n");
            exit(0);
        }
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(player.x, WINDOW_HEIGHT - player.y);
    glVertex2f(player.x + player.width, WINDOW_HEIGHT - player.y);
    glVertex2f(player.x + player.width, WINDOW_HEIGHT - player.y - player.height);
    glVertex2f(player.x, WINDOW_HEIGHT - player.y - player.height);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(enemy.x, WINDOW_HEIGHT - enemy.y);
    glVertex2f(enemy.x + enemy.width, WINDOW_HEIGHT - enemy.y);
    glVertex2f(enemy.x + enemy.width, WINDOW_HEIGHT - enemy.y - enemy.height);
    glVertex2f(enemy.x, WINDOW_HEIGHT - enemy.y - enemy.height);
    glEnd();

    //Player Attack Box
    if (player.isAttacking == true) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(player.x, WINDOW_HEIGHT - player.y);
        glVertex2f(player.x + player.attackBoxWidth, WINDOW_HEIGHT - player.y);
        glVertex2f(player.x + player.attackBoxWidth, WINDOW_HEIGHT - player.y - player.attackBoxHeight);
        glVertex2f(player.x, WINDOW_HEIGHT - player.y - player.attackBoxHeight);
        glEnd();
    }
    //Enemy Attack Box
    if (enemy.isAttacking == true) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(enemy.x - 50, WINDOW_HEIGHT - enemy.y);
        glVertex2f(enemy.x - 50 + enemy.attackBoxWidth, WINDOW_HEIGHT - enemy.y);
        glVertex2f(enemy.x - 50 + enemy.attackBoxWidth, WINDOW_HEIGHT - enemy.y - enemy.attackBoxHeight);
        glVertex2f(enemy.x - 50 , WINDOW_HEIGHT - enemy.y - enemy.attackBoxHeight);
        glEnd();
    }

    //Drawing Health bars
    //Player Health Bar
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(5,WINDOW_HEIGHT - 5);//top left vertex
    glVertex2f(5 + player.healthBarWidth, WINDOW_HEIGHT - 5);
    glVertex2f(5 + player.healthBarWidth, WINDOW_HEIGHT - 5 - player.healthBarHeight);
    glVertex2f(5,WINDOW_HEIGHT - 5 - player.healthBarHeight);
    glEnd();

    //Enemy Health Bar
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - 5 , WINDOW_HEIGHT - 5);//top left vertex
    glVertex2f(WINDOW_WIDTH - 5 - enemy.healthBarWidth, WINDOW_HEIGHT - 5);
    glVertex2f(WINDOW_WIDTH - 5 - enemy.healthBarWidth, WINDOW_HEIGHT - 5 - enemy.healthBarHeight);
    glVertex2f(WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5 - enemy.healthBarHeight);
    glEnd();


    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
    case 'd':
    case 'a':
    case 'w':
    case ' ':
        // Process input
        process_input(key,0,0);
        break;
    case 27: // Escape key
        exit(0);
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

void process_release(unsigned char key, int, int) {
    switch (key) {
    case 'd':
        player.dKeyPressed = false;
        break;
    case 'a':
        player.aKeyPressed = false;
        break;
    case 'w':
        player.wKeyPressed = false;
        break;
    }
}

void process_special_release(int key, int, int) {
    switch (key) {
    case GLUT_KEY_LEFT:
        enemy.leftKeyPressed = false;
        break;
    case GLUT_KEY_RIGHT:
        enemy.rightKeyPressed = false;
        break;
    case GLUT_KEY_UP:
        enemy.upKeyPressed = false;
        break;
    }
}

void timer(int) {
    update();
    glutPostRedisplay();
    glutTimerFunc(1000 / 120, timer, 0); // 120 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("2D FIGHTING GAME");

    init();
    setup();

    glutDisplayFunc(render);
    //process_input (KEYDOWN)
    glutKeyboardFunc(keyboard);
    //Arrow keys come in special
    glutSpecialFunc(special); // Register special key function
    //KEYUP
    glutKeyboardUpFunc(process_release);
    //KEYUP for special keys
    glutSpecialUpFunc(process_special_release);
    //Game-Loop
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}
