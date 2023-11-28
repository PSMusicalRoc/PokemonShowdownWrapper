#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

int main(int argc, char** argv)
{
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL could not initialize!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
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
