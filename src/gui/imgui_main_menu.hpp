#ifndef IMGUI_MAIN_MENU_H
#define IMGUI_MAIN_MENU_H

#include <Nostalgia/Nostalgia.hpp>
#include <Nostalgia/ui/solution.hpp>

class ImGuiMainMenu : public UI_Solution
{
public:
    void Update() final;
    void Input(InputEvent*) final;

    static void SetOpen(bool inIsOpen);

private:
    static bool m_sOpen;
    std::string mTheatrePath{"Theatres/TestingTheatre.nt"};
    std::string mLastTheatrePath{mTheatrePath};
};

#endif // IMGUI_MAIN_MENU_H
