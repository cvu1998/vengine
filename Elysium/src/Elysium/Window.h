#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Elysium
{
    struct WindowProperties
    {
        std::string Title;
        unsigned int Width, Height;

        WindowProperties(const std::string& title = "Elysium Engine", unsigned int width = 1280, unsigned int height = 960) :
            Title(title), Width(width), Height(height) { }
    };

    class Window
    {
        using EventCallbackFunction = std::function<void(Event&)>;

    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;

            int Status;
            bool VSync;
                
            EventCallbackFunction EventCallback;
        };

        WindowData m_Data;

    public:

        Window(const WindowProperties& properties);
        ~Window();

        void onUpdate();

        inline unsigned int getWidth() const { return m_Data.Width; }
        inline unsigned int getHeight() const { return m_Data.Height; }
        inline int getStatus() const { return m_Data.Status; }
        inline GLFWwindow* getGLFWWindow() const { return m_Window; }

        void setEventCallback(const EventCallbackFunction& callback) { m_Data.EventCallback = callback; }
        void setVSync(bool enabled);
        inline bool isVSync() const { return m_Data.VSync; };

        static Window* Create(const WindowProperties& properties = WindowProperties());
    };
}