#include "Connect4Scene.h"

#include "Game/Systems.h"

Connect4Scene::Connect4Scene(unsigned int width, unsigned int height) : Elysium::Scene("Connect4"),
m_Camera(-m_Height * (float)(width / height), m_Height * (float)(width / height), -m_Height * 0.5f, m_Height * 0.5f),
m_SpriteSheet("res/texture/platformPack_tilesheet.png")
{
    m_CoinTextures[0] = m_SpriteSheet.getTextureData();
    m_CoinTextures[0].subtextureCoordinates({ 10, 6 }, { 128, 128 });

    m_CoinTextures[1] = m_SpriteSheet.getTextureData();
    m_CoinTextures[1].subtextureCoordinates({ 11, 5 }, { 128, 128 });


    e_PhysicsSystem2D.createPhysicalBody(&m_Rectangles[8], Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Rectangle", 0.0f, { 0.0f, -1.0f }, { 25.5f, 2.0f });

    for (size_t i = 0; i < m_Rectangles.size() - 1; i++)
        e_PhysicsSystem2D.createPhysicalBody(&m_Rectangles[i], Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Rectangle", 0.0f, { (float)(-12.25f + 3.5f * i), 6.5f }, { 1.0f, 13.0f });

    m_Camera.setPosition({ 0.0f, 5.0f, 0.0f });
}

Connect4Scene::~Connect4Scene()
{
    e_PhysicsSystem2D.clear();
}

void Connect4Scene::onUpdate(Elysium::Timestep ts)
{
    if (m_GameOver)
    {
        if (m_Restart)
        {
            m_Coins.fill(std::make_pair(nullptr, 0));
            e_PhysicsSystem2D.clear();

            m_CoinIndex = 0;
            m_GameOver = false;
            m_Restart = false;

            m_Grid.clear();

            e_PhysicsSystem2D.createPhysicalBody(&m_Rectangles[8], Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Rectangle", 0.0f, { 0.0f, -1.0f }, { 25.5f, 2.0f });

            for (size_t i = 0; i < m_Rectangles.size() - 1; i++)
                e_PhysicsSystem2D.createPhysicalBody(&m_Rectangles[i], Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Rectangle", 0.0f, { (float)(-12.25f + 3.5f * i), 6.5f }, { 1.0f, 13.0f });
        }
    }

    if (m_MoveCooldown < 0.0f)
        m_MoveCooldown += ts;

    e_PhysicsSystem2D.onUpdate(ts);

    Elysium::Renderer::beginScene(m_Camera);
    for (Elysium::BodyHandle& body : m_Rectangles)
    {
        const Elysium::PhysicalBody2D& rectangle = e_PhysicsSystem2D.readPhysicalBody(body);
        Elysium::Renderer::drawQuad(rectangle.Position, rectangle.getSize(), { 0.0f, 0.0f, 1.0f, 1.0f });
    }
    for (size_t i = 0; i < m_CoinIndex; i++)
    {
        Elysium::Renderer::drawQuad(m_Coins[i].first->Position, m_Coins[i].first->getSize(), m_CoinTextures[m_Coins[i].second - 1]);
    }
    Elysium::Renderer::endScene();

    ImGui::Begin("Statistics");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Number of Draw Calls: %d", Elysium::Renderer::getStats().DrawCount);
    ImGui::Text("Number of Quads: %d", Elysium::Renderer::getStats().QuadCount);
    ImGui::Text("Red: %d : Blue %d", m_RedScore, m_BlueScore);
    ImGui::Checkbox("Restart", &m_Restart);
    ImGui::End();

    Elysium::Renderer::resetStats();
}

void Connect4Scene::onEvent(Elysium::Event& event)
{
    Elysium::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Elysium::MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(Connect4Scene::onMousePressedEvent));
    dispatcher.Dispatch<Elysium::WindowResizeEvent>(BIND_EVENT_FUNCTION(Connect4Scene::onWindowResizeEvent));
}

bool Connect4Scene::onMousePressedEvent(Elysium::MouseButtonPressedEvent& event)
{
    if (!m_GameOver && m_MoveCooldown >= 0.0f)
    {
        m_MoveCooldown = -1.0f;
        auto mousePosition = Elysium::Input::getMousePosition();
        auto width = Elysium::Application::Get().getWindow().getWidth();
        auto height = Elysium::Application::Get().getWindow().getHeight();

        auto position = m_Camera.getScreenToWorldPosition(width, height, mousePosition);

        if (position.y < 15.0f)
        {
            float xposition = 0.0f;
            size_t column = 0;
            if (position.x > -12.25f && position.x < -8.75f)
            {
                xposition = -10.5f;
                column = 0;
            }
            else if (position.x > -8.75f && position.x < -5.25f)
            {
                xposition = -7.0f;
                column = 1;
            }
            else if (position.x > -5.25f && position.x < -1.75f)
            {
                xposition = -3.5f;
                column = 2;
            }
            else if (position.x > -1.75f && position.x < 1.75f)
            {
                xposition = 0.0f;
                column = 3;
            }
            else if (position.x > 1.75f && position.x < 5.25f)
            {
                xposition = 3.5f;
                column = 4;
            }
            else if (position.x > 5.25f && position.x < 8.75f)
            {
                xposition = 7.0f;
                column = 5;
            }
            else if (position.x > 8.75f && position.x < 12.25f)
            {
                xposition = 10.5f;
                column = 6;
            }

            if (m_CoinIndex >= 42)
                m_GameOver = true;

            if (m_Grid.isValid(column))
            {
                m_Coins[m_CoinIndex].first = e_PhysicsSystem2D.createPhysicalBody(Elysium::BodyType::DYNAMIC, Elysium::Collider::CIRCLE,
                    "Coin", 1.0f, 
                    { xposition, 15.0f }, { 2.0f, 2.0f });
                m_Coins[m_CoinIndex++].second = m_Turn;
                m_Grid.appendColumn(column, m_Turn);

                if (m_Grid.isWinningMove(column, m_Turn))
                {
                    m_GameOver = true;
                    switch (m_Turn)
                    {
                    case 1:
                        m_BlueScore++;
                        break;
                    case 2:
                        m_RedScore++;
                        break;
                    }
                }

                switch (m_Turn)
                {
                case 1:
                    m_Turn = 2;
                    break;
                case 2:
                    m_Turn = 1;
                    break;
                }
                m_Grid.printGrid();
            }
        }
    }
    return false;
}

bool Connect4Scene::onWindowResizeEvent(Elysium::WindowResizeEvent& event)
{
    unsigned int width = event.getWidth();
    unsigned int height = event.getHeight();
    if (width > 0 && height > 0)
        m_Camera.setProjection(-m_Height * (float)(width / height), m_Height * (float)(width / height),
            -m_Height * 0.5f, m_Height * 0.5f);
    return false;
}