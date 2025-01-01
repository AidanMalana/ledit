#include "raylib.h"

typedef struct {
    int x;
    int y;
    Rectangle rect;
    Texture2D texture;
} Tile;
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

Texture2D texture;
//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "ledit - Legato Editor");
    renderTarget = LoadRenderTexture(virtualWidth, virtualHeight);

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second

    texture = LoadTexture("resources/art/tiles.png");
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
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
        DrawTexture(texture, 0, 0, WHITE);
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
