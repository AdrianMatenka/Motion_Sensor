
#include "InputController.h"

InputController::~InputController()
{
    releaseAll();
}

bool InputController::pressKey(WORD key)
{
    // Avoid sending repeated key-down events
    if (pressedKeys.contains(key))
        return true;

    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    if (SendInput(1, &input, sizeof(INPUT)) != 1)
        return false;

    pressedKeys.insert(key);

    return true;
}

bool InputController::releaseKey(WORD key)
{
    // Release only keys tracked by this controller
    if (!pressedKeys.contains(key))
        return true;

    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    if (SendInput(1, &input, sizeof(INPUT)) != 1)
        return false;

    pressedKeys.erase(key);

    return true;
}

bool InputController::tapKey(WORD key)
{
    if (!pressKey(key))
        return false;

    return releaseKey(key);
}

void InputController::releaseAll()
{
    for (WORD key : pressedKeys)
    {
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(1, &input, sizeof(INPUT));
    }

    pressedKeys.clear();
}
