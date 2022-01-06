#pragma once
#include <vector>
#include <string>
#include "ui.h"


class ToolBar
{
private:

    std::vector<UI> button_list;

public:
    ToolBar();
    ~ToolBar();
    void init();
    void Draw();
};

