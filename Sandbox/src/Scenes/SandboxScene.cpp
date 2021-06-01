#include "SandboxScene.h"

SandboxScene::SandboxScene(unsigned int width, unsigned int height) : Scene("Sandbox"),
    m_Camera(-m_Height * (float)(width / height), m_Height * (float)(width / height), -m_Height * 0.5f, m_Height * 0.5f),
    m_ParticleSystem(25000, Elysium::UpdateDevice::CPU),
    m_Player({ { -12.5f, 20.0f } })
{
    m_Textures.reserve(12);
    m_Textures.emplace_back("res/texture/soccer_ball.png");
    m_Textures.emplace_back("res/texture/Idle (1).png");
    m_Textures.emplace_back("res/texture/platformPack_tilesheet.png");
    m_Textures.emplace_back("res/texture/background.png");

    m_Textures.emplace_back("res/texture/Run (1).png");
    m_Textures.emplace_back("res/texture/Run (2).png");
    m_Textures.emplace_back("res/texture/Run (3).png");
    m_Textures.emplace_back("res/texture/Run (4).png");
    m_Textures.emplace_back("res/texture/Run (5).png");
    m_Textures.emplace_back("res/texture/Run (6).png");
    m_Textures.emplace_back("res/texture/Run (7).png");
    m_Textures.emplace_back("res/texture/Run (8).png");

    m_Background = m_Textures[3].getTextureData();

    m_Particle.Position = { 0.0f, 0.0f };
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 1.0f, 1.0f };
    m_Particle.RotationSpeed = 180.0f;

    m_Particle.ColorBegin = { 0.0f, 0.0f, 1.0f, 1.0f };
    m_Particle.ColorEnd = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_Particle.TextureData = m_Textures[0].getTextureData();

    m_Particle.SizeBegin = 0.5f, m_Particle.SizeEnd = 0.0f, m_Particle.SizeVariation = 0.3f;

    m_Particle.LifeTime = 3.0f;

    // ---------------------------------------------------------------------------------- //

    m_Particle2.Position = { 0.0f, 0.0f };
    m_Particle2.Velocity = { 0.0f, 0.0f };
    m_Particle2.VelocityVariation = { 1.0f, 1.0f };
    m_Particle2.RotationSpeed = 180.0f;

    m_Particle2.ColorBegin = { 1.0f, 0.0f, 1.0f, 1.0f };
    m_Particle2.ColorEnd = { 0.0f, 0.0f, 0.0f, 1.0f };

    m_Particle2.SizeBegin = 0.15f, m_Particle.SizeEnd = 0.0f, m_Particle.SizeVariation = 0.05f;

    m_Particle2.LifeTime = 3.0f;

    // ---------------------------------------------------------------------------------- //

    m_Player.m_IdleTexture = m_Textures[1].getTextureData();
    m_Player.m_TextureData = m_Player.m_IdleTexture;

    for (size_t i = 4; i < m_Textures.size(); i++)
        m_Player.RunAnimation.Textures[i - 4] = m_Textures[i].getTextureData();

    m_GroundTexture = m_Textures[2].getTextureData();
    m_GroundTexture.subtextureCoordinates({ 0, 6 }, { 128, 128 });

    m_BoxTexture = m_Textures[2].getTextureData();
    m_BoxTexture.subtextureCoordinates({ 4, 1 }, { 128, 128 });

    m_BallTexture = m_Textures[0].getTextureData();

    e_PhysicsSystem2D.createPhysicalBody(&m_Ground, Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Ground", 10.0f, { 0.0f, 0.0f }, { 500.0f, 2.0f });
    e_PhysicsSystem2D.createPhysicalBody(&m_MoveableBox, Elysium::BodyType::DYNAMIC, Elysium::Collider::QUAD, "Box", 10.0f, { 4.5f, 25.0f }, { 2.0f, 2.0f });
    e_PhysicsSystem2D.createPhysicalBody(&m_Box, Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Static-Box-Left", 10.0f, { 2.5f, 2.0f }, { 2.0f, 2.0f });
    e_PhysicsSystem2D.createPhysicalBody(&m_sBox, Elysium::BodyType::STATIC, Elysium::Collider::QUAD, "Static-Box-Right", 10.0f, { 4.5f, 2.0f }, { 2.0f, 2.0f });

    e_PhysicsSystem2D.createPhysicalBody(&m_Ball, Elysium::BodyType::DYNAMIC, Elysium::Collider::CIRCLE, "Ball", 1.0f, { -2.0f, 10.0f }, { 2.0f, 2.0f });
    e_PhysicsSystem2D.createPhysicalBody(&m_Circle, Elysium::BodyType::DYNAMIC, Elysium::Collider::CIRCLE, "Circle", 1.0f, { -5.0f, 10.0f }, { 2.0f, 2.0f });

    Elysium::PhysicalBody2D* ground = e_PhysicsSystem2D.getPhysicalBody(m_Ground);
    ground->setFrictionCoefficient(1.0f);
    //ground->Rotation = glm::radians(10.0f);

    Elysium::PhysicalBody2D* ball = e_PhysicsSystem2D.getPhysicalBody(m_Ball);
    ball->setElasticityCoefficient(1.0f);

    Elysium::PhysicalBody2D* circle = e_PhysicsSystem2D.getPhysicalBody(m_Circle);

    m_Player.Ball = e_PhysicsSystem2D.getPhysicalBody(m_Ball);

    //----- MATRICES -----//
    Elysium::Matrix A({ {1, 2, 3, 4, 5} });
    Elysium::Matrix B( {6, 7, 8, 9, 10} );
    Elysium::Matrix C;

    C = Elysium::Matrix::Concatenate(A, B, false);
    C.print();

    C = Elysium::Matrix::Concatenate(Elysium::Matrix({ {1, 2}, {3, 4} }), Elysium::Matrix({ {1, 2}, {3, 4} }), false);
    C.print();

    A = Elysium::Matrix::Concatenate(A, B);
    A.print();

    C = Elysium::Matrix::Slice(A, 0, 0, 0, 4);
    C.print();

    C = Elysium::Matrix::Slice(A, 0, 0, 4, 5);
    C.print();

    //--- PERCEPTRON ---//
    Elysium::Perceptron perceptron;

    constexpr size_t epochs = 5;

    Elysium::Matrix ANDGateData({ { 0.0f, 0.0f, 0.0f }, 
                                { 1.0f, 0.0f, 0.0f }, 
                                { 0.0f, 1.0f, 0.0f }, 
                                { 1.0f, 1.0f, 1.0f } });

    perceptron.fit(Elysium::Matrix::Slice(ANDGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(ANDGateData, 0, 0, 2, 3), 
        epochs);

    std::vector<float> results;
    perceptron.predict(Elysium::Matrix::Slice(ANDGateData, 0, 0, 0, 2), results);
    float meanAccuracy = perceptron.score(Elysium::Matrix::Slice(ANDGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(ANDGateData, 0, 0, 2, 3));

    for (float y : results)
        ELY_INFO("AND Gate: {0}", y);
    ELY_INFO("Mean Accuracy for AND: {0}", meanAccuracy);

    Elysium::Matrix ORGateData({ { 0.0f, 0.0f, 0.0f },
                                { 1.0f, 0.0f, 1.0f },
                                { 0.0f, 1.0f, 1.0f },
                                { 1.0f, 1.0f, 1.0f } });

    perceptron.fit(Elysium::Matrix::Slice(ORGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(ORGateData, 0, 0, 2, 3), 
        epochs);

    results.clear();
    perceptron.predict(Elysium::Matrix::Slice(ORGateData, 0, 0, 0, 2), results);
    meanAccuracy = perceptron.score(Elysium::Matrix::Slice(ORGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(ORGateData, 0, 0, 2, 3));

    for (float y : results)
        ELY_INFO("OR Gate: {0}", y);
    ELY_INFO("Mean Accuracy for OR: {0}", meanAccuracy);

    Elysium::Matrix XORGateData({ { 0.0f, 0.0f, 0.0f },
                                { 1.0f, 0.0f, 1.0f },
                                { 0.0f, 1.0f, 1.0f },
                                { 1.0f, 1.0f, 0.0f } });

    perceptron.fit(Elysium::Matrix::Slice(XORGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(XORGateData, 0, 0, 2, 3), 
        epochs);

    results.clear();
    perceptron.predict(Elysium::Matrix::Slice(XORGateData, 0, 0, 0, 2), results);
    meanAccuracy = perceptron.score(Elysium::Matrix::Slice(XORGateData, 0, 0, 0, 2),
        Elysium::Matrix::Slice(XORGateData, 0, 0, 2, 3));

    for (float y : results)
        ELY_INFO("XOR Gate: {0}", y);
    ELY_INFO("Mean Accuracy for XOR: {0}", meanAccuracy);
}

SandboxScene::~SandboxScene()
{
    e_PhysicsSystem2D.clear();
}

void SandboxScene::onUpdate(Elysium::Timestep ts)
{
    const Elysium::PhysicalBody2D* player = m_Player.getBody();
    const Elysium::PhysicalBody2D& ground = e_PhysicsSystem2D.readPhysicalBody(m_Ground);
    const Elysium::PhysicalBody2D& box = e_PhysicsSystem2D.readPhysicalBody(m_MoveableBox);
    const Elysium::PhysicalBody2D& sBox1 = e_PhysicsSystem2D.readPhysicalBody(m_Box);
    const Elysium::PhysicalBody2D& sBox2 = e_PhysicsSystem2D.readPhysicalBody(m_sBox);
    const Elysium::PhysicalBody2D& ball = e_PhysicsSystem2D.readPhysicalBody(m_Ball);
    const Elysium::PhysicalBody2D& circle = e_PhysicsSystem2D.readPhysicalBody(m_Circle);

    m_Camera.setPosition({ player->Position.x, player->Position.y + (player->getSize().y * 4.0f), 0.0f });

    auto mousePosition = Elysium::Input::getMousePosition();
    auto width = Elysium::Application::Get().getWindow().getWidth();
    auto height = Elysium::Application::Get().getWindow().getHeight();

    m_Particle.Position = m_Camera.getScreenToWorldPosition(width, height, mousePosition);
    m_Particle2.Position = { player->Position.x, player->Position.y };

    for (int i = 0; i < 5; i++)
    {
        m_ParticleSystem.Emit(m_Particle);
        m_ParticleSystem.Emit(m_Particle2);
    }
    e_PhysicsSystem2D.onUpdate(ts);

    m_Player.onUpdate(ts);

    Elysium::Renderer::beginScene(m_Camera);
    Elysium::Renderer::drawQuad({ 0.0f, 15.0f }, { 1000.0f, 30.0f }, m_Background, { 15.0f, 1.0f });
    Elysium::Renderer::endScene();

    m_ParticleSystem.onUpdate(ts);
    m_ParticleSystem.onRender(m_Camera);

    Elysium::Renderer::beginScene(m_Camera);
    Elysium::Renderer::drawQuad(player->Position, player->getSize(), m_Player.m_TextureData);
    Elysium::Renderer::drawQuadWithRotation(ground.Position, ground.getSize(), ground.Rotation, m_GroundTexture);
    Elysium::Renderer::drawQuadWithRotation(box.Position, box.getSize(), box.Rotation, m_BoxTexture);
    Elysium::Renderer::drawQuad(sBox1.Position, sBox1.getSize(), m_BoxTexture);
    Elysium::Renderer::drawQuad(sBox2.Position, sBox2.getSize(), m_BoxTexture);
    Elysium::Renderer::drawQuadWithRotation(ball.Position, ball.getSize(), ball.Rotation, m_BallTexture);
    Elysium::Renderer::drawQuadWithRotation(circle.Position, circle.getSize(), circle.Rotation, m_BallTexture);
    Elysium::Renderer::endScene();

    ImGui::Begin("Statistics");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Number of Draw Calls: %d", Elysium::Renderer::getStats().DrawCount);
    ImGui::Text("Number of Quads: %d", Elysium::Renderer::getStats().QuadCount);
    ImGui::End();

    Elysium::Renderer::resetStats();
}

void SandboxScene::onEvent(Elysium::Event& event)
{
    Elysium::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Elysium::KeyPressedEvent>(BIND_EVENT_FUNCTION(SandboxScene::onKeyPressedEvent));
    dispatcher.Dispatch<Elysium::WindowResizeEvent>(BIND_EVENT_FUNCTION(SandboxScene::onWindowResizeEvent));

    m_Player.onEvent(event);
}

bool SandboxScene::onKeyPressedEvent(Elysium::KeyPressedEvent& event)
{
    return false;
}

bool SandboxScene::onWindowResizeEvent(Elysium::WindowResizeEvent& event)
{
    unsigned int width = event.getWidth();
    unsigned int height = event.getHeight();
    if (width > 0 && height > 0)
    m_Camera.setProjection(-m_Height * (float)(width / height), m_Height * (float)(width / height),
        -m_Height * 0.5f, m_Height * 0.5f);
    return false;
}