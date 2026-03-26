#include "./gui_globals.hpp"
#include "./imgui_main_menu.hpp"
#include "things/player.hpp"
#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/theatre/theatre.hpp>
#include <Nostalgia/managers/manager.hpp>

void GUI::SetInputFocus(bool inHasFocus)
{
    if(inHasFocus)
        { MainWindow()->SetMouseMode(IWindow::MOUSE_MODE_VISIBLE); }
    if(Manager::GetTheatreState() == ManagerEnums::IN_LEVEL)
    {
        auto player{Theatre::Current()->GetThinker<RoseTintedPlayer3D>(UID::o_Player)};
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
