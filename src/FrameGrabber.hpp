#ifndef SPRITEANIMATOR_HPP
#define SPRITEANIMATOR_HPP

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

void DrawSpriteFrame(olc::PixelGameEngine *pge, const olc::vf2d& pos, olc::Sprite *sprite_sheet, const olc::vi2d& frame_size, const int& frames_per_row, const int& frame_number)
{
  olc::vf2d source_pos = olc::vi2d(frame_number % frames_per_row, frame_number / frames_per_row) * frame_size;
  pge->DrawPartialSprite(pos, sprite_sheet, source_pos, frame_size);
}

#endif
