#include "raylib.h"
#include <stdio.h>

#ifndef UTIL_H
#define UTIL_H

typedef struct {
  Rectangle rect;
  int tile; // Use a spritemap to draw tiles, don't assign each tile an
            // individual texture
} Tile;

typedef struct {
  Vector2 pos;
  Rectangle size;
  Tile tile;
} Cell;

typedef struct {
  Vector2 pos;
  Vector2 moveRemainder;
  Vector2 velocity;
  Vector2 maxVelocity;
  bool canJump;
  Rectangle body;
} Actor;

void DrawTilemap();
Rectangle *LoadTileset();
Tile *LoadTilemap();

void UpdateActor(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight);
void ActorMoveX(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight);
void ActorMoveY(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight);
bool ActorCollideAt(Actor *actor, Tile *tilemap, int worldWidth, int worldHeight, Rectangle actorCollider);

#endif
