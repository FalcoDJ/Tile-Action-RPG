#ifndef ENGINE_HPP
#define ENGINE_HPP

#ifndef OLC_PGE_APPLICATION
#define OLC_PGE_APPLICATION
#endif

#ifndef OLC_PGEX_TRANSFORMEDVIEW
#define OLC_PGEX_TRANSFORMEDVIEW
#endif

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#include <vector>

#include "Shapes/ShapeHandler.hpp"
#include "Shapes/HitBox/HitBoxHandler.hpp"
#include "Tiles/TileMap.hpp"
#include "Tiles/Walker.hpp"
#include "Entity/Player/Player.hpp"
#include "Entity/Enemies/Enemy.hpp"

using namespace olc;

class Engine : public olc::PixelGameEngine
{
public:
    ShapeHandler sh;
    HitBoxHandler hbh;
    TileTransformedView tv;

    vf2d Canvas_Size = {480, 270};
    vf2d Canvas_Test_Width = {1280, 720};
    vf2d Canvas_Scale = {1, 1};

    TileMap tile_map;
    Walker walker;
    int level_width = 50;
    int level_height = 50;
    olc::vi2d level_size;

    std::vector<Enemy*> enemies;
    std::vector<Player*> players;

    olc::vf2d tv_scale = {0,0};
    

private:
    void CreateLevel()
    {
        walker.walk(100);
        ShapeHandler::ClearCircles();
        ShapeHandler::ClearRectangles();
        HitBoxHandler::Clear();

        for (auto const &i : walker.step_history)
        {
            tile_map.setTileXY(i.x,i.y, disabled);
        }


        enemies.clear();

        for (auto& room : walker.rooms)
        {
            if (room.size > vi2d(2,2))
            {
                enemies.push_back(new Enemy);
                enemies.back()->Spawn(room.position + room.size/2);
            }
        }

        players.clear();

        players.push_back(new Player);

        for (auto* player : players)
            player->Spawn(walker.step_history.front());
        
        tv.SetWorldScale(tv_scale);

        tile_map.CreateBuffer(this, &tv);
        // object.pos = walker.step_history.front() * 24;

        ShapeHandler::SetWorldBounds({0,0}, level_size);

        for (int i = 0; i < level_width * level_height; i++)
        {
            if (tile_map.getTile(i).state == enabled)
            {
                olc::vf2d tilePos = tile_map.index2xy(i);
                Rectangle* r = ShapeHandler::CreateRectangle();
                r->pos = tilePos;
                r->size = {tile_map.tileSize,tile_map.tileSize};
            }
        }
    }
    
public:
    Engine()
    {
        // if (Canvas_Test_Width > Canvas_Size)
        Canvas_Scale = Canvas_Test_Width/Canvas_Size;
        Canvas_Scale.y = Canvas_Scale.x;

        std::cout << "(" << Canvas_Scale.x << ", " << Canvas_Scale.y << ")";
        
        sAppName = "Action RPG";

        HitBoxHandler::GetNullLayer();
    }
    ~Engine()
    {
        players.clear();
        enemies.clear();
    }

    bool OnUserCreate() override
    {
        tv = TileTransformedView({ScreenWidth(), ScreenHeight()}, {16,16});
        tv_scale = tv.GetWorldScale();

        tile_map.path_to_spritesheet = "assets/Tiles/MysticChroma_Basics.png";
        tile_map.defaultStateForTile = enabled;
        tile_map.tileSize = 1;

        level_size = {level_width, level_height};

        if (!tile_map.Setup(level_width, level_height, this))
        {
            return false;
        }

        if (walker.Setup(level_size/2,{0,0},level_size))
        {
            CreateLevel();
        }
        else
        {
            return false;
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        // // Handle Pan & Zoom
		// if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		// if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		// if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		// if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
		// if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());

        for (auto* enemy : enemies)
        {
            enemy->Update(fElapsedTime);

            Player* PlayerToBeChased = players[0];
            
            float distanceFromOldPlayer = (players[0]->GetBounds().pos - enemy->GetBounds().pos).mag2();

            for (auto* player : players)
            {
                if ((player->GetBounds().pos - enemy->GetBounds().pos).mag2() < distanceFromOldPlayer * distanceFromOldPlayer)
                {
                    PlayerToBeChased = nullptr;
                    PlayerToBeChased = player;
                    distanceFromOldPlayer = (player->GetBounds().pos - enemy->GetBounds().pos).mag2();
                }
            }

            enemy->chase_player(PlayerToBeChased->GetBounds().pos);
            PlayerToBeChased = nullptr;
        }

        for (auto* player : players)
        {
            player->handleInput(this);
            player->Update(fElapsedTime);
        }

        // These will need to updated when multiplayer is implemented
        tv.SetWorldOffset(players[0]->GetBounds().pos - tv.ScaleToWorld(olc::vf2d(ScreenWidth()/2.0f, ScreenHeight()/2.0f)));
		olc::vi2d vCurrentCell = players[0]->GetBounds().pos.floor()/24;
        // 


        if (GetKey(olc::ENTER).bPressed)
        {
            for (int i = 0; i < level_width * level_height; i++)
            {
                tile_map.setTileIndex(i, enabled);
            }

            if (walker.Setup(level_size/2,{0,0},level_size))
            {
                CreateLevel();
            }
        }
        
        HitBoxHandler::Update(fElapsedTime);
        ShapeHandler::Update(fElapsedTime);

        Clear(BLANK);
        tile_map.DrawByBuffer(&tv, {0,0});
        
        for (auto* enemy : enemies)
        enemy->Draw(&tv, {0,0});

        for (auto* player : players)
        player->Draw(&tv, {0,0});

        HitBoxHandler::Draw(&tv);

        DrawStringDecal({2,2}, "(" + std::to_string(vCurrentCell.x) + ", " + std::to_string(vCurrentCell.y) + ") FPS: " + std::to_string(GetFPS()), RED);

        return true;
    }
};

#endif