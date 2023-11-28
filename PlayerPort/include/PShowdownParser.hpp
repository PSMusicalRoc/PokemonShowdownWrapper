#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>

using json=nlohmann::json;

class PShowdownParser
{
public:
    /**
     * @todo remove inpipe and outpipe, they serve zero purpose in this
     * particular program.
    */
    PShowdownParser(SDL_Renderer* renderer, int player_perspective = 1)
        :ren(renderer), m_perspective(player_perspective) {}

    std::string parsePShowdownOutput(const std::string& input);

    const std::string GetPlayer1Name() const { return m_p1data["name"]; }
    const std::string GetPlayer2Name() const { return m_p2data["name"]; }

    std::string QueryMoves();

    bool OppSelectMove() const { return m_opp_needs_select_move; }
    void OppSelectedMove() { m_opp_needs_select_move = false; }

    std::map<std::string, SDL_Texture*> back_textures;
    std::map<std::string, SDL_Texture*> front_textures;

private:
    bool m_playerdatainitialized = false;
    bool m_battledatainitialized = false;

    int m_perspective;
    int m_curr_editing = 0;

    json m_battledata;

    json m_p1data;
    json m_p2data;
    json m_p3data;
    json m_p4data;

    SDL_Renderer* ren;

    bool m_opp_needs_select_move = false;
};