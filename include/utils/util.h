#include "raylib.h"

#ifndef UTIL_H
#define UTIL_H

typedef struct {
    int x;
    int y;
    Rectangle rect;
    int tile; // Use a spritemap to draw tiles, don't assign each tile an individual texture
} Tile;

void DrawTilemap(RenderTexture2D target, Texture2D tt, Rectangle *ts, Tile *tilemap, int ww, int wh, int tSize);
Rectangle* LoadTileset(char *path, int tileCount, int tileSize, int *defaultTile);

#endif