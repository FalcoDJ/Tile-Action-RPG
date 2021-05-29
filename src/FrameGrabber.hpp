#ifndef SPRITEANIMATOR_HPP
#define SPRITEANIMATOR_HPP

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

void DrawDecalFrame(olc::TileTransformedView *tv, olc::Decal *Decal, olc::vf2d Pos, olc::vf2d SpriteSize, olc::vi2d framesPer, int currentFrame, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
{
  int hfl = 1;
  if (flip == olc::Sprite::Flip::HORIZ)
  {
    hfl = -1;
    Pos.x += SpriteSize.x;
  }

  int spriteSheetRow = (currentFrame / framesPer.x);
  int spriteSheetColumn = (currentFrame % framesPer.x);
  int spriteSheetX = (spriteSheetColumn * SpriteSize.x);
  int spriteSheetY = (spriteSheetRow * SpriteSize.y);

  tv->DrawPartialDecal({(float)Pos.x, (float)Pos.y}, Decal, {float(spriteSheetX),float(spriteSheetY)}, {float(SpriteSize.x), float(SpriteSize.y)}, {hfl,1});
}

void DrawDecalFrame(olc::PixelGameEngine *pge, olc::Decal *Decal, olc::vf2d Pos, olc::vf2d SpriteSize, olc::vi2d framesPer, int currentFrame, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
{
  int hfl = 1;
  if (flip == olc::Sprite::Flip::HORIZ)
  {
    hfl = -1;
    Pos.x += SpriteSize.x;
  }

  int spriteSheetRow = (currentFrame / framesPer.x);
  int spriteSheetColumn = (currentFrame % framesPer.x);
  int spriteSheetX = (spriteSheetColumn * SpriteSize.x);
  int spriteSheetY = (spriteSheetRow * SpriteSize.y);

  pge->DrawPartialDecal({(float)Pos.x, (float)Pos.y}, Decal, {float(spriteSheetX),float(spriteSheetY)}, {float(SpriteSize.x), float(SpriteSize.y)}, {hfl,1});
}

void DrawSpriteFrame(olc::PixelGameEngine *pge, olc::vf2d pos, olc::Sprite *sprite_sheet, olc::vi2d frame_size, int frames_per_row, int frame_number)
{
  olc::vf2d source_pos = olc::vi2d(frame_number % frames_per_row, frame_number / frames_per_row) * frame_size;
  pge->DrawPartialSprite(pos, sprite_sheet, source_pos, frame_size);
  // pge->DrawPartialSprite(index2xy(i) * tv->GetWorldScale(), SpriteSheet, {0,0}, {16,16});
  // pge->DrawPartialSprite({(float)Pos.x, (float)Pos.y}, Sprite, {float(spriteSheetX),float(spriteSheetY)}, {float(SpriteSize.x), float(SpriteSize.y)}, {hfl,1});
}

#endif
