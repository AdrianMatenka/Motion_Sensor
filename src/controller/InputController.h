
#pragma once

#include <Windows.h>
#include <unordered_set>

class InputController
{
public:
    InputController() = default;
    ~InputController();

    bool pressKey(WORD key);
    bool releaseKey(WORD key);
    bool tapKey(WORD key);

    void releaseAll();

private:
    std::unordered_set<WORD> pressedKeys;
};
