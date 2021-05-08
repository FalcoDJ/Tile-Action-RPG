#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <vector>
#include "../FileChecker.hpp"
#include "../olcPGEX_TransformedView.h"

enum class tilebitmap {
    TLeft, TMiddle, TRight,
    MLeft, MMiddle, MRight,
    BLeft, BMiddle, BRight,
};

enum tileBool {
    enabled=1,
    disabled=0
};

class Tile
{
public:
    Tile(){}
    Tile(tileBool e) : state(e) {}
    ~Tile(){}
    tileBool state;
};

class TileMap
{
private:
    std::vector<Tile> m_TileVector;
    int m_Width;
    int m_Height;

public: 
    std::string path_to_spritesheet;
    olc::Sprite* SpriteSheet = nullptr;
    olc::Decal* SpriteSheetDecal = nullptr;
    olc::Sprite* BufferSprite = nullptr;
    olc::Decal* BufferDecal = nullptr;

    tileBool defaultStateForTile = disabled;
    float tileSize;

public:
    TileMap(){}
    ~TileMap(){}

    bool Setup(int width, int height, olc::PixelGameEngine* enginePointer)
    {
        bool results = true;
    
        m_Width = width;
        m_Height = height;

        if (!file::doesExist(path_to_spritesheet))
        {
            results = false;
        }
        else
        {
            SpriteSheet = new olc::Sprite(path_to_spritesheet);
            SpriteSheetDecal = new olc::Decal(SpriteSheet);
        }

        for (int i = 0; i < width * height; i++)
        {
            m_TileVector.push_back(Tile(defaultStateForTile));
        }

        return results;
    }

    void CreateBuffer(olc::PixelGameEngine* pge, olc::TileTransformedView* tv)
    {
        if (BufferSprite != nullptr)
        delete BufferSprite;

        BufferSprite = new olc::Sprite(m_Width * tv->GetWorldScale().x,m_Height * tv->GetWorldScale().y);
        pge->SetDrawTarget(BufferSprite);
        // pge->SetPixelMode(olc::Pixel::ALPHA);
        pge->Clear(olc::BLANK);
        
        tv->SetWorldOffset({0,0});
        
        DrawByTiles(tv, {0,0});

        pge->SetDrawTarget(nullptr);

        if (BufferDecal != nullptr)
        delete BufferDecal;

        BufferDecal = new olc::Decal(BufferSprite);
    }

    void DrawByTiles(olc::TileTransformedView* tv, olc::vf2d Camera_Offset)
    {
        
        for (int i = 0; i < m_Width * m_Height; i++)
        {
            tv->FillRect(index2xy(i) * tileSize - Camera_Offset, olc::vf2d(tileSize,tileSize), bool(m_TileVector[i].state) ? olc::WHITE : olc::BLANK);
        }
    }
    void DrawByBuffer(olc::TileTransformedView* tv, olc::vf2d Camera_Offset)
    {
        tv->DrawDecal(olc::vf2d(0,0) - Camera_Offset, BufferDecal);
    }

    bool boundsCheck(int x, int y) {
        // probably off by 1, untested
        return (x >= 0 && x <= this->m_Width-1 && y >= 0 && y <= this->m_Height-1);
    }

    bool boundsCheck(int index) {
        // probably off by 1, untested
        olc::vf2d location = index2xy(index);
        return (location.x >= 0 && location.x <= this->m_Width && location.y >= 0 && location.y <= this->m_Height);
    }

    int xy2index(int x, int y)
    {
        return x + m_Width * y;
    }
    olc::vf2d index2xy(int index)
    {
        return olc::vf2d(index % m_Width, floor(index / m_Width));
    }
    Tile getTile(int index)
    {
        if (boundsCheck(index))
        return m_TileVector[index];
    }
    Tile getTile(int x, int y)
    {
        if (boundsCheck(x,y))
        return m_TileVector[xy2index(x,y)];
    }
    void setTileIndex(int index, tileBool mode)
    {
        if (boundsCheck(index))
        m_TileVector[index].state = mode;
    }
    void setTileXY(int x, int y, tileBool mode)
    {
        if (boundsCheck(x,y))
        setTileIndex(xy2index(x,y), mode);
    }

};

#endif