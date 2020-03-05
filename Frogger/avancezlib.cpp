#include "avancezlib.h"
#include "util.h"

bool AvancezLib::init(int width, int height)
{
    SDL_Log("Initializing the engine...\n");
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
        return false;
    }

    //Create window
    window = SDL_CreateWindow("Can it play FROGGER?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Init();
    font = TTF_OpenFont("/Users/larsa/Chalmers/TDA572/Data/misc/PressStart2P.ttf", 16); //this opens a font style and sets a size
    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    // initialize the keys
    key.fire  = false;
    key.left  = false;
    key.right = false;
    key.up    = false;
    key.down  = false;
    key.esc   = false;
    key.pause = false;
    key.opt0  = false;
    key.opt1  = false;
    key.opt2  = false;
    key.restart = false;
    
    // Enable Post Processing
    enable_post_processing = true;

    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear screen
    SDL_RenderClear(renderer);

    this->width = width;
    this->height = height;

    SDL_Log("Engine up and running...\n");
    return true;
}


// Destroys the avancez library instance
void AvancezLib::destroy()
{
    SDL_Log("Shutting down the engine\n");
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}

void AvancezLib::quit() {
    destroy();
    exit(0);
}


void AvancezLib::processInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            case SDLK_q:
                key.esc = true;
                break;
            case SDLK_SPACE:
                key.fire = true;
                break;
            case SDLK_LEFT:
                key.left = true;
                break;
            case SDLK_UP:
               key.up    = true;
               break;
            case SDLK_DOWN:
                key.down = true;
                break;
            case SDLK_RIGHT:
                key.right = true;
                break;
            case SDLK_r:
                key.restart = true;
                break;
            case SDLK_p:
                key.pause = true;
                break;
            case SDLK_o:
                key.opt0 = true;
                break;
            }
        }

        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                key.fire = false;
                break;
            case SDLK_LEFT:
                key.left = false;
                break;
            case SDLK_RIGHT:
                key.right = false;
                break;
            case SDLK_UP:
               key.up    = false;
               break;
            case SDLK_DOWN:
                key.down = false;
                break;
            case SDLK_r:
                key.restart = false;
                break;
            case SDLK_p:
                key.pause = false;
                break;
            case SDLK_o:
                key.opt0 = false;
                break;
            }
        }
        
        if (event.type == SDL_QUIT)
        {
            quit();
        }

    }
}

void AvancezLib::togglePostProcessing() {
    enable_post_processing = !enable_post_processing;
}

void AvancezLib::postProcessing() {
    if (!enable_post_processing) return;
    
    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        Uint32 rmask = 0xff000000;
        Uint32 gmask = 0x00ff0000;
        Uint32 bmask = 0x0000ff00;
        Uint32 amask = 0x000000ff;
    #else
        Uint32 rmask = 0x000000ff;
        Uint32 gmask = 0x0000ff00;
        Uint32 bmask = 0x00ff0000;
        Uint32 amask = 0xff000000;
    #endif
    
    SDL_Surface *source = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, source->pixels, source->pitch);
    SDL_Surface *copy   = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    SDL_BlitSurface(source,NULL,copy,NULL);
    SDL_RenderClear(renderer);
    float centerX = width/2, centerY = height/2;
    /* This is my Post processing */
    int warp_y0 = irandom(height-1);
    int warp_y1 = (int)(SDL_GetTicks() / 10.f) % height-1;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            
            /* Chromatic Aberration */
            int offsetX = 5 * ((x - centerX) / centerX);
            int offsetY = 2 * ((y - centerY) / centerY);
            int yy1 = clamp(y-offsetY, 0, height);
            int yy2 = clamp(y+offsetY, 0, height);
            int xx1 = clamp(x-offsetX, 0, width-5);
            int xx2 = clamp(x+offsetX, 0, width-5);
            
            ((int*)source->pixels)[(y * width) + x]  = (((int*)copy->pixels)[(y * width)   + x]   & 0x00FF0000);
            ((int*)source->pixels)[(y * width) + x] += (((int*)copy->pixels)[(yy1 * width) + xx1] & 0xFF000000);
            ((int*)source->pixels)[(y * width) + x] += (((int*)copy->pixels)[(yy2 * width) + xx2] & 0x0000FF00);
            
            /* Vignette */
            float vig = x * y * ( width - x ) * ( height - y ) * 8.0;
            vig = pow(vig, 0.25);
            int alpha = (int)clamp(vig, 0, 255);
            if (alpha == 0) alpha = 255;
            if (x == 0 || x == width || y == 0 || y == height) alpha = 0;
            ((int*)source->pixels)[(y * width) + x] += alpha;
            
            /* Noise (UGLY) */
            if (percentChance( .75 ) && x > 1 && x < width-1) {
                
                /* Amount of noise */
                int noise = 0x20202000;
                if (percentChance(33)) {
                    noise = 0x10101000;
                } else if (percentChance(50)) {
                    noise = 0x25252500;
                }
                
                /* Warp */
                ((int*)source->pixels)[(y * width) + x-2] = ((int*)source->pixels)[(y * width) + x-2] | noise;
                ((int*)source->pixels)[(y * width) + x-1] = ((int*)source->pixels)[(y * width) + x-1] | noise;
                ((int*)source->pixels)[(y * width) + x]   = ((int*)source->pixels)[(y * width) + x]   | noise;
                ((int*)source->pixels)[(y * width) + x+1] = ((int*)source->pixels)[(y * width) + x+1] | noise;
                ((int*)source->pixels)[(y * width) + x+2] = ((int*)source->pixels)[(y * width) + x+2] | noise;
            }
            
            /* Screen Tearing */
            if (y == warp_y0 || y == warp_y1) {
                ((int*)source->pixels)[(y * width) + x]   = ((int*)source->pixels)[(y * width) + x + 4];
            }
            
        }
    }
    
    //Create texture from surface pixels
    
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, source);
    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create a texture from the surface %s\n", SDL_GetError());
    }
    
    SDL_Rect rect; rect.x = 0; rect.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    
    // Render texture to screen
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    // Free memory
    SDL_FreeSurface(source);
    SDL_FreeSurface(copy);
}

