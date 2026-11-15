
#include "GameController.h"

#include <utility>

GameController::GameController(InputController& inputController)
    : inputController(inputController)
{
}

void GameController::addMapping(const ActionMapping& mapping)
{
    mappings.push_back(mapping);
    activeMappings.push_back(false);
}

void GameController::update(const GestureDetector& gestureDetector)
{
    for (std::size_t i = 0; i < mappings.size(); i++)
    {
        const auto& mapping = mappings[i];

        const GestureState state =
            gestureDetector.getGestureState(mapping.gesture);

        switch (mapping.mode)
        {
            case ActionMode::ON_ACTIVATION:
                if (state == GestureState::JUST_ACTIVATED)
                    inputController.tapKey(mapping.key);
                break;

            case ActionMode::WHILE_ACTIVE:
                if (state == GestureState::JUST_ACTIVATED ||
                    state == GestureState::ACTIVE)
                {
                    if (!activeMappings[i])
                    {
                        inputController.pressKey(mapping.key);
                        activeMappings[i] = true;
                    }
                }
                else if (state == GestureState::JUST_DEACTIVATED)
                {
                    inputController.releaseKey(mapping.key);
                    activeMappings[i] = false;
                }
                break;

            case ActionMode::ON_DEACTIVATION:
                if (state == GestureState::JUST_DEACTIVATED)
                    inputController.tapKey(mapping.key);
                break;
        }
    }
}

void GameController::reset()
{
    inputController.releaseAll();

    for (std::size_t i = 0; i < activeMappings.size(); i++)
    {
        activeMappings[i] = false;
    }
}
