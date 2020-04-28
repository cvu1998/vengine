#include "ScreenSaver_Test.h"

namespace test {

    ScreenSaver_Test::ScreenSaver_Test() :
        m_Shader("res/shaders/basic.shader"),
        m_Height(1.0f), m_Width(1.0f),
        r(0.0f), inc(true),
        m_Translation(0.01f), x(0.0f), y(0.0f), m_SignX(false), m_SignY(false)
    {
        float vertices[] = {
           -m_Height, -m_Width, 0.0f, 0.0f,    // 0
            m_Height, -m_Width, 1.0f, 0.0f,    // 1
            m_Height,  m_Width, 1.0f, 1.0f,    // 2
           -m_Height,  m_Width, 0.0f, 1.0f     // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_vb = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        m_va.addBuffer(*m_vb, layout);

        m_ib = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader.bind();
        m_Shader.setUniform1i("u_UseTexture", 0);
    }

    ScreenSaver_Test::~ScreenSaver_Test()
    {
    }

    void ScreenSaver_Test::onUpdate(float deltaTime)
    {
        /***CHECK WITHIN BORDERS***/
        if (!(x > -3.0f && x < 3.0f) ||
            !(y > -2.0f && y < 2.0f)) {
            if (!(x > -3.0f && x < 3.0f) &&
                !(y > -2.0f && y < 2.0f) && m_Case != OutOfBounds::XY) {
                m_SignX = !m_SignX;
                m_SignY = !m_SignY;
                m_Case = OutOfBounds::XY;
            }
            else if (!(x > -3.0f && x < 3.0f) &&
                (y > -2.0f && y < 2.0f) && m_Case != OutOfBounds::X_ONLY) {
                m_SignX = !m_SignX;
                m_Case = OutOfBounds::X_ONLY;
            }
            else if ((x > -3.0f && x < 3.0f) &&
                !(y > -2.0f && y < 2.0f) && m_Case != OutOfBounds::Y_ONLY) {
                m_SignY = !m_SignY;
                m_Case = OutOfBounds::Y_ONLY;
            }
        }
        else {
            m_Case = OutOfBounds::IN_BOUNDS;
        }
        /***CHECK WITHIN BORDERS***/
        /***BOUNCE LOGIC***/
        if (m_SignX && m_SignY) {
            x -= m_Translation;
            y -= m_Translation;
        }
        else if (m_SignX && !m_SignY) {
            x -= m_Translation;
            y += m_Translation;
        }
        else if (!m_SignX && m_SignY) {
            x += m_Translation;
            y -= m_Translation;
        }
        else {
            x -= -m_Translation;
            y -= -m_Translation;
        }
        /***BOUNCE LOGIC***/
        /***COLOR CHANGE***/
        if (r < 1.0f && inc) {
            r += 0.01f;
        }
        else if (r > 0 && !inc) {
            r -= 0.01f;
        }
        else {
            inc = !inc;
        }
        /***COLOR CHANGE***/
    }

    void ScreenSaver_Test::onRender(const glm::mat4& proj, const glm::mat4& view)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
        glm::mat4 mvp = proj * view * model;

        m_Shader.setUniform4f("u_Color", r, 0.0f, 1.0f, 1.0f);
        m_Shader.setUniformMat4f("u_MVP", mvp);

        Renderer::draw(m_va, *m_ib, m_Shader);
    }

    void ScreenSaver_Test::onImGuiRender()
    {
        ImGui::SliderFloat("Translation Velocity", &m_Translation, 0.001f, 1.5f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}