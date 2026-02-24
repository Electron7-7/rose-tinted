#include "imgui_main_menu.hpp"
#include "thirdparty/DearImGui/imgui.h"
#include "thirdparty/DearImGui/imgui_stdlib.h"
#include <Nostalgia/events/event.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/managers/manager.hpp>
#include <Nostalgia/managers/theatre_manager.hpp>

static ImGuiMainMenu sMainMenu{};
ImGuiMainMenu* g_pMainMenu{&sMainMenu};

void ImGuiMainMenu::Input(InputEvent* inEvent)
{
    if(inEvent->IsInputAction() and inEvent->IsActive("toggle_main_menu"))
        { mMainMenuOpen = !mMainMenuOpen; }
}

void ImGuiMainMenu::Update()
{
    if(!mMainMenuOpen)
        { return; }
    static auto main_window_flags{ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse};
    FAUTO main_window_size{MainWindow()->GetScale()};
    ImVec2 main_menu_size{(float)main_window_size.w(), (float)main_window_size.h()};
    ImGui::SetNextWindowSize(main_menu_size, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0,0});
    if(!ImGui::Begin("MainMenu", nullptr, main_window_flags))
        { ImGui::End(); return; }
    ImGui::InputText("Theatre File Path", &mTheatrePath);
    ImGui::SameLine();
    if(ImGui::Button("Load Theatre"))
    {
        if(g_pTheatreManager->LoadNewTheatre(mTheatrePath))
        {
            mLastTheatrePath = mTheatrePath;
            mMainMenuOpen = false;
        }
    }
    ImGui::BeginDisabled(Manager::GetTheatreState() != ManagerEnums::IN_LEVEL);
    if(ImGui::Button("Exit Theatre"))
    {
        g_pTheatreManager->ShutdownTheatre();
        mMainMenuOpen = true;
    }
    ImGui::EndDisabled();
    ImGui::End();
}
