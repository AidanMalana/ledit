// CURRENT ERRORS/BUGS
// HANDLE CASES WHERE MEMORY ALLOCATION MAY RETURN NULL
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void UpdateDrawFrame(void);     // Update and Draw one frame
void DrawMouse(RenderTexture2D); // Draw the mouse 
void saveMap();
void resizeTileMap();

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
int selectedTile = 0;
Rectangle *tileset;
Texture2D tilesetTexture;

int worldWidth = 10;
int worldHeight = 10;
Tile *tilemap; 
FILE *mapFile;
char *mapFileName;

// GUI
bool showResizeWorldPanel = false;
int selectedWorldWidth;
int selectedWorldHeight;
bool widthBoxSelected;
bool heightBoxSelected;


//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "ledit - Legato Editor");
    renderTarget = LoadRenderTexture(virtualWidth, virtualHeight);
    HideCursor();

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
        } else {
            mapFile = fopen("levels/level.txt", "r");
            fscanf(mapFile, "%d %d %d", &worldWidth, &worldHeight, &tileSize);
            fclose(mapFile);
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
    tilemap = calloc(worldHeight * worldWidth, sizeof(Tile));
    char line[1024];
    int lineCount = 0;
    int tmpX, tmpY, tmpZ;
    while (fgets(line, sizeof line, mapFile) != NULL) {
        if (lineCount > 0) {
            // I think the better way to do this would be to use JSON or something, or at the very least use strtol, but I just want to try this for now
            sscanf(line, "%d %d %d", &tmpX, &tmpY, &tmpZ); // X and Y will need to be in units of tiles not pixels
            // I never actually use the x or y for anything (hopefully)
            tilemap[(int)tmpX * worldHeight + (int)tmpY].x = tmpX;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].y = tmpY;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.x = tmpX * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.y = tmpY * tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.width = tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].rect.height = tileSize;
            tilemap[(int)tmpX * worldHeight + (int)tmpY].tile = (int)tmpZ;
        }
        lineCount++;
    }
    fclose(mapFile);
    mapFile = NULL;

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_Q);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Select tiles to use
        switch (GetKeyPressed()) {
            case KEY_ZERO: 
                selectedTile = 0;
                break;
            case KEY_ONE: 
                selectedTile = 1;
                break;
            case KEY_TWO: 
                selectedTile = 2;
                break;
            case KEY_THREE: 
                selectedTile = 3;
                break;
            case KEY_FOUR: 
                selectedTile = 4;
                break;
            case KEY_FIVE: 
                selectedTile = 5;
                break;
            case KEY_SIX: 
                selectedTile = 6;
                break;
            case KEY_SEVEN: 
                selectedTile = 7;
                break;
            case KEY_EIGHT: 
                selectedTile = 8;
                break;
            case KEY_NINE: 
                selectedTile = 9;
                break;
            case KEY_E:
                selectedTile = 9;
                break;
        }

        // Show resize panel
        if (IsKeyPressed(KEY_ESCAPE) && !showResizeWorldPanel) {
            showResizeWorldPanel = true;
            selectedWorldWidth = worldWidth;
            selectedWorldHeight = worldHeight;
            widthBoxSelected = false;
            heightBoxSelected = false;
            ShowCursor();
        }

        // Save map to file 
        if (IsKeyPressed(KEY_S)) {
            saveMap();
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            int x = round((mousePos.x)/scale/tileSize);
            int y = round((mousePos.y)/scale/tileSize);
            if (x < worldWidth && y < worldHeight) {
                tilemap[x * worldHeight + y].tile = selectedTile;
            }
        }
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
        DrawTilemap(renderTarget, tilesetTexture, tileset, tilemap, worldWidth, worldHeight, tileSize);
        DrawMouse(renderTarget);
        
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
        
        if (showResizeWorldPanel) {
            Rectangle windowBox = (Rectangle) {screenWidth/2 - 160, screenHeight/2 - 90, 320, 180};
            int result = GuiWindowBox(windowBox, "Resize World");
            if (result > 0) showResizeWorldPanel = false; // If window close button clicked
            /*Seems to work, but I need to add to the map.
            Currently, if I set it higher than the current width stored in the level file, it crashes, due to what I assume is an indexOutOfBounds thing. 
            */ 
            Rectangle widthBox = (Rectangle) {windowBox.x + 48, windowBox.y + 48, 248, 24};
            Rectangle heightBox = (Rectangle) {windowBox.x + 48, windowBox.y + 96, 248, 24};

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                widthBoxSelected = false;
                heightBoxSelected = false;
                if (CheckCollisionPointRec(GetMousePosition(), widthBox)) {
                    widthBoxSelected = true;
                }
                if (CheckCollisionPointRec(GetMousePosition(), heightBox)) {
                    heightBoxSelected = true;
                }
            }
            GuiValueBox(widthBox, "Width: ", &selectedWorldWidth, 1, 100, widthBoxSelected);  
            GuiValueBox(heightBox, "Height: ", &selectedWorldHeight, 1, 100, heightBoxSelected);  
            int apply = GuiButton((Rectangle){windowBox.x + 24, windowBox.y + windowBox.height - 48, 64, 24}, "Apply");
            if (apply == 1) {
                resizeTileMap();
            }
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void DrawMouse(RenderTexture2D target) {
    Vector2 mousePos = GetMousePosition();
    DrawRectangle(round((mousePos.x)/scale/tileSize)*tileSize-1, round((mousePos.y)/scale/tileSize)*tileSize-1, tileSize+2, tileSize+2, YELLOW);
    DrawTextureRec(tilesetTexture, tileset[selectedTile], (Vector2) {round((mousePos.x)/scale/tileSize)*tileSize, round((mousePos.y)/scale/tileSize)*tileSize}, WHITE);
}

