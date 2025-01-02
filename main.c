#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
int defaultTile = 9; // Sky
Rectangle *tileset;
Texture2D tilesetTexture;

int worldWidth = 10;
int worldHeight = 10;
Tile *tilemap; 
FILE *mapFile;
char *mapFileName;


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

    mapFileName = malloc(100);
    if (DirectoryExists("levels")) {
        if (!FileExists("levels/level.txt")) {
            mapFile = fopen("levels/level.txt", "w");
            fprintf(mapFile, "%d %d %d\n", worldWidth, worldHeight, tileSize);
            for (int i = 0; i < worldWidth; i++) {
                for (int j = 0; j < worldHeight; j++) {
                    fprintf(mapFile, "%d %d %d\n", i, j, defaultTile); // fill map with default tiles
                }
            }
            fclose(mapFile);
            mapFile = NULL;
        }
        strcpy(mapFileName, "levels/level.txt");
    } else {
        MakeDirectory("levels");
        mapFile = fopen("levels/level.txt", "w");
        fprintf(mapFile, "%d %d %d\n", worldWidth, worldHeight, tileSize);
        for (int i = 0; i < worldWidth; i++) {
            for (int j = 0; j < worldHeight; j++) {
                fprintf(mapFile, "%d %d %d\n", i, j, defaultTile); // fill map with default tiles
            }
        }
        strcpy(mapFileName, "levels/level.txt");
        fclose(mapFile);
        mapFile = NULL;
    }
    mapFile = fopen(mapFileName, "r");
    tilemap = calloc(100, sizeof(Tile));
    char line[1024];
    int lineCount = 0;
    int tmpX, tmpY, tmpZ;
    while (fgets(line, sizeof line, mapFile) != NULL) {
        if (lineCount > 0) {
            // I think the better way to do this would be to use JSON or something, or at the very least use strtol, but I just want to try this for now
            sscanf(line, "%d %d %d", &tmpX, &tmpY, &tmpZ); // X and Y will need to be in units of tiles not pixels
            tilemap[(int)tmpX * worldHeight + (int)tmpY].x = tmpX * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].y = tmpY * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.x = tmpX * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.y = tmpY * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.width = tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.height = tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].textureRec = tileset[(int)tmpZ];
        }
        lineCount++;
    }
    // for (int i = 0; i < worldWidth; i++) {
    //     for (int j = 0; j < worldHeight; j++) {
    //         tilemap[i * worldHeight + j].x = i * tileSize;
    //         tilemap[i * worldHeight + j].y = j * tileSize;
    //         tilemap[i * worldHeight + j].rect.x = i * tileSize;
    //         tilemap[i * worldHeight + j].rect.y = j * tileSize;
    //         tilemap[i * worldHeight + j].rect.width = tileSize;
    //         tilemap[i * worldHeight + j].rect.height = tileSize;
    //         tilemap[i * worldHeight + j].textureRec = tileset[2];
    //     }
    // }
    fclose(mapFile);
    mapFile = NULL;

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    if (tileset != NULL) {
        free(tileset);
    }
    if (tilemap != NULL) {
        free(tilemap);
    }
    if (mapFileName != NULL) {
        free(mapFileName);
    }
    if (mapFile != NULL) {
        fclose(mapFile);
    }
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