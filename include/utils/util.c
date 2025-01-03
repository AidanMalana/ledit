#include "util.h"

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