void saveMap() {
    mapFile = fopen(mapFileName, "w");
    fprintf(mapFile, "%d %d %d\n", worldWidth, worldHeight, tileSize);
    for (int i = 0; i < worldWidth; i++) {
        for (int j = 0; j < worldHeight; j++) {
            fprintf(mapFile, "%d %d %d\n", i, j, tilemap[i * worldHeight + j].tile);
        }
    }
    fclose(mapFile);
    mapFile = NULL;
}

void resizeTileMap()
{
    if (selectedWorldWidth <= worldWidth && selectedWorldHeight <= worldHeight) {
        Tile *newTileMap = calloc(selectedWorldWidth * selectedWorldHeight, sizeof(Tile));
        for (int i = 0; i < selectedWorldWidth; i++) {
            for (int j = 0; j < selectedWorldHeight; j++) {
                newTileMap[i * selectedWorldHeight + j] = tilemap[i * worldHeight + j];
            }
        }

        if (tilemap != NULL) {
            free(tilemap);
        }
        tilemap = NULL;
        tilemap = newTileMap;
    } else {
        Tile *newTileMap = calloc(selectedWorldWidth * selectedWorldHeight, sizeof(Tile));
        for (int i = 0; i < worldWidth; i++) {
            for (int j = 0; j < worldHeight; j++) {
                newTileMap[i * selectedWorldHeight + j] = tilemap[i * worldHeight + j];
            }
        }

        if (tilemap != NULL) {
            free(tilemap);
        }
        tilemap = NULL;

        /* Cover all cases/tiles. There might be a more effecient solution but this works*/
        if (selectedWorldWidth > worldWidth && selectedWorldHeight <= worldHeight) {
            for (int i = worldWidth; i < selectedWorldWidth; i++) {
                for (int j = 0; j < selectedWorldHeight; j++) {
                    newTileMap[i * selectedWorldHeight + j].x = i;
                    newTileMap[i * selectedWorldHeight + j].y = j;
                    newTileMap[i * selectedWorldHeight + j].rect.x = i * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.y = j * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.width = tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.height = tileSize;
                    newTileMap[i * selectedWorldHeight + j].tile = defaultTile;
                }
            }
        } else if (selectedWorldWidth <= worldWidth && selectedWorldHeight > worldHeight) {
            for (int i = 0; i < selectedWorldWidth; i++) {
                for (int j = worldHeight; j < selectedWorldHeight; j++) {
                    newTileMap[i * selectedWorldHeight + j].x = i;
                    newTileMap[i * selectedWorldHeight + j].y = j;
                    newTileMap[i * selectedWorldHeight + j].rect.x = i * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.y = j * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.width = tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.height = tileSize;
                    newTileMap[i * selectedWorldHeight + j].tile = defaultTile;
                }
            }
        } else if (selectedWorldWidth > worldWidth && selectedWorldHeight > worldHeight) { // Doesn't seem to fill all tiles
            for (int i = worldWidth; i < selectedWorldWidth; i++) {
                for (int j = 0; j < selectedWorldHeight; j++) {
                    newTileMap[i * selectedWorldHeight + j].x = i;
                    newTileMap[i * selectedWorldHeight + j].y = j;
                    newTileMap[i * selectedWorldHeight + j].rect.x = i * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.y = j * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.width = tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.height = tileSize;
                    newTileMap[i * selectedWorldHeight + j].tile = defaultTile;
                }
            }
            for (int i = 0; i < selectedWorldWidth; i++) {
                for (int j = worldHeight; j < selectedWorldHeight; j++) {
                    newTileMap[i * selectedWorldHeight + j].x = i;
                    newTileMap[i * selectedWorldHeight + j].y = j;
                    newTileMap[i * selectedWorldHeight + j].rect.x = i * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.y = j * tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.width = tileSize;
                    newTileMap[i * selectedWorldHeight + j].rect.height = tileSize;
                    newTileMap[i * selectedWorldHeight + j].tile = defaultTile;
                }
            }
        }

        tilemap = newTileMap;
    }

    worldWidth = selectedWorldWidth;
    worldHeight = selectedWorldHeight;
    saveMap();
}