#include "./gui_globals.hpp"
#include "./imgui_main_menu.hpp"
#include "things/player.hpp"
#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/managers/theatre_manager.hpp>
#include <Nostalgia/theatre/theatre.hpp>

void GUI::SetInputFocus(bool inHasFocus)
{
    if(inHasFocus)
        { MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_VISIBLE); }
    if(Manager::GetTheatreState() == ManagerEnums::IN_LEVEL)
    {
        auto player{g_pTheatreManager->CurrentTheatre()->GetThinker<RoseTintedPlayer3D>(UID::o_Player)};
        if(not inHasFocus)
        {
            player->mCaptureKeyboard = player->mCaptureMouse = true;
            MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_DISABLED);
        }
        else
            { player->mCaptureKeyboard = player->mCaptureMouse = false; }
    }
}

void GUI::OpenAll()
{
    SetInputFocus(true);
    ImGuiMainMenu::SetOpen(true);
}

void GUI::CloseAll()
{
    SetInputFocus(false);
    ImGuiMainMenu::SetOpen(false);
}
