/* 
 * astar.cpp
 *
 * Manages the A* implementation
 *
 * Worked on by Jigar
 *
 */

#include "astar.hpp"
#include <math.h>

using namespace std;

void run_astar(PiGameMap map) {
    priority_queue<q_elem, vector<q_elem>, PriorityComp> main_q; 
    bool goal_reached = false;
    vector<vec2> positions = retrieve_ship_positions(map);
    vector<vec2> coords = retrieve_ship_coords(map);
    vec2 pirate_pos = positions[0]; // Hacky way to get PiPirate position
    vec2 pirate_coord = coords[0]; // Hacky way to get PiPirate coord
    pirate_pos.print_vec2();
    vec2 target_pos = get_target(positions);
    vec2 selected_pos = pirate_pos;
    main_q.push(q_elem(selected_pos, vec2(-1, -1), 0));
    while (!goal_reached) {
        q_elem selected_q = main_q.top();
        selected_pos = selected_q.pos;
        main_q.pop();
        vector<vec2> next_positions = retrieve_next_positions(selected_pos, 
                                        map.x_size, map.y_size);
        for (vec2 pos : next_positions) {
            // f(n) = g(n) + h(n)
            float g = distance(pirate_pos, selected_pos);
            float h = distance(selected_pos, target_pos);
            float priority = g + h;
            vec2 start = (selected_q.start.x == -1) ? pos : selected_q.start; 
            main_q.push(q_elem(pos, start, priority));
        }
        goal_reached = compare_vec(selected_pos, target_pos);
        if (goal_reached) {
            shift_pirate(map, pirate_coord, 
                    convert_coord_tile(map, selected_q.start)); // fix later
        }
    }
} 

void astar_trial() {
    PiGameMap map = PiGameMap::createRandomMap();
    int ticks = 0;
    for (ticks = 0; ticks < 5; ticks++) {
        map.print_game_map(); 
        priority_queue<q_elem, vector<q_elem>, PriorityComp> main_q; 
        bool goal_reached = false;
        vector<vec2> positions = retrieve_ship_positions(map);
        vec2 pirate_pos = positions[0]; // Hacky way to get PiPirate position
        pirate_pos.print_vec2();
        vec2 target_pos = get_target(positions);
        vec2 selected_pos = pirate_pos;
        main_q.push(q_elem(selected_pos, vec2(-1, -1), 0));
        while (!goal_reached) {
            q_elem selected_q = main_q.top();
            selected_pos = selected_q.pos;
            main_q.pop();
            vector<vec2> next_positions = retrieve_next_positions(selected_pos, 
                                            map.x_size, map.y_size);
            for (vec2 pos : next_positions) {
                // f(n) = g(n) + h(n)
                float g = distance(pirate_pos, selected_pos);
                float h = distance(selected_pos, target_pos);
                float priority = g + h;
                vec2 start = (selected_q.start.x == -1) ? pos : selected_q.start; 
                main_q.push(q_elem(pos, start, priority));
            }
            goal_reached = compare_vec(selected_pos, target_pos);
            if (goal_reached) {
                move_pirate(map, pirate_pos, selected_q.start);
                merchant_moves(map);
            }
        }
    }
    map.print_game_map(); 
}

void merchant_moves(PiGameMap& gm) {
    for (PiMerchant m : gm.merchants) {
        move_merchant(gm, m.position, vec2(m.position.x - 2, m.position.y));
    } 
}

vector<vec2> retrieve_ship_positions(PiGameMap gm) {
    vector<vec2> positions; 
    for(PiPirate p: gm.pirates) {
        positions.push_back(p.position);
    }
    for(PiMerchant m: gm.merchants) {
        positions.push_back(m.position);
    }
    return positions; 
}

vector<vec2> retrieve_ship_coords(PiGameMap gm) {
    vector<vec2> coords; 
    for(PiPirate p: gm.pirates) {
        coords.push_back(p.coord_pos);
    }
    for(PiMerchant m: gm.merchants) {
        coords.push_back(m.coord_pos);
    }
    return coords;
}

vector<vec2> retrieve_next_positions(vec2 curr, int max_x, int max_y) {
    vector<vec2> next; 
    int x; 
    int y; 
    for (x = -1; x < 2; x++) {
        for(y = -1; y < 2; y++) {
            int new_x = curr.x + x; 
            int new_y = curr.y + y; 
            if (new_x > 0 && new_y > 0 && 
                new_x < max_x && new_y < max_y && 
                (new_x != curr.x || new_y != curr.y)) {
                next.push_back(vec2(new_x, new_y));        
            }
        }
    }
    return next; 
}


vec2 get_target(vector<vec2> positions) {
    vec2 start_pos = positions[0]; // hacky way to get PiPirate
    vec2 min_pos = start_pos; 
    float min_dist = -1;
    for (vec2 pos : positions) {
        float curr_dist = distance(start_pos, pos);
        if (curr_dist < min_dist || min_dist < 1) {
            min_dist = curr_dist; 
            min_pos = pos;
        }
    }
    return min_pos;
}

// Basic distance, add better physics later
float distance(vec2 pos1, vec2 pos2) {
    float delta_x = pos1.x - pos2.x; 
    float delta_y = pos1.y - pos2.y;
    float dist = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
    return dist;
}

