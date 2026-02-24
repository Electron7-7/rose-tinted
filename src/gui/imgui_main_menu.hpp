#ifndef IMGUI_MAIN_MENU_H
#define IMGUI_MAIN_MENU_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/ui/solution.hpp>

class ImGuiMainMenu : public UI_Solution
{
public:
    void Update() final;
    void Input(InputEvent*) final;

private:
    bool mMainMenuOpen{true};
    std::string mTheatrePath{"Theatres/TestingTheatre.nt"};
    std::string mLastTheatrePath{mTheatrePath};
};

extern ImGuiMainMenu* g_pMainMenu;

#endif // IMGUI_MAIN_MENU_H
