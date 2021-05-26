#ifndef WALKER_HPP
#define WALKER_HPP

#include <list>
#include <random>
#include "TileMap.hpp"

uint_fast32_t seed = 1234567891;

template < typename T > void shuffle( std::list<T>& lst ) // shuffle contents of a list
{
    // create a vector of (wrapped) references to elements in the list
    // http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
    std::vector< std::reference_wrapper< const T > > vec( lst.begin(), lst.end() ) ;

    // shuffle (the references in) the vector
    std::shuffle( vec.begin(), vec.end(), std::mt19937{ std::random_device{}() } ) ;

    // copy the shuffled sequence into a new list
    std::list<T> shuffled_list {  vec.begin(), vec.end() } ;

    // swap the old list with the shuffled list
    lst.swap(shuffled_list) ;
}

olc::vf2d LEFT =  olc::vf2d(-1,0);
olc::vf2d UP =  olc::vf2d(0,-1);
olc::vf2d RIGHT =  olc::vf2d(1,0);
olc::vf2d DOWN =  olc::vf2d(0,1);
std::list<olc::vf2d> DIRECTIONS = {olc::vf2d(-1,0), olc::vf2d(0,-1), olc::vf2d(1,0), olc::vf2d(0,1)};

class room
{
    public:
    room(){}
    room(olc::vf2d p, olc::vf2d s) : position(p), size(s) {}
    olc::vf2d position;
    olc::vf2d size;
};

class Walker
{
private:
    olc::vf2d m_Position;
    olc::vf2d m_Direction = {1,0};
    olc::vf2d m_Top_Left_Border;
    olc::vf2d m_Bottom_Right_Border;
    int steps_since_turn = 0;

public:
    std::list<room> rooms;
    std::list<olc::vf2d> step_history;
    int max_hall_length = 6;

private:
    void change_direction()
    {
        place_room(m_Position);

        steps_since_turn = 0;
        std::list<olc::vf2d> directions = DIRECTIONS;
        
        directions.remove(m_Direction);

        shuffle(directions);

        m_Direction = directions.front();
        directions.pop_front();

        while (m_Position + m_Direction < m_Top_Left_Border && m_Position + m_Direction > m_Bottom_Right_Border)
        {
            m_Direction = directions.front();
            directions.pop_front();
        }

    }
    bool step()
    {
        olc::vf2d target_pos = m_Position + m_Direction;
        if (target_pos > m_Top_Left_Border && target_pos < m_Bottom_Right_Border)
        {
            steps_since_turn++;
            m_Position = target_pos;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    room create_room(olc::vf2d position, olc::vf2d size)
    {
        return room(position,size);
    }
    
    void place_room(olc::vf2d position)
    {
        olc::vf2d size = {float(rand() % 2 + (rand() % 2 + 4)), float(rand() % 2 + 4)};
        olc::vf2d top_left_corner = (m_Position - size/2).ceil();
        // step_history.push_back(top_left_corner);
        rooms.push_back(create_room(position, size));
        for (int y = 0; y < size.y; y++)
            for (int x = 0; x < size.x; x++)
            {
                olc::vf2d new_step = top_left_corner + olc::vf2d(x,y);
                if (new_step > top_left_corner && new_step < m_Bottom_Right_Border)
                    step_history.push_back(new_step);
            }
    }


    room get_end_room()
    {
        room end_room = rooms.front();
        rooms.pop_front();
        olc::vf2d starting_position = step_history.front();
        for (auto r : rooms)
            if ((r.position - starting_position).mag() > (end_room.position - starting_position).mag())
                end_room = r;
        return end_room;
    }

public:
    Walker(){}
    ~Walker(){}

    bool Setup(olc::vf2d start_pos, olc::vf2d top_left, olc::vf2d bottom_right)
    {
        if (start_pos < top_left || start_pos > bottom_right) // if start_pos isn't in borders
        return false;

        step_history.clear();
        rooms.clear();

        m_Position = start_pos;
        step_history.push_back(start_pos);

        m_Top_Left_Border = top_left;
        m_Bottom_Right_Border = bottom_right;

        return true;
    }

    void walk(int steps)
    {
        srand(time(0) + seed);

        place_room(m_Position);

        for (int i = 0; i < steps; i++)
        {
            if (rand() > 0.5 && steps_since_turn >= max_hall_length)
                change_direction();

            if (step())
            step_history.push_back(m_Position);
            else
            change_direction();
        }
    }
};

#endif