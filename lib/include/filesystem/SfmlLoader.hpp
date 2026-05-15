#pragma once

#include <DGM/classes/Error.hpp>
#include <DGM/classes/Utility.hpp>
#include <SFML/Audio/Music.hpp>
#include <expected>
#include <filesystem>

class [[nodiscard]] SfmlLoader final
{
public:
    static constexpr const auto loadFont = dgm::Utility::loadFont;

    static inline std::expected<sf::Shader, dgm::Error>
    loadShader(const std::filesystem::path& path)
    {
        try
        {
            auto&& path1 = path.string() + ".vert";
            auto&& path2 = path.string() + ".frag";

            auto shader = sf::Shader(std::filesystem::path(path1), path2);
            return shader;
        }
        catch (const std::exception& ex)
        {
            return std::unexpected(dgm::Error(ex.what()));
        }
    }

    static std::expected<sf::Music, dgm::Error>
    loadSong(const std::filesystem::path& path)
    {
        try
        {
            auto music = sf::Music(path);
            return music;
        }
        catch (const std::exception& ex)
        {
            return std::unexpected { dgm::Error(ex.what()) };
        }
    }

    static constexpr const auto loadSound = dgm::Utility::loadSound;

    static constexpr const auto loadTexture = dgm::Utility::loadTexture;
};
