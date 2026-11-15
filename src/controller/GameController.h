
#pragma once

#include "InputController.h"
#include "Gesture.h"
#include "GestureDetector.h"

#include <vector>
#include <string>

enum class ActionMode
{
    ON_ACTIVATION,
    WHILE_ACTIVE,
    ON_DEACTIVATION
};

struct ActionMapping
{
    GestureID gesture;
    WORD key;
    ActionMode mode;
};

class GameController
{
public:
    explicit GameController(InputController& inputController);

    bool loadMappings(const std::string& configPath);

    void update(const GestureDetector& gestureDetector);

    void reset();

private:
    InputController& inputController;
    std::vector<ActionMapping> mappings;
    std::vector<bool> activeMappings;
};
