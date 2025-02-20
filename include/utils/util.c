#include "util.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>

// void DrawTilemap() {
// }

// Rectangle *LoadTileset() {
// }

// Tile *LoadTilemap() {}

void UpdateActor(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight) {
  ActorMoveX(actor, tilemap, worldWidth, worldHeight);
  ActorMoveY(actor, tilemap, worldWidth, worldHeight);
}

void ActorMoveX(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight) {
  actor->moveRemainder.x += actor->velocity.x;
  int move = round(actor->moveRemainder.x);
  if (move != 0) {
    actor->moveRemainder.x -= move;
    float sign = move > 0 ? 1 : -1;
    while (move != 0) {
      if (!ActorCollideAt(actor, tilemap, worldWidth, worldHeight, (Rectangle){actor->body.x + sign, actor->body.y, actor->body.width, actor->body.height})) {
        actor->pos.x += sign;
        actor->body.x = actor->pos.x;
        actor->body.y = actor->pos.y;
        move -= sign;
      } else {
        actor->velocity.x = 0;
        break;
      }
    }
  }
}

void ActorMoveY(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight) {
  actor->moveRemainder.y += actor->velocity.y;
  int move = round(actor->moveRemainder.y);
  if (move != 0) {
    actor->moveRemainder.y -= move;
    float sign = move > 0 ? 1 : -1;
    while (move != 0) {
      if (!ActorCollideAt(actor, tilemap, worldWidth, worldHeight, (Rectangle){actor->body.x, actor->body.y + sign, actor->body.width, actor->body.height})) {
        actor->pos.y += sign;
        actor->body.x = actor->pos.x;
        actor->body.y = actor->pos.y;
        move -= sign;
      } else {
        actor->velocity.y = 0;
        actor->canJump = true;
        break;
      }
    }
  }
}

bool ActorCollideAt(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight, Rectangle actorCollider) {
  Vector2 tileCoordinates = PixelToTileCoordinates(actor->pos, 8);
  for (int i = 0; i < worldWidth; i++) {
    for (int j = 0; j < worldHeight; j++) {
      if (CheckCollisionRecs(actorCollider, tilemap[i * worldHeight + j].rect) && tilemap[i * worldHeight + j].tile != 9)
        return true;
    }
  }
  return false;
}
