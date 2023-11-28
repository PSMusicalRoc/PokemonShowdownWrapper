#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "PShowdownParser.hpp"
#include "QuickReading.hpp"
#include <unistd.h>

int main(int argc, char** argv)
{
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL could not initialize!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cerr << "SDL_IMG could not be initialized!" << std::endl;
        std::cerr << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    
    window = SDL_CreateWindow(
        "Pokemon Showdown Player Port",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1920,
        1080,
        0
    );

    if (window == NULL)
    {
        std::cerr << "Window could not be created!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui_ImplSDL2_InitForSDLRenderer(window, ren);
    ImGui_ImplSDLRenderer2_Init(ren);

    SDL_Event e;
    bool windowShouldRun = true;
    PShowdownParser parser(ren);

    while (windowShouldRun)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                windowShouldRun = false;
            }

            ImGui_ImplSDL2_ProcessEvent(&e);
        }
        if (!windowShouldRun)
        {
            continue;
        }

        std::string serverInput;
        QuickRead(serverInput, STDIN_FILENO);
        if (serverInput != "")
        {
            std::string send_back = parser.parsePShowdownOutput(serverInput);
            write(STDOUT_FILENO, (void*)(send_back.c_str()),
                sizeof(char) * send_back.size());
        }

        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui::NewFrame();
        
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        ImGui::SetNextWindowPos(ImVec2(0, 1080-270));
        ImGui::SetNextWindowSize(ImVec2(1920, 270));

        ImGui::Begin("MoveInput", (bool*)NULL, flags);

        if (ImGui::BeginTable("MainBattleMenuTable", 4))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::Button("FIGHT", ImVec2(460, 250)))
            {
                std::cout << "Fight" << std::endl;
            }
            ImGui::TableNextColumn();

            if (ImGui::Button("ITEMS", ImVec2(460, 250)))
            {
                std::cout << "Items" << std::endl;
            }
            ImGui::TableNextColumn();

            if (ImGui::Button("POKEMON", ImVec2(460, 250)))
            {
                std::cout << "Pokemon" << std::endl;
            }
            ImGui::TableNextColumn();

            if (ImGui::Button("RUN", ImVec2(460, 250)))
            {
                std::cout << "Run" << std::endl;
            }
            
            ImGui::EndTable();
        }

        ImGui::End();

        SDL_SetRenderDrawColor(ren, 100, 100, 0, 255);
        SDL_Rect fullscreen = {0, 0, 1920, 1080};
        SDL_RenderFillRect(ren, &fullscreen);

        if (parser.front_textures.find("substitute") != parser.front_textures.end())
        {
            SDL_Rect dst_rect = { 0, 0, 96, 96 };
            SDL_RenderCopy(ren, parser.front_textures.at("substitute"), NULL, &dst_rect);
        }

        if (parser.back_textures.find("substitute") != parser.back_textures.end())
        {
            SDL_Rect dst_rect = { 50, 50, 96, 96 };
            SDL_RenderCopy(ren, parser.back_textures.at("substitute"), NULL, &dst_rect);
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(ren);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
