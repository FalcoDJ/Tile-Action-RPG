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
#include "olcPGEX_LayerController.h"

#include <vector>
#include <assert.h>

#include "Shapes/ShapeHandler.hpp"
#include "Shapes/HitBox/HitBoxHandler.hpp"
#include "Tiles/TileMap.hpp"
#include "Tiles/Walker.hpp"
#include "Items/HealthPickup.hpp"
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

    HealthPickup hhhhh;

    std::vector<Enemy*> enemies;
    std::vector<Player*> players;

    olc::vf2d tv_scale = {0,0};
    int walker_step_count = 100;
    

private:
    void CreateLevel()
    {
        walker.walk(walker_step_count);
        ShapeHandler::ClearCircles();
        ShapeHandler::ClearRectangles();
        HitBoxHandler::Clear();

        tile_map.Setup(level_size.x, level_size.y);

        for (auto const &i : walker.step_history)
        {
            tile_map.setTileXY(i.x,i.y, disabled);
        }

        players.clear();

        players.push_back(new Player);

        for (auto* player : players)
        {
            player->Spawn(walker.rooms.front().position);
            walker.rooms.pop_front();
            
            assert(player->Setup("assets/Entity/Player/SpriteSheet.png"));
        }

        hhhhh.Spawn(walker.get_end_room().position);

        enemies.clear();

        for (auto& room : walker.rooms)
        {
            if (room.size.mag2() > 2 * 2)
            {
                enemies.push_back(new Enemy);
                enemies.back()->Spawn(room.position + vf2d{ rand() % 1 - 0.5f, rand() % 1 - 0.5f});
            }
        }
        
        tv.SetWorldScale(tv_scale);
        tv.SetRangeX(true, 0, level_size.x);
        tv.SetRangeY(true, 0, level_size.y);

        tile_map.CreateBuffer(this, &tv);

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

    Decal* tempDecal = nullptr;

    int TilesLayer = 0;
    int EntityLayer = 0;
    int UILayer = 0;

    std::string ui_layer = "1";
    std::string entity_layer = "2";
    std::string tiles_layer = "3";

    bool OnUserCreate() override
    {   
        LayerController::CreateLayer(ui_layer);
        LayerController::CreateLayer(entity_layer);
        LayerController::CreateLayer(tiles_layer);

        tv = TileTransformedView({ScreenWidth(), ScreenHeight()}, {16,16});
        tv_scale = tv.GetWorldScale();

        tempDecal = new Decal(new Sprite("assets/Tiles/Tiles.png"));

        tile_map.path_to_spritesheet = "assets/Tiles/Tiles.png";
        tile_map.defaultStateForTile = enabled;
        tile_map.tileSize = 1;

        level_size = {level_width, level_height};

        if (!tile_map.Setup(level_width, level_height))
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

    bool pan = false;
    olc::vf2d CameraPos;

    bool OnUserUpdate(float fElapsedTime) override
    {

        // Handle Pan & Zoom
		// if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		// if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		// if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		// if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(1.1f, GetMousePos());
		// if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.9f, GetMousePos());
        // if (GetKey(Key::P).bPressed) pan = !pan;
        tv.SetWorldScale({20.0f, 20.0f});

        int number_of_dead_enemies = 0;

        for (auto* enemy : enemies)
        {

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

            enemy->SetPlayerPos(PlayerToBeChased->GetBounds().pos);
            PlayerToBeChased = nullptr;

            enemy->Update(fElapsedTime);

            if (!enemy->AmIAlive()) number_of_dead_enemies++;
        }

        for (auto* player : players)
        {
            player->handleInput(this);
            player->Update(fElapsedTime);
        }

        float cam_speed = 0.0125f;
        CameraPos = CameraPos.lerp(players[0]->GetBounds().pos, 1 - pow(cam_speed,fElapsedTime));

        // These will need to updated when/if multiplayer is implemented
        if (!pan)
            tv.SetWorldOffset(CameraPos - tv.ScaleToWorld(olc::vf2d(ScreenWidth()/2.0f, ScreenHeight()/2.0f)));
        
		olc::vi2d vCurrentCell = players[0]->GetBounds().pos.floor()/24;
        // 


        if (GetKey(olc::ENTER).bPressed)
        {
            walker_step_count += 25;
            level_size += { 5, 5 };
            if (walker.Setup(level_size/2,{0,0},level_size))
            {
                CreateLevel();
            }
        }
        
        HitBoxHandler::Update(fElapsedTime);
        ShapeHandler::Update(fElapsedTime);

        if (number_of_dead_enemies >= enemies.size()) 
        { 
            if (walker.Setup(level_size/2,{0,0},level_size))
            {
                CreateLevel();
            } 
        }

        // Drawing Stuff Starts Here

        LayerController::SetActiveLayer(tiles_layer);
        Clear(BLANK);
        tile_map.DrawByBuffer(&tv);
        
        LayerController::SetActiveLayer(entity_layer);
        Clear(BLANK);
        for (auto* enemy : enemies)
            if (enemy->AmIAlive())
                enemy->Draw(&tv);

        for (auto* player : players)
            if (player->AmIAlive())
                player->Draw(&tv);


        LayerController::SetActiveLayer(ui_layer);
        Clear(BLANK);

        HitBoxHandler::Draw(&tv);

        FillRectDecal({7,3}, {82,10}, BLACK);
        FillRectDecal({8,4}, {players[0]->GetHealthPercentage() * 80, 8}, GREEN);
        DrawStringDecal({96,4}, "(" + std::to_string(vCurrentCell.x) + ", " + std::to_string(vCurrentCell.y) + ") FPS: " + std::to_string(GetFPS()), RED);

        return true;
    }
};

#endif