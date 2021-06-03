#include "HitBoxHandler.hpp"

HitBoxHandler* HitBoxHandler::m_s_Instance = nullptr;

HitBoxHandler::HitBoxHandler()
{
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;

    m_HurtBoxes.reserve(9999999);
}
HitBoxHandler::~HitBoxHandler()
{
    m_HitBoxes.clear();
    m_HurtBoxes.clear();
}

int HitBoxHandler::GetLayer(std::string layer_name)
{
    auto& lm = m_s_Instance->m_Layers;
    
    auto keyValuePair = lm.find(layer_name);

    if (keyValuePair != lm.end())
    {
        // layer exists
        return keyValuePair->second;
    }
    else
    {
        // create layer
        auto& layer = lm[layer_name];
        
        layer = lm.size() - 1;

        return layer;
    }
}

int HitBoxHandler::GetNullLayer()
{
    return m_s_Instance->GetLayer(m_s_Instance->m_NullLayer);
}

void HitBoxHandler::CreateHitBox(olc::vf2d p, float r, float m, HBType t, int layer)
{
    auto& hv = m_s_Instance->m_HitBoxes;

    HitBox h;
    h.pos = p;
    h.radius = r;
    h.magnitude = m;
    h.type = t;
    h.layer = layer;

    hv.push_back(h);
}

HitBox* HitBoxHandler::CreateLongTermHitBox(olc::vf2d p, float r, float m, HBType t, int layer)
{
    auto& hv = m_s_Instance->m_LongTermHitboxes;

    HitBox h;
    h.pos = p;
    h.radius = r;
    h.magnitude = m;
    h.type = t;
    h.layer = layer;
    h.ID = hv.size();

    hv.push_back(h);

    return &hv.back();
}

void HitBoxHandler::DeleteLongTermHitBox(int& hitbox_id)
{
    auto it = m_s_Instance->m_LongTermHitboxes.begin();
    std::advance(it, hitbox_id);
    m_s_Instance->m_LongTermHitboxes.erase(it);
}

HurtBox* HitBoxHandler::CreateHurtBox(std::string layer_name)
{
    auto& hv = m_s_Instance->m_HurtBoxes;
    hv.push_back(HurtBox(m_s_Instance->GetLayer(layer_name)));
    return &hv[hv.size()-1];
}

void HitBoxHandler::Clear()
{
    m_s_Instance->m_HurtBoxes.clear();
    m_s_Instance->m_LongTermHitboxes.clear();
}

void HitBoxHandler::Update(float fElapsedTime)
{
    auto& hurtbox_vec = m_s_Instance->m_HurtBoxes;
    auto& lt_hitboxlist = m_s_Instance->m_LongTermHitboxes;
    auto& hitbox_vec = m_s_Instance->m_HitBoxes;

    for (auto& ltHitBox : lt_hitboxlist)
        hitbox_vec.push_back(ltHitBox);


    for (auto& hurtbox : hurtbox_vec)
    {
        for (auto& hitbox : hitbox_vec)
        {
            if (hurtbox.layer != hitbox.layer && hurtbox.layer != GetNullLayer())
            {
                // HurtBox and hitbox are not on the same layer so continue
                olc::vf2d vecDistance = (hurtbox.pos - hitbox.pos);
                float distance = vecDistance.mag2();
                float sumOfRadii = (hurtbox.radius + hitbox.radius);
                if (distance < sumOfRadii * sumOfRadii)
                {
                    // There is a collision
                    hurtbox.magnitude = hitbox.magnitude;
                    hurtbox.type = hitbox.type;
                }
            }
        }
    }
}

void HitBoxHandler::Draw(olc::TransformedView* tv)
{
    olc::Pixel HurtBoxColor = olc::Pixel(255,255,0,127);
    olc::Pixel HitBoxColor  = olc::Pixel(255,0,0,127);
    auto& hurtbox_vec = m_s_Instance->m_HurtBoxes;
    auto& hitbox_vec = m_s_Instance->m_HitBoxes;

    for (auto& hurtbox : hurtbox_vec)
        if (hurtbox.layer != GetNullLayer())
            tv->DrawCircle(hurtbox.pos, hurtbox.radius, HurtBoxColor);

    for (auto& hitbox : hitbox_vec)
        if (hitbox.layer != GetNullLayer())
            tv->DrawCircle(hitbox.pos, hitbox.radius, HitBoxColor);

    hitbox_vec.clear();
}