void AvancezLib::swapBuffers() {
    // Update screen
    SDL_RenderPresent(renderer);
}

void AvancezLib::clearWindow() {
    // Clear screen
    SDL_RenderClear(renderer);
}


Sprite * AvancezLib::createSprite(const char * path)
{
    SDL_Surface* surf = SDL_LoadBMP(path);
    if (surf == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    //Create texture from surface pixels
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(surf);

    Sprite * sprite = new Sprite(renderer, texture);

    return sprite;
}

void AvancezLib::drawRect(int x0, int y0, int x1, int y1, SDL_Color col, bool border) {
    SDL_Rect rect;
    rect.x = x0;
    rect.y = y0;
    rect.w = x1-x0;
    rect.h = y1-y0;
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    if (border) SDL_RenderDrawRect(renderer, &rect);
    else  SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void AvancezLib::drawText(int x, int y, const char * msg, H_ALIGN halign, V_ALIGN valign, SDL_Color col)
{
    SDL_Surface* surf = TTF_RenderText_Solid(font, msg, col); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

    int w = 0, h = 0;
    SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
    if (halign == H_ALIGN::CENTER)   x -= w/2;
    else if (halign == H_ALIGN::RIGHT) x -= w;
    if (valign == V_ALIGN::CENTER)   y -= h/2;
    else if (valign == V_ALIGN::BOT)   y -= h;
    
    SDL_Rect dst_rect = { x, y, w, h };

    SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

    SDL_DestroyTexture(msg_texture);
    SDL_FreeSurface(surf);
}

void AvancezLib::drawText(int x, int y, const char * msg, H_ALIGN halign, V_ALIGN valign, SDL_Color col, SDL_Color bg)
{
    SDL_Surface* surf = TTF_RenderText_Solid(font, msg, col); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

    int w = 0, h = 0;
    SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
    if (halign == H_ALIGN::CENTER)   x -= w/2;
    else if (halign == H_ALIGN::RIGHT) x -= w;
    if (valign == V_ALIGN::CENTER)   y -= h/2;
    else if (valign == V_ALIGN::BOT)   y -= h;
    
    SDL_Rect dst_rect = { x, y, w, h };
    
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &dst_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

    SDL_DestroyTexture(msg_texture);
    SDL_FreeSurface(surf);
}

float AvancezLib::getElapsedTime()
{
    return SDL_GetTicks() / 1000.f;
}

void AvancezLib::getKeyStatus(KeyStatus & keys)
{
    keys.fire = key.fire;
    keys.left = key.left;
    keys.right = key.right;
    keys.up = key.up;
    keys.down = key.down;
    keys.esc = key.esc;
    keys.pause = key.pause;
    keys.opt0 = key.opt0;
    keys.opt1 = key.opt1;
    keys.opt2 = key.opt2;
    keys.restart = key.restart;
}


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
    this->renderer = renderer;
    this->texture = texture;
}

unsigned int Sprite::getImageWidth() {
    SDL_Rect rect;
    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    return rect.w;
}

unsigned int Sprite::getImageHeight() {
    SDL_Rect rect;
    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    return rect.h;
}

void Sprite::draw(int x, int y)
{
    draw(x, y, SDL_FLIP_NONE);
}

void Sprite::draw(int x, int y, SDL_RendererFlip flip)
{
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    
    //Render texture to screen
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, flip);
}

void Sprite::drawStretched(int x, int y, float scale_x, float scale_y)
{
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    
    rect.w *= scale_x;
    rect.h *= scale_y;
    
    //Render texture to screen
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Sprite::destroy()
{
    SDL_DestroyTexture(texture);
}
