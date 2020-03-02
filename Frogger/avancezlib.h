#pragma once



#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <iostream>

enum class H_ALIGN { LEFT, CENTER, RIGHT };
enum class V_ALIGN { TOP, CENTER, BOT };
enum class DIRECTION { NONE, LEFT, RIGHT, UP, DOWN };

class Sprite
{
    SDL_Renderer * renderer;
    SDL_Texture * texture;

public:

    Sprite(SDL_Renderer * renderer, SDL_Texture * texture);

    // Destroys the sprite instance
    void destroy();

    // Draw the sprite at the given position.
    // Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
    // (All sprites are 32*32 pixels, clipping is not supported)
    void draw(int x, int y);
    void draw(int x, int y, SDL_RendererFlip flip);
    void drawStretched(int x, int y, float scale_x, float scale_y);
    unsigned int getImageWidth();
    unsigned int getImageHeight();
};

class AvancezLib
{
public:
    
    // Destroys the avancez library instance
    void destroy();

    // Destroys the avancez library instance and exits
    void quit();

    // Creates the main window. Returns true on success.
    bool init(int width, int height);

    // Clears the screen and draws all sprites and texts which have been drawn
    // since the last update call.
    // If update returns false, the application should terminate.
    void processInput();

    void swapBuffers();

    void clearWindow();

    // Create a sprite given a string.
    // All sprites are 32*32 pixels.
    Sprite* createSprite(const char* name);

    // Draws the given text.
    void drawText(int x, int y, const char* msg, H_ALIGN halign, V_ALIGN valign, SDL_Color col);
    void drawText(int x, int y, const char* msg, H_ALIGN halign, V_ALIGN valign, SDL_Color col, SDL_Color bg);
    
    // Draws a rectangle
    void drawRect(int x0, int y0, int x1, int y1, SDL_Color col, bool border);

    // Return the total time spent in the game, in seconds.
    float getElapsedTime();

    struct KeyStatus
    {
        bool fire;  // space
        bool left;  // left arrow
        bool right; // right arrow
        bool up;    // up arrow
        bool down;  // down arrow
        bool restart; // r-key
        bool pause;   // p-key
        bool esc;     // escape button
    };

    // Returns the keyboard status. If a flag is set, the corresponding key is being held down.
    void getKeyStatus(KeyStatus& keys);
    
    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    GLint compileShader(const char* filename, GLenum type);
    GLint compileShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    
private:
    SDL_Window * window;
    SDL_Renderer * renderer;
    GLint shader;
    
    SDL_Texture *texTarget;
    GLint  gVertexPos2DLocation;

    TTF_Font* font;

    KeyStatus key;
    int width, height;
};

