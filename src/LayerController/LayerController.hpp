#ifndef LAYER_CONTROLLER
#define LAYER_CONTROLLER

#include "../olcPixelGameEngine.h"

class LayerController : public olc::PGEX
{
private:
    LayerController() : olc::PGEX(true) {}

    void OnBeforeUserCreate()
    {
        Get().pge->SetDrawTarget(nullptr);
        Get().pge->Clear(olc::BLANK);
    }

    static LayerController& Get()
    {
        static LayerController instance;
        return instance;
    }

    std::map<std::string,int> m_LayerIdMap;

public:
    static void CreateLayer(std::string layer_name)
    {
        auto& m = Get().m_LayerIdMap;
        m[layer_name] = Get().pge->CreateLayer();
        Get().pge->EnableLayer(m[layer_name], true);
        Get().pge->SetDrawTarget(m[layer_name]);
        Get().pge->SetPixelMode(olc::Pixel::ALPHA);
        Get().pge->Clear(olc::BLANK);
        Get().pge->SetDrawTarget(nullptr);
    }
    static void SetLayer(std::string layer_name)
    {
        auto& m = Get().m_LayerIdMap;

        auto keyvaluepair = m.find(layer_name);

        if (keyvaluepair != m.end())
        {
            Get().pge->SetDrawTarget(keyvaluepair->second);
        }
        else
        {
            m[layer_name] = Get().pge->CreateLayer();
            Get().pge->EnableLayer(m[layer_name], true);
            Get().pge->SetDrawTarget(m[layer_name]);
            Get().pge->SetPixelMode(olc::Pixel::ALPHA);
            Get().pge->Clear(olc::BLANK);
        }
        
    }
};

#endif