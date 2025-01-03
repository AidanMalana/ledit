#include "util.h"
#include "raylib.h"
#include <stdio.h>
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

Tile* LoadTilemap(Tile *tilemap, FILE *mapFile, char *mapFileName, int ww, int wh, int tileSize) {
    mapFile = fopen(mapFileName, "r");
    tilemap = calloc(ww * wh, sizeof(Tile));
    char line[1024];
    int lineCount = 0;
    int tmpX, tmpY, tmpZ;
    while (fgets(line, sizeof line, mapFile) != NULL) {
        if (lineCount > 0) {
            // I think the better way to do this would be to use JSON or something, or at the very least use strtol, but I just want to try this for now
            sscanf(line, "%d %d %d", &tmpX, &tmpY, &tmpZ); // X and Y will need to be in units of tiles not pixels
            // I never actually use the x or y for anything (hopefully)
            tilemap[(int)tmpX * wh + (int)tmpY].x = tmpX;
            tilemap[(int)tmpX * wh + (int)tmpY].y = tmpY;
            tilemap[(int)tmpX * wh + (int)tmpY].rect.x = tmpX * tileSize;
            tilemap[(int)tmpX * wh + (int)tmpY].rect.y = tmpY * tileSize;
            tilemap[(int)tmpX * wh + (int)tmpY].rect.width = tileSize;
            tilemap[(int)tmpX * wh + (int)tmpY].rect.height = tileSize;
            tilemap[(int)tmpX * wh + (int)tmpY].tile = (int)tmpZ;
        }
        lineCount++;
    }
    fclose(mapFile);
    mapFile = NULL;

    return tilemap;
}