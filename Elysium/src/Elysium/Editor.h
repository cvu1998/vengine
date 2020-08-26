#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>

#include "Math.h"
#include "OrthographicCameraController.h"
#include "Scene.h"
#include "Timestep.h"

#include "Renderer/Renderer2D.h"

namespace Elysium 
{
    enum class QuadData
    {
        UNKNOWN = -1,
        TAG = 0,
        POSITION = 1,
        SIZE = 2,
        ROTATION = 3,
        COLOR = 4
    };

    struct Quad
    {
        size_t Index = 0;
        std::string Tag = "Quad";
        float Position[2] = { 0.0f, 0.0f };
        float Size[2] = { 2.0f, 2.0f };
        float Rotation = 0.0f;
        float Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        void saveQuad(std::ofstream& sceneFile)
        {
            sceneFile << "Tag=" << Tag << '\n'
                << "Position=" << Position[0] << ", " << Position[1] << '\n'
                << "Size=" << Size[0] << ", " << Size[1] << '\n'
                << "Rotation=" << Rotation << '\n'
                << "Color=" << Color[0] << ", " << Color[1] << ", " << Color[2] << ", " << Color[3] << '\n';
        }
    };

    struct SceneData
    {
        std::vector<Quad> Quads;
    };

    /*** NEEDS IMGUI TO RUN ***/
    class Editor : public Scene
    {
    private:

        unsigned int m_WindowWidth;
        unsigned int m_WindowHeight;

        OrthographicCameraController m_CameraController;

        Quad* m_Current = nullptr;
        unsigned int m_Repeat = 0;
        bool m_CurrentIsMoving = false;

        SceneData m_Data;

        char m_Filename[64] = "Scene";

    private:
        bool isWithinBounds(Vector2 position, Vector2 bottom, Vector2 top);

        Vector2 getCursorPosition();

        void saveSceneToFile();

    public:
        Editor(unsigned int width, unsigned int height);

        void onUpdate(Timestep ts) override;
        void onEvent(Elysium::Event& event) override;

        bool onMousePressedEvent(Elysium::MouseButtonPressedEvent& event);
        bool onWindowResizeEvent(Elysium::WindowResizeEvent& event);

        static void getSceneFromFile(const char* filename, std::vector<Quad>& quads);
    };
}