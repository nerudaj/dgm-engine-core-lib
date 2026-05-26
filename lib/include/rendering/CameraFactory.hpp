#pragma once

#include <DGM/classes/Camera.hpp>

class [[nodiscard]] CameraFactory final
{
public:
    dgm::Camera RenderingEngine::createFullscreenCamera(
        const sf::Vector2f& currentResolution,
        const sf::Vector2f& desiredResolution);
};
