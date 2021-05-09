#ifndef HIT_BOX_HANDLER
#define HIT_BOX_HANDLER

#include <vector>
#include <map>
#include "../../olcPGEX_TransformedView.h"
#include "HitBox.hpp"
#include "HurtBox.hpp"

class HitBoxHandler
{
    private:
        std::vector<HitBox> m_HitBoxes;
        std::vector<HurtBox> m_HurtBoxes;
        std::map<std::string, int> m_Layers;
        std::string m_NullLayer = "null_layer_0"; // Used for "invisible" hurtboxes

        static HitBoxHandler* m_s_Instance;

    public:
        HitBoxHandler();
        ~HitBoxHandler();

        static int GetLayer(std::string layer_name);
        static int GetNullLayer();

        static void CreateHitBox(olc::vf2d p, float r, float m, HBType t, int layer);
        static HurtBox* CreateHurtBox(std::string layer_name);

        static void Clear();

        static void Update(float fElapsedTime);
        static void Draw(olc::TransformedView* tv);
};

#endif