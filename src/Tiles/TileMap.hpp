#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <vector>
#include "../FileChecker.hpp"
#include "../FrameGrabber.hpp"
#include "../Shapes/Shapes.hpp"
#include "../FrameGrabber.hpp"

// These correspond to a tile in the sheet
enum TileBitMap {
    TL=0, TM=4, TR=5,
    ML=10, MM=9, MR=15,
    BL=40, BM=41, BR=45,
};

std::vector<int> AlternateMiddles = { 6, 7, 8, 9, 16, 17, 18, 19, 26, 27, 28, 29 };

class BITMAP 
{
public:
    BITMAP(bool b = true) : NW(b), N(b), NE(b), W(b), M(b), E(b), SW(b), S(b), SE(b) {}

    bool NW, N, NE,
         W,  M,  E,
         SW, S, SE;
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
    int frame = 2;
};

class TileMap
{
private:
    std::vector<Tile> m_TileVector;
    int m_Width;
    int m_Height;
    
private:
    void CreateBitMap()
    {
        for (int i = 0; i < m_TileVector.size(); i++)
        {
            srand(time(0) + i);

            olc::vi2d xyOfI = index2xy(i);

            int n  = xy2index(xyOfI + olc::vi2d(0, -1)); // Northern neighbor
            int ne = xy2index(xyOfI + olc::vi2d(1, -1));
            int nw = xy2index(xyOfI + olc::vi2d(-1,-1));
            int s  = xy2index(xyOfI + olc::vi2d(0,  1)); // Southern neighbor
            int se = xy2index(xyOfI + olc::vi2d(1,  1));
            int sw = xy2index(xyOfI + olc::vi2d(-1 ,1));
            int e  = xy2index(xyOfI + olc::vi2d(1,  0)); // Eastern  neighbor
            int w  = xy2index(xyOfI + olc::vi2d(-1, 0)); // Western  neighbor

            BITMAP bm(false);

            // If I exist
            if ((bool)m_TileVector[i].state)
            {
                bm.M = true;

                // Do I have a north neighbor
                if ((bool)m_TileVector[n].state)
                {
                    // adjust bitmap
                    bm.N = true;
                }

                // Do I have a south neighbor
                if ((bool)m_TileVector[s].state)
                {
                    // adjust bitmap
                    bm.S = true;
                }

                // Do I have a west neighbor
                if ((bool)m_TileVector[w].state)
                {
                    // adjust bitmap
                    bm.W = true;
                }

                // Do I have a east neighbor
                if ((bool)m_TileVector[e].state)
                {
                    // adjust bitmap
                    bm.E = true;
                }

                // Do I have a northeast neighbor
                if ((bool)m_TileVector[ne].state)
                {
                    // adjust bitmap
                    bm.NE = true;
                }

                // Do I have a northwest neighbor
                if ((bool)m_TileVector[nw].state)
                {
                    // adjust bitmap
                    bm.NW = true;
                }

                // Do I have a southeast neighbor
                if ((bool)m_TileVector[se].state)
                {
                    // adjust bitmap
                    bm.SE = true;
                }

                // Do I have a southwest neighbor
                if ((bool)m_TileVector[sw].state)
                {
                    // adjust bitmap
                    bm.SW = true;
                }
            }

            if (bm.NW && bm.N && bm.NE &&
                bm.W  && bm.M && bm.E  &&
                bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = 78;
            }
            else
            if (bm.NW && bm.N && bm.NE &&
                bm.W  && bm.M && bm.E  &&
                bm.SW && bm.S && !bm.SE)
            {
                m_TileVector[i].frame = TL;
            }
            else
            if (bm.NW && bm.N && bm.NE &&
                bm.W  && bm.M && bm.E  &&
                !bm.SW && !bm.S && !bm.SE)
            {
                m_TileVector[i].frame = TM;
            }
            else
            if (bm.NW && bm.N && bm.NE &&
                bm.W  && bm.M && bm.E  &&
                !bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = TR;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                !bm.W  && bm.M && bm.E  &&
                !bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                bm.W  && bm.M && bm.E  &&
                bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = BR;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                bm.W  && bm.M && bm.E  &&
                bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = BM;
            }
            else
            if (bm.NW && bm.N && !bm.NE &&
                bm.W  && bm.M && bm.E  &&
                bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = BL;
            }
            else
            if (bm.NW && bm.N && !bm.NE &&
                bm.W  && bm.M && !bm.E  &&
                bm.SW && bm.S && !bm.SE)
            {
                m_TileVector[i].frame = ML;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                !bm.W  && !bm.M && !bm.E  &&
                !bm.SW && !bm.S && !bm.SE)
            {
                m_TileVector[i].frame = AlternateMiddles[rand() % AlternateMiddles.size()];
            }
            else
            if (bm.NW &&  bm.N &&  bm.NE &&
                bm.W  &&  bm.M &&  bm.E  &&
                bm.SW && !bm.S && !bm.SE)
            {
                m_TileVector[i].frame = TM;
            }
            else
            if  (bm.NW &&  bm.N && bm.NE &&
                 bm.W  &&  bm.M && bm.E  &&
                !bm.SW && !bm.S && bm.SE)
            {
                m_TileVector[i].frame = TM;
            }
            else
            if (!bm.NW && !bm.N && bm.NE &&
                 bm.W  &&  bm.M && bm.E  &&
                 bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = BM;
            }
            else
            if (bm.NW && !bm.N && !bm.NE &&
                bm.W  &&  bm.M &&  bm.E  &&
                bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = BM;
            }
            else
            if (bm.NW && bm.N && !bm.NE &&
                bm.W  && bm.M && !bm.E  &&
                bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = ML;
            }
            else
            if (bm.NW && bm.N &&  bm.NE &&
                bm.W  && bm.M && !bm.E  &&
                bm.SW && bm.S && !bm.SE)
            {
                m_TileVector[i].frame = ML;
            }
            else
            if  (bm.NW && bm.N && bm.NE &&
                !bm.W  && bm.M && bm.E  &&
                !bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                !bm.W  && bm.M && bm.E  &&
                 bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                !bm.W  && bm.M && bm.E  &&
                 bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                 bm.W  &&  bm.M && !bm.E  &&
                 bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                !bm.W  &&  bm.M &&  bm.E  &&
                 bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (!bm.NW && !bm.N && bm.NE &&
                !bm.W  &&  bm.M && bm.E  &&
                !bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (bm.NW && !bm.N && !bm.NE &&
                bm.W  &&  bm.M && !bm.E  &&
                bm.SW &&  bm.S && !bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                !bm.W  &&  bm.M &&  bm.E  &&
                !bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                 bm.W  &&  bm.M && !bm.E  &&
                 bm.SW &&  bm.S && !bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && !bm.N && bm.NE &&
                !bm.W  &&  bm.M && bm.E  &&
                 bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (bm.NW && !bm.N && !bm.NE &&
                bm.W  &&  bm.M && !bm.E  &&
                bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                 bm.W  &&  bm.M &&  bm.E  &&
                !bm.SW &&  bm.S &&  bm.SE )
            {
                m_TileVector[i].frame = TM;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M && !bm.E  &&
                 bm.SW && bm.S && !bm.SE )
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M &&  bm.E  &&
                 bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = BM;
            }
            else
            if (bm.NW && bm.N && !bm.NE &&
                bm.W  && bm.M &&  bm.E  &&
                bm.SW && bm.S && !bm.SE)
            {
                m_TileVector[i].frame = ML;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                 bm.W  && bm.M && bm.E  &&
                !bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (!bm.NW && bm.N && bm.NE &&
                 bm.W  && bm.M && bm.E  &&
                !bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if ( bm.NW && bm.N && bm.NE &&
                !bm.W  && bm.M && bm.E  &&
                 bm.SW && bm.S && bm.SE)
            {
                m_TileVector[i].frame = MR;
            }
            else
            if (bm.NW && bm.N &&  bm.NE &&
                bm.W  && bm.M && !bm.E  &&
                bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = ML;
            }
            else
            if (bm.NW && !bm.N && !bm.NE &&
                bm.W  &&  bm.M &&  bm.E  &&
                bm.SW &&  bm.S && !bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && !bm.N && bm.NE &&
                 bm.W  &&  bm.M && bm.E  &&
                !bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M &&  bm.E  &&
                !bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (!bm.NW && !bm.N &&  bm.NE &&
                 bm.W  &&  bm.M &&  bm.E  &&
                 bm.SW &&  bm.S && !bm.SE  )
            {
                m_TileVector[i].frame = 53;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                !bm.W  && bm.M &&  bm.E  &&
                 bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M && !bm.E  &&
                 bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if ( bm.NW && bm.N &&  bm.NE &&
                !bm.W  && bm.M &&  bm.E  &&
                !bm.SW && bm.S && !bm.SE)
            {
                m_TileVector[i].frame = 61;
            }
            else
            if ( bm.NW && !bm.N && !bm.NE &&
                 bm.W  &&  bm.M &&  bm.E  &&
                !bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 60;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                 bm.W  &&  bm.M &&  bm.E  &&
                !bm.SW &&  bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 60;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                !bm.W  &&  bm.M && !bm.E  &&
                 bm.SW &&  bm.S && !bm.SE)
            {
                m_TileVector[i].frame = 60;
            }
            else
            if (!bm.NW && !bm.N && !bm.NE &&
                !bm.W  &&  bm.M && !bm.E  &&
                !bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = 60;
            }
            else
            if (!bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M && !bm.E  &&
                 bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 53;
            }
            else
            if ( bm.NW && bm.N && !bm.NE &&
                 bm.W  && bm.M &&  bm.E  &&
                !bm.SW && bm.S &&  bm.SE)
            {
                m_TileVector[i].frame = 50;
            }
            else
            if (bm.NW && !bm.N && bm.NE &&
                bm.W  &&  bm.M && bm.E  &&
                bm.SW &&  bm.S && bm.SE)
            {
                m_TileVector[i].frame = BM;
            }
            else
            if (!bm.W  &&  bm.M && !bm.E &&
                !bm.SW && !bm.S && !bm.SE )
            {
                m_TileVector[i].frame = TM;
            }
            else
            if (!bm.W && bm.M && !bm.E)
            {
                m_TileVector[i].frame = 61;
            }
            else
            if (!bm.M)
            {
                m_TileVector[i].frame = AlternateMiddles[rand() % AlternateMiddles.size()];
            }
        }
    }

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

    std::vector<Tile>& GetTileVector()
    {
        return m_TileVector;
    }

    bool Setup(int width, int height)
    {
        bool results = true;
    
        m_Width = width;
        m_Height = height;

        m_TileVector.clear();

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
        CreateBitMap();

        if (BufferSprite != nullptr)
        delete BufferSprite;

        BufferSprite = new olc::Sprite(m_Width * tv->GetWorldScale().x,m_Height * tv->GetWorldScale().y);
        pge->SetDrawTarget(BufferSprite);
        pge->Clear(olc::BLANK);
        
        tv->SetWorldOffset({0,0});
        
        // DrawByTiles(tv);
        for (int i = 0; i < m_Width * m_Height; i++)
            DrawSpriteFrame(pge, index2xy(i) * tv->GetWorldScale(), SpriteSheet, {16, 16}, 10, m_TileVector[i].frame);
            // tv->FillRect(index2xy(i) * tileSize, olc::vf2d(tileSize,tileSize), bool(m_TileVector[i].state) ? olc::WHITE : olc::BLANK);
                // pge->DrawPartialSprite(index2xy(i) * tv->GetWorldScale(), SpriteSheet, {0,0}, {16,16});

        pge->SetDrawTarget(nullptr);

        if (BufferDecal != nullptr)
        delete BufferDecal;

        BufferDecal = new olc::Decal(BufferSprite);
    }

    void DrawByBuffer(olc::TileTransformedView* tv)
    {
        tv->DrawDecal(olc::vf2d(0,0), BufferDecal);
    }
    
    void DrawByTiles(olc::TileTransformedView* tv)
    {
        
        for (int i = 0; i < m_Width * m_Height; i++)
        {
            tv->FillRect(index2xy(i) * tileSize, olc::vf2d(tileSize,tileSize), bool(m_TileVector[i].state) ? olc::WHITE : olc::BLANK);
            // tv->DrawPartialDecal(index2xy(i) * tileSize, {16,16}, SpriteSheetDecal, {0,0}, {16,16});
        }
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
    int xy2index(olc::vi2d index)
    {
        return index.x + m_Width * index.y;
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