#include "util.h"
#include "raylib.h"
#include <stdlib.h>

// It shouldn't matter if the argument name matches the actual name, I think.
void DrawTilemap(RenderTexture2D target, Texture2D tt, Rectangle *ts, Tile *tilemap, int ww, int wh, int tSize) {
    DrawTexture(tt, 160, 90, WHITE);
    for (int i = 0; i < ww; i++) {
        for (int j = 0; j < wh; j++) {
            DrawTextureRec(tt, 
                ts[tilemap[i * wh + j].tile],
                (Vector2) {i * tSize, j * tSize}, 
                WHITE);
            
        }
    }
}

Rectangle* LoadTileset(char *path, int tileCount, int tileSize, int *defaultTile) {
    Rectangle *tileset = calloc(tileCount, sizeof(Rectangle)); 
    for (int i = 0; i < tileCount; i++) { // sizeof evaluates the size of the type, not the size of the array. sizeof() only works for statically declared arrays
        tileset[i].width = tileSize;
        tileset[i].height = tileSize;
        tileset[i].x = i % 3 * tileSize;
        tileset[i].y = i / 3 * tileSize;
    }
    *defaultTile = tileCount--;
    return tileset;
}