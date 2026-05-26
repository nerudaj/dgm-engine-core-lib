#pragma once

#include <DGM/classes/Camera.hpp>

class [[nodiscard]] CameraFactory final
{
public:
    /**
    * \brief Create fullscreen camera with a fixed resolution and aspect ratio
    *
    * When porting games to multiple systems / resolutions / aspect ratio, the
    * renderer can show less / more than you need to. This function allows you
    * to run the game at some fixed internal resolution, inserting black bars
    * if the aspect ratio of the current device mismatches your desired aspect
    * ratio.
    */
    static dgm::Camera createFullscreenCamera(
        const sf::Vector2f& currentResolution,
        const sf::Vector2f& desiredResolution);
};
