#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
    Rectangle rect;
    Rectangle textureRec; // Use a spritemap to draw tiles, don't assign each tile an individual texture
} Tile;

void UpdateDrawFrame(void);     // Update and Draw one frame
void DrawTilemap(RenderTexture2D, Tile*);         // Draw the tilemap

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1600;
const int screenHeight = 900;

const int virtualWidth = 320;
const int virtualHeight = 180;
// set scale to smaller value
double scale = screenWidth / virtualWidth > screenHeight / virtualHeight ? screenHeight / virtualHeight : screenWidth / virtualWidth; 

RenderTexture2D renderTarget;

int tileSize = 8; // Square for now, add support for rectangles or different shapes later?
int tileCount = 10;
Rectangle *tileset;
Texture2D tilesetTexture;

int worldWidth = 10;
int worldHeight = 10;
Tile *tilemap; // Set default to 100, resize world using some kind of prompt and with realloc?


//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "ledit - Legato Editor");
    renderTarget = LoadRenderTexture(virtualWidth, virtualHeight);

    tilesetTexture = LoadTexture("resources/art/tiles.png");
    tileset = calloc(tileCount, sizeof(Rectangle)); 
        for (int i = 0; i < tileCount; i++) { // sizeof evaluates the size of the type, not the size of the array. sizeof() only works for statically declared arrays
        tileset[i].width = tileSize;
        tileset[i].height = tileSize;
        tileset[i].x = i % 3 * tileSize;
        tileset[i].y = i / 3 * tileSize;
    }

    tilemap = calloc(100, sizeof(Tile));
    for (int i = 0; i < worldWidth; i++) {
        for (int j = 0; j < worldHeight; j++) {
            tilemap[i * worldHeight + j].x = i * tileSize;
            tilemap[i * worldHeight + j].y = j * tileSize;
            tilemap[i * worldHeight + j].rect.x = i * tileSize;
            tilemap[i * worldHeight + j].rect.y = j * tileSize;
            tilemap[i * worldHeight + j].rect.width = tileSize;
            tilemap[i * worldHeight + j].rect.height = tileSize;
            tilemap[i * worldHeight + j].textureRec = tileset[1];
        }
    }

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(tileset);
    free(tilemap);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(renderTarget);

        ClearBackground(RAYWHITE);
        DrawTilemap(renderTarget, tilemap);
        DrawText("Congrats! You created your first window!", 24, 90, 12, LIGHTGRAY);
        
    EndTextureMode();

    BeginDrawing();

        ClearBackground(SKYBLUE);
        // (Texture, Source, Dest, Origin, Rotation, Tint)
        DrawTexturePro(renderTarget.texture, 
            (Rectangle) {0, 0, renderTarget.texture.width, -renderTarget.texture.height},
            (Rectangle) {0, 0, renderTarget.texture.width * scale, -renderTarget.texture.height * scale},
            (Vector2) {0, 0}, 
            0.0f,
            WHITE);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void DrawTilemap(RenderTexture2D target, Tile *tilemap) {
    DrawTexture(tilesetTexture, 160, 90, WHITE);
    for (int i = 0; i < worldWidth; i++) {
        for (int j = 0; j < worldHeight; j++) {
            DrawTextureRec(tilesetTexture, 
                tilemap[i * worldHeight + j].textureRec,
                (Vector2) {i * tileSize, j * tileSize}, 
                WHITE);
            // DrawTextureRec(tilesetTexture, 
            //     (Rectangle) {0, 0, tileSize, tileSize},
            //     (Vector2) {i * tileSize, j * tileSize}, 
            //     WHITE);
        }
    }
}