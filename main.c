// TODO:
// - Refactor into new Tile -> Cell -> Map format
// - Create mode for loading a tileset
// - Set destination directory for level
// - Rename level
//
// CURRENT ERRORS/BUGS:
// HANDLE CASES WHERE MEMORY ALLOCATION MAY RETURN NULL
#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "include/utils/util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UpdateDrawFrame(void);       // Update and Draw one frame
void DrawMouse(Vector2 mousePos); // Draw the mouse
void saveMap();
void resizeTileMap();

const int screenWidth = 1600;
const int screenHeight = 900;

const int virtualWidth = 320;
const int virtualHeight = 180;
// set scale to smaller value
double scale = screenWidth / virtualWidth > screenHeight / virtualHeight ? screenHeight / virtualHeight : screenWidth / virtualWidth;

RenderTexture2D renderTarget;
Camera2D cam;

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

//-----------------------------------------------------------------------------
// Main Entry Point
//-----------------------------------------------------------------------------
int main() {
  // Initialization
  //---------------------------------------------------------------------------
  InitWindow(screenWidth, screenHeight, "ledit - Legato Editor");
  renderTarget = LoadRenderTexture(virtualWidth, virtualHeight);
  while (!WindowShouldClose()) {
  }
  CloseWindow();
}
