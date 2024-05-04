#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "constants.h"

#include "utilityFunctions.h"

// STB_IMPORTING for images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Bump Allocator Code:
BumpAllocator transientStorage;

int last_frame_time = 0;
GLFWwindow* window;

bool gameEnd = false;
int winner = -1;//winner == 0 for player and == 1 for enemy

//Screen Texture for gameEnd
GLuint finishTextureID1, finishTextureID2;
void loadFinish1Texture() {
    int width, height, channels;
    unsigned char* image = stbi_load("./assets/p1won.png", &width, &height, &channels, STBI_rgb_alpha);

    if (image == nullptr) {
        printf("Failed to load background image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &finishTextureID1);
    glBindTexture(GL_TEXTURE_2D, finishTextureID1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadFinish2Texture() {
    int width, height, channels;
    unsigned char* image = stbi_load("./assets/p2won.png", &width, &height, &channels, STBI_rgb_alpha);

    if (image == nullptr) {
        printf("Failed to load background image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &finishTextureID2);
    glBindTexture(GL_TEXTURE_2D, finishTextureID2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadFinishTexture() {
    //load player-1 win texture
    loadFinish1Texture();

    //load player-2 win texture
    loadFinish2Texture();
}

// ######################################## BACKGROUND AND PLAYER SPRITE IMAGE LOADING LOGIC ####################################
// Function to load the background image
GLuint backgroundTextureID;
void loadBackgroundTexture() {
    int width, height, channels;
    unsigned char* image = stbi_load("./assets/background.png", &width, &height, &channels, STBI_rgb_alpha);

    if (image == nullptr) {
        printf("Failed to load background image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &backgroundTextureID);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Load sprite texture
GLuint spriteTextureID, spriteKickTextureID;
GLuint enemySpriteTextureID, enemyKickTextureID;

void loadPlayerIdleSprite() {
    int width, height, channels;
    unsigned char* image = stbi_load("./assets/goukenIdle.png", &width, &height, &channels, STBI_rgb_alpha); // Use STBI_rgb_alpha for PNG images with alpha channel

    if (image == nullptr) {
        printf("Failed to load sprite image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &spriteTextureID);
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadPlayerKickSprite() {
    int width, height, channels;
    unsigned char* image = stbi_load("./assets/goukenKick.png", &width, &height, &channels, STBI_rgb_alpha); // Use STBI_rgb_alpha for PNG images with alpha channel

    if (image == nullptr) {
        printf("Failed to load sprite image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &spriteKickTextureID);
    glBindTexture(GL_TEXTURE_2D, spriteKickTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadEnemyIdleSprite() {
    int width2, height2, channels2;
    unsigned char* image2 = stbi_load("./assets/enemy.png", &width2, &height2, &channels2, STBI_rgb_alpha); // Use STBI_rgb_alpha for PNG images with alpha channel

    if (image2 == nullptr) {
        printf("Failed to load sprite image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &enemySpriteTextureID);
    glBindTexture(GL_TEXTURE_2D, enemySpriteTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image2);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadEnemyKickSprite() {
    int width2, height2, channels2;
    unsigned char* image2 = stbi_load("./assets/enemySlap.png", &width2, &height2, &channels2, STBI_rgb_alpha); // Use STBI_rgb_alpha for PNG images with alpha channel

    if (image2 == nullptr) {
        printf("Failed to load sprite image.\n");
        // Print more details about the error if available
        printf("Error: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &enemyKickTextureID);
    glBindTexture(GL_TEXTURE_2D, enemyKickTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2); // Use GL_RGBA for PNG images with alpha channel
    stbi_image_free(image2);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Set texture filtering to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadSpriteTexture() {
    //Call Player Idle State Sprite
    loadPlayerIdleSprite();

    //Call Player Kick State Sprite
    loadPlayerKickSprite();

    //Call Enemy Idle State Sprite
    loadEnemyIdleSprite();

    //Call Enemy Kick State Sprite
    loadEnemyKickSprite();
}

// ############################################## PLAYER AND ENEMY STRUCT DEFINATION #############################################

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
    float lastAttackTime;
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
    float lastAttackTime;
} enemy;

void setup() {
    // ########################################### ALLOCATING PROPERTIES TO PLAYER AND ENEMY ##################################3
    player.x = 300;
    player.y = 0;
    player.width = 50;
    player.height = 150;
    player.velocityY = 400;
    player.velocityX = 0;
    player.aKeyPressed = false;
    player.dKeyPressed = false;
    player.wKeyPressed = false;
    player.attackBoxHeight = 50;
    player.attackBoxWidth = 230;
    player.attackBoxPositionX = player.x;
    player.attackBoxPositionY = player.y;
    player.isAttacking = false;
    player.attackTimerRunning = false;//attack timer for toggle
    player.healthBarHeight = 50;
    player.healthBarWidth = 650;
    player.lastAttackTime = 0.0;

    enemy.x = WINDOW_WIDTH/2+400;
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
    enemy.healthBarWidth = 650;
    enemy.lastAttackTime = 0.0;

    // ############################################# TEXTURE LOADING #######################################################
    //Setting up background image texture
    // Load background texture
    loadBackgroundTexture();
    //load sprite texture
    loadSpriteTexture();
    //load finish texture
    loadFinishTexture();

    return;
}

// ########################## BACKGROUND, FINISH & SPRITE RENDER LOGIC WHICH IS TRIGGERED IN RENDER() IN GAME LOOP ##################

// Function to render a full-screen quad with the background image
void renderBackground() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); // Texture coordinate (bottom left)
    glVertex2f(0, 0);

    glTexCoord2f(1.0f, 1.0f); // Texture coordinate (bottom right)
    glVertex2f(WINDOW_WIDTH, 0);

    glTexCoord2f(1.0f, 0.0f); // Texture coordinate (top right)
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);

    glTexCoord2f(0.0f, 0.0f); // Texture coordinate (top left)
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderFinish1() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, finishTextureID1);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); // Texture coordinate (bottom left)
    glVertex2f(0, 0);

    glTexCoord2f(1.0f, 1.0f); // Texture coordinate (bottom right)
    glVertex2f(WINDOW_WIDTH, 0);

    glTexCoord2f(1.0f, 0.0f); // Texture coordinate (top right)
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);

    glTexCoord2f(0.0f, 0.0f); // Texture coordinate (top left)
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderFinish2() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, finishTextureID2);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); // Texture coordinate (bottom left)
    glVertex2f(0, 0);

    glTexCoord2f(1.0f, 1.0f); // Texture coordinate (bottom right)
    glVertex2f(WINDOW_WIDTH, 0);

    glTexCoord2f(1.0f, 0.0f); // Texture coordinate (top right)
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);

    glTexCoord2f(0.0f, 0.0f); // Texture coordinate (top left)
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Function to render a full-screen quad with the background image
void renderFinish() {
    if (gameEnd == true) {
        if (winner == 0) {
            renderFinish1();
        }
        else {
            renderFinish2();
        }
    }
}

void renderPlayerIdleSprite() {
    // Calculate texture coordinates
    float textureWidth = 1.0f;
    float textureHeight = 1.0f;

    // Render Player Sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteTextureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); //Texture top-left cooridnate
    glVertex2f(player.x, WINDOW_HEIGHT - player.y + 100);

    glTexCoord2f(textureWidth, 0.0f); //Texture top-right cooridnate
    glVertex2f(player.x + player.width + 200, WINDOW_HEIGHT - player.y + 100);

    glTexCoord2f(textureWidth, textureHeight);
    glVertex2f(player.x + player.width + 200, WINDOW_HEIGHT - (player.y + player.height));

    glTexCoord2f(0.0f, textureHeight);
    glVertex2f(player.x, WINDOW_HEIGHT - (player.y + player.height));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderPlayerKickSprite() {
    // Calculate texture coordinates
    float textureWidth = 1.0f;
    float textureHeight = 1.0f;

    // Render Player Sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteKickTextureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); //Texture top-left cooridnate
    glVertex2f(player.x, WINDOW_HEIGHT - player.y + 100);

    glTexCoord2f(textureWidth, 0.0f); //Texture top-right cooridnate
    glVertex2f(player.x + player.width + 200, WINDOW_HEIGHT - player.y + 100);

    glTexCoord2f(textureWidth, textureHeight);
    glVertex2f(player.x + player.width + 200, WINDOW_HEIGHT - (player.y + player.height));

    glTexCoord2f(0.0f, textureHeight);
    glVertex2f(player.x, WINDOW_HEIGHT - (player.y + player.height));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderEnemyIdleSprite() {
    //Render Enemy Sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, enemySpriteTextureID);

    // Calculate texture coordinates
    float textureWidth2 = 1.0f;
    float textureHeight2 = 1.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); //Texture top-left cooridnate
    glVertex2f(enemy.x, WINDOW_HEIGHT - enemy.y + 90);

    glTexCoord2f(textureWidth2, 0.0f); //Texture top-right cooridnate
    glVertex2f(enemy.x + enemy.width - 230, WINDOW_HEIGHT - enemy.y + 90);

    glTexCoord2f(textureWidth2, textureHeight2);
    glVertex2f(enemy.x + enemy.width - 230, WINDOW_HEIGHT - (enemy.y + enemy.height));

    glTexCoord2f(0.0f, textureHeight2);
    glVertex2f(enemy.x, WINDOW_HEIGHT - (enemy.y + enemy.height));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderEnemyKickSprite() {
    //Render Enemy Sprite
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, enemyKickTextureID);

    // Calculate texture coordinates
    float textureWidth2 = 1.0f;
    float textureHeight2 = 1.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); //Texture top-left cooridnate
    glVertex2f(enemy.x, WINDOW_HEIGHT - enemy.y + 90);

    glTexCoord2f(textureWidth2, 0.0f); //Texture top-right cooridnate
    glVertex2f(enemy.x + enemy.width - 400, WINDOW_HEIGHT - enemy.y + 90);

    glTexCoord2f(textureWidth2, textureHeight2);
    glVertex2f(enemy.x + enemy.width - 400, WINDOW_HEIGHT - (enemy.y + enemy.height));

    glTexCoord2f(0.0f, textureHeight2);
    glVertex2f(enemy.x, WINDOW_HEIGHT - (enemy.y + enemy.height));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Function to render the sprite
void renderSprite() {
    //Player Idle State Sprite Render Call
    if(glfwGetTime() - player.lastAttackTime > 0.3)
        renderPlayerIdleSprite();

    //Player Kick State Sprite Render Call
    if(glfwGetTime() - player.lastAttackTime <= 0.3)
        renderPlayerKickSprite();

    //Enemy Idle State Sprite Render Call
    if (glfwGetTime() - enemy.lastAttackTime > 0.3)
        renderEnemyIdleSprite();

    //Eenemy Kick State Sprite Render Call
    if (glfwGetTime() - enemy.lastAttackTime <= 0.3)
        renderEnemyKickSprite();

    renderFinish();
}

// ############################################# PROCESSING KEYBOARD INPUT ################################################

void process_input(int key, int, int) {
    switch (key) {
    case GLFW_KEY_D:
        player.dKeyPressed = true;
        player.lastKey = 'd';
        break;
    case GLFW_KEY_A:
        player.aKeyPressed = true;
        player.lastKey = 'a';
        break;
    case GLFW_KEY_W:
        player.wKeyPressed = true;
        player.lastKey = 'w';
        player.velocityY = -2000;
        break;
    case GLFW_KEY_SPACE:
        //printf("spacebar pressed\n");
        if (glfwGetTime() - player.lastAttackTime >= 0.3) { // Check if enough time has passed since the last attack
            player.isAttacking = true;
            player.lastAttackTime = glfwGetTime(); // Update the last attack time
        }
        break;
    case GLFW_KEY_ESCAPE:
        exit(0);
        break;
    }
}

// ###################################### PROCESSING SPECIAL KEYS ON KEYBOARD ######################################

void special(int key, int, int) {
    switch (key) {
    case GLFW_KEY_LEFT: // Left arrow key
        enemy.leftKeyPressed = true;
        enemy.lastKey = 'l';
        break;
    case GLFW_KEY_RIGHT: // Right arrow key
        enemy.rightKeyPressed = true;
        enemy.lastKey = 'r';
        break;
    case GLFW_KEY_UP: // Up arrow key
        enemy.upKeyPressed = true;
        enemy.lastKey = 'u';
        enemy.velocityY -= 2000;
        break;
    case GLFW_KEY_DOWN: // down arrow key
        if (glfwGetTime() - enemy.lastAttackTime >= 0.3) { // Check if enough time has passed since the last attack
            enemy.isAttacking = true;
            enemy.lastAttackTime = glfwGetTime(); // Update the last attack time
        }
        break;
    }
}

// ############################################# UPDATE() OF GAME LOOP ##########################################################

void update() {
    int time_now = glfwGetTime() * 1000; // in milliseconds
    float delta_time = (time_now - last_frame_time) / 1000.0f;
    last_frame_time = time_now;

    player.x += player.velocityX * delta_time;
    player.attackBoxPositionX = player.x;

    player.y += player.velocityY * delta_time;
    player.attackBoxPositionY = player.y;
    if (player.y + player.height + player.velocityY * delta_time >= WINDOW_HEIGHT - 100) {
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
    bool xCollisionCondition = player.attackBoxPositionX + player.attackBoxWidth + 130 >= enemy.x && player.attackBoxPositionX <= enemy.x + enemy.width - 200;
    bool yCollisionCondition = player.attackBoxPositionY + player.attackBoxHeight >= enemy.y - 100 && player.attackBoxPositionY - 100 <= (enemy.y + enemy.height);
    if (xCollisionCondition && yCollisionCondition && player.isAttacking == true) {
        //Reduce enemy health width
        enemy.healthBarWidth -= (float)(enemy.healthBarWidth * 0.30);
        printf("Player attacked enemy!! \n");

        //printf("Enemy Health: %f \n",enemy.healthBarWidth);
        if (enemy.healthBarWidth <= 1) {
            gameEnd = true;
            winner = 0;
            printf("Player Won!!\n");
            //exit(0);
        }
    }

    //Setting player.isAttacking == false in a loop and activating only when spacebar is pressed
    player.isAttacking = false;

    //Enemy Logic
    enemy.x += enemy.velocityX * delta_time;
    enemy.y += enemy.velocityY * delta_time;
    if (enemy.y + enemy.height + enemy.velocityY * delta_time >= WINDOW_HEIGHT - 110) {
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
    bool xCollisionCondition2 = (enemy.x - 200) >= player.x && (enemy.x - 450) <= player.x + player.width;
    bool yCollisionCondition2 = enemy.y + enemy.height + 30 >= player.y && player.y + player.height >= enemy.y - 50;
    if (xCollisionCondition2 && yCollisionCondition2 && enemy.isAttacking == true) {
        //Reduce player health width
        player.healthBarWidth -= (float)(player.healthBarWidth * 0.20);
        printf("Enemy attacked Player!! \n");

        if (player.healthBarWidth <= 1) {
            printf("Enemy Won!!\n");
            gameEnd = true;
            winner = 1;
            //exit(0);
        }
    }

    //Setting enemy.isAttacking == false in a loop and activating only when spacebar is pressed
    enemy.isAttacking = false;
}

// ######################################### RENDER() OF GAME LOOP ########################################################

void render() {
    //Rendering the background image
    renderBackground();

    /*
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
    
    if (glfwGetTime() - player.lastAttackTime <= 0.3) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(player.x, WINDOW_HEIGHT - player.y);
        glVertex2f(player.x + player.attackBoxWidth, WINDOW_HEIGHT - player.y);
        glVertex2f(player.x + player.attackBoxWidth, WINDOW_HEIGHT - player.y - player.attackBoxHeight);
        glVertex2f(player.x, WINDOW_HEIGHT - player.y - player.attackBoxHeight);
        glEnd();
    }
    
    //Enemy Attack Box
    if (glfwGetTime() - enemy.lastAttackTime <= 0.1) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(enemy.x - 50, WINDOW_HEIGHT - enemy.y);
        glVertex2f(enemy.x - 50 + enemy.attackBoxWidth, WINDOW_HEIGHT - enemy.y);
        glVertex2f(enemy.x - 50 + enemy.attackBoxWidth, WINDOW_HEIGHT - enemy.y - enemy.attackBoxHeight);
        glVertex2f(enemy.x - 50, WINDOW_HEIGHT - enemy.y - enemy.attackBoxHeight);
        glEnd();
    }
    */

    //Drawing Health bars
    //Player Health Bar
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(5, WINDOW_HEIGHT - 5);//top left vertex
    glVertex2f(5 + player.healthBarWidth, WINDOW_HEIGHT - 5);
    glVertex2f(5 + player.healthBarWidth, WINDOW_HEIGHT - 5 - player.healthBarHeight);
    glVertex2f(5, WINDOW_HEIGHT - 5 - player.healthBarHeight);
    glEnd();

    //Enemy Health Bar
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5);//top left vertex
    glVertex2f(WINDOW_WIDTH - 5 - enemy.healthBarWidth, WINDOW_HEIGHT - 5);
    glVertex2f(WINDOW_WIDTH - 5 - enemy.healthBarWidth, WINDOW_HEIGHT - 5 - enemy.healthBarHeight);
    glVertex2f(WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5 - enemy.healthBarHeight);
    glEnd();

    // Reset color to white before rendering other objects
    glColor3f(1.0f, 1.0f, 1.0f);

    //render player sprite and enemy sprite
    renderSprite();

    glfwSwapBuffers(window);
}

// #################################### KEYBOARD PROCESSING CALLS PROCESS_INPUT() ###########################################
void keyboard(int key, int, int) {
    switch (key) {
    case GLFW_KEY_D:
    case GLFW_KEY_A:
    case GLFW_KEY_W:
    case GLFW_KEY_SPACE:
    case GLFW_KEY_ESCAPE:
        // Process input
        process_input(key, 0, 0);
        break;
    }
}

// ############################################## INIT() OF GAME LOOP #####################################################
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1); // Specify the orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ############################################ PROCESS_RELEASE FOR KEYBOARD #################################################
void process_release(int key, int, int) {
    switch (key) {
    case GLFW_KEY_D:
        player.dKeyPressed = false;
        break;
    case GLFW_KEY_A:
        player.aKeyPressed = false;
        break;
    case GLFW_KEY_W:
        player.wKeyPressed = false;
        break;
    case GLFW_KEY_SPACE:
        player.isAttacking = false;
        break;
    }
}
// ########################################## PROCESS_SPECIAL_RELEASE FOR SPECIAL KEYS #############################
void process_special_release(int key, int, int) {
    switch (key) {
    case GLFW_KEY_LEFT:
        enemy.leftKeyPressed = false;
        break;
    case GLFW_KEY_RIGHT:
        enemy.rightKeyPressed = false;
        break;
    case GLFW_KEY_UP:
        enemy.upKeyPressed = false;
        break;
    case GLFW_KEY_DOWN:
        enemy.isAttacking = false;
        break;
    }
}

void special_release(int key, int, int) {
    switch (key) {
    case GLFW_KEY_LEFT:
    case GLFW_KEY_RIGHT:
    case GLFW_KEY_UP:
    case GLFW_KEY_DOWN:
        process_special_release(key, 0, 0);
        break;
    }
}

// ##################################### TIMER() FUNC OF GAME LOOP WHICH CALLS UPDATE() #####################################
void timer() {
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;

    update();

    lastTime = currentTime;
}

// ###################################### KEYBOARD() FUNCTION ###############################################################
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        process_input(key, 0, 0);
    }
    else if (action == GLFW_RELEASE) {
        process_release(key, 0, 0);
    }

    // Process special keys
    switch (key) {
    case GLFW_KEY_LEFT:
    case GLFW_KEY_RIGHT:
    case GLFW_KEY_UP:
    case GLFW_KEY_DOWN:
        if (action == GLFW_PRESS) {
            special(key, 0, 0);
        }
        else if (action == GLFW_RELEASE) {
            special_release(key, 0, 0);
        }
        break;
    }
}

// ######################################## VARIABLE WINDOW SIZE ADJUSTMENT ###################################################
// Callback function to handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Adjust the viewport when the window is resized
    glViewport(0, 0, width, height);
}

// ######################################## INT MAIN() FUNCTION ################################################################
int main() {
    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2D FIGHTING GAME", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set the framebuffer size callback to handle window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    init();
    setup();

    // Set keyboard callback functions
    glfwSetKeyCallback(window, keyboard);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0f);

        // Update and render
        timer();
        render();

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}