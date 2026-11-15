
#include "GameController.h"

#include <utility>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace
{
    GestureID parseGesture(const std::string& name)
    {
        if (name == "LEFT_HAND_RAISED")
            return GestureID::LEFT_HAND_RAISED;

        if (name == "RIGHT_HAND_RAISED")
            return GestureID::RIGHT_HAND_RAISED;

        throw std::runtime_error("Unknown gesture: " + name);
    }

    ActionMode parseActionMode(const std::string& name)
    {
        if (name == "ON_ACTIVATION")
            return ActionMode::ON_ACTIVATION;

        if (name == "WHILE_ACTIVE")
            return ActionMode::WHILE_ACTIVE;

        if (name == "ON_DEACTIVATION")
            return ActionMode::ON_DEACTIVATION;

        throw std::runtime_error("Unknown action mode: " + name);
    }

    WORD parseKey(const std::string& name)
{
    if (name == "SPACE")
        return VK_SPACE;

    if (name == "SHIFT")
        return VK_SHIFT;

    if (name == "CTRL")
        return VK_CONTROL;

    if (name == "ALT")
        return VK_MENU;

    if (name == "ENTER")
        return VK_RETURN;

    if (name == "ESC")
        return VK_ESCAPE;

    throw std::runtime_error("Unknown key: " + name);
}
}

bool GameController::loadMappings(const std::string& configPath)
{
    try
    {
        std::ifstream file(configPath);

        if (!file.is_open())
            throw std::runtime_error("Cannot open config file: " + configPath);

        nlohmann::json config;
        file >> config;

        // Get active profile
        const std::string activeProfile =
            config.at("application").at("active_profile");

        const auto& profile =
            config.at("profiles").at(activeProfile);

        const auto& jsonMappings = profile.at("mappings");

        std::vector<ActionMapping> newMappings;

        for (const auto& item : jsonMappings)
        {
            ActionMapping mapping;

            mapping.gesture = parseGesture(
                item.at("gesture").get<std::string>()
            );

            mapping.key = parseKey(
                item.at("key").get<std::string>()
            );

            mapping.mode = parseActionMode(
                item.at("mode").get<std::string>()
            );

            newMappings.push_back(mapping);
        }

        // Replace current mappings only after successful parsing
        mappings = std::move(newMappings);
        activeMappings.assign(mappings.size(), false);

        std::cout << "Loaded " << mappings.size()
                  << " gesture mappings from profile: "
                  << activeProfile << std::endl;

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Configuration loading failed: "
                  << e.what() << std::endl;

        return false;
    }
}

GameController::GameController(InputController& inputController)
    : inputController(inputController)
{
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
