#include "Scene_Zelda.h"

#include "GameEngine.h"
#include "imgui.h"
#include "imgui-SFML.h"

Scene_Zelda::Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_LevelPath(levelPath)
{
    init(levelPath);
}

void Scene_Zelda::update()
{
    m_EntityManager.update();

    // Implement pause functionality

    sAI();
    sMovement();
    sStatus();
    sCollision();
    sAnimation();
    sCamera();
    sGUI();

    m_CurrentFrame += 1;
}

void Scene_Zelda::init(const std::string& levelPath)
{
    loadLevel(levelPath);

    m_GridText.setCharacterSize(12);
    m_GridText.setFont(m_Game->assets().getFont("Tech"));

    // Register the actions required to play the game

    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::G, "TOGGLE_FOLLOW");
}

void Scene_Zelda::loadLevel(const std::string& filename)
{
    m_EntityManager = EntityManager();

    // load the level file and put all entities in the manager
    // use the getPosition() function below to convert room-tile coords to game world coords

    spawnPlayer();
}

Vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty) const
{
    // Implement this function, which takes in the room (rx, ry) coordinate
    // as well as the tile (tx, ty) coordinate, and returns the Vec2 game world
    // position of the center of the entity
    return Vec2();
}

void Scene_Zelda::spawnPlayer()
{
    auto p = m_EntityManager.addEntity("player");
    p->add<CTransform>(Vec2(640, 480));
    p->add<CAnimation>(m_Game->assets().getAnimation("StandDown"), true);
    p->add<CBoundingBox>(Vec2(48, 48), true, false);
    p->add<CHealth>(7, 3);

    // Implement this function so that it uses the parameters input from the level file
    // These parameters should be stored in the m_PlayerConfig variable
}

void Scene_Zelda::sAnimation()
{
    // Implement player facing direction animation
    // Implement sword animation based on player facing
    //  The sword should move if the player changes direction mid swing
    // Implement destruction of entities with non repeating finished animations
}

void Scene_Zelda::sCamera()
{
    // TODO implement camera logic

    sf::View view = m_Game->window().getView();

    if (m_Follow)
    {
        // TODO
    }
    else
    {
        // TODO
    }

    m_Game->window().setView(view);
}

void Scene_Zelda::sMovement()
{
    // Implement all player movement functionality here based on
    // the player's input component variables
}


void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity)
{
    // Implement the spawning of the sword which
    // - should be given the appropriate lifespan
    // - should spawn at the appropriate location based on player's facing direction
    // - be given a damage value of 1
    // - should play a "Slash" sound
}

void Scene_Zelda::onEnd()
{

}

std::shared_ptr<Entity> Scene_Zelda::player()
{

    auto& entities = m_EntityManager.getEntities("player");
    if (entities.size() != 1)
    {
        std::cerr << "There is currently no player entity" << std::endl;
        exit(-1);
    }

    return entities[0];
}

void Scene_Zelda::sStatus()
{
    // Implement lifespan here
    // Implement invincibility frame here
}

void Scene_Zelda::sAI()
{
    // Implement Follow behavior
    // Implement Patrol behavior
}

void Scene_Zelda::sCollision()
{
    // Implement entity - tile collisions
    // Implement player - enemy collisions with appropriate damage calculations
    // Implement Sword - NPC collisions
    // Implement entity - heart collisions and life gain logic
    // Implement black tile collisions / teleporting

    // You may want to use helper functions for these behaviors or this function will get long
}

void Scene_Zelda::sGUI()
{
    ImGui::Begin("Scene Properties");
    ImGui::SetWindowFontScale(1.5f);

    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Debug"))
        {
            ImGui::Checkbox("Draw Grid", &m_DrawGrid);
            ImGui::Checkbox("Draw Textures", &m_DrawTextures);
            ImGui::Checkbox("Draw Debug", &m_DrawDebug);
            ImGui::Checkbox("Follow Cam", &m_Follow);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations"))
        {
            // TODO
            ImGui::Text("Do this");

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Entity Manager"))
        {
            // TODO
            ImGui::Text("Do this too");

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Scene_Zelda::sRender()
{
    m_Game->window().clear(sf::Color(255, 192, 122));
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    if (m_DrawTextures)
    {
        for (auto e : m_EntityManager.getEntities())
        {
            auto& transform = e->get<CTransform>();
            sf::Color c = sf::Color::White;
            if (e->has<CInvincibility>())
            {
                c = sf::Color(255, 255, 255, 128);
            }

            if (e->has<CAnimation>())
            {
                auto& animation = e->get<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                animation.getSprite().setColor(c);
                m_Game->window().draw(animation.getSprite());
            }
        }

        // draw health bars
        for (auto e : m_EntityManager.getEntities())
        {
            auto& transform = e->get<CTransform>();
            if (e->has<CHealth>())
            {
                auto& h = e->get<CHealth>();
                Vec2 size(64, 6);
                sf::RectangleShape rect({ size.x, size.y });
                rect.setPosition(transform.pos.x - 32, transform.pos.y - 48);
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2.0f);
                m_Game->window().draw(rect);

                float ratio = (float)(h.current) / h.max;
                size.x *= ratio;
                rect.setSize({ size.x, size.y });
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0.0f);
                m_Game->window().draw(rect);

                for (int i = 0; i < h.max; i++)
                {
                    tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64.0f * 1 / h.max, 0.0f));
                    m_Game->window().draw(tick);
                }
            }
        }
    }

    if (m_DrawGrid)
    {
        float leftX = m_Game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_GridSize.x;
        float topY = m_Game->window().getView().getCenter().y - height() / 2;
        float bottomY = topY + height() + m_GridSize.y;
        float nextGridX = leftX - ((int)leftX % (int)m_GridSize.x);
        float nextGridY = topY - ((int)topY % (int)m_GridSize.y);

        for (float x = nextGridX; x < rightX; x += m_GridSize.x)
        {
            drawLine(Vec2(x, 0.0f), Vec2(x, height()));
        }

        for (float y = 0; y < bottomY; y += m_GridSize.y)
        {
            drawLine(Vec2(leftX, y), Vec2(rightX, y));

            for (float x = nextGridX; x < rightX; x += m_GridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_GridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_GridSize.y);
                m_GridText.setString("(" + xCell + "," + yCell + ")");
                m_GridText.setPosition(x + 3.0f, height() - y - m_GridSize.y + 2.0f);
                m_Game->window().draw(m_GridText);
            }
        }
    }

    if (m_DrawCollision)
    {
        sf::CircleShape dot(4.0f);
        dot.setFillColor(sf::Color::Black);
        for (auto e : m_EntityManager.getEntities())
        {
            if (e->has<CBoundingBox>())
            {
                auto& box = e->get<CBoundingBox>();
                auto& transform = e->get<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color::Transparent);

                if (box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Black); }
                else if (box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::Blue); }
                else if (!box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Red); }
                else if (!box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::White); }
                rect.setOutlineThickness(1.0f);
                m_Game->window().draw(rect);
            }

            if (e->has<CPatrol>())
            {
                auto& patrol = e->get<CPatrol>().positions;
                for (size_t p = 0; p < patrol.size(); p++)
                {
                    dot.setPosition(patrol[p].x, patrol[p].y);
                    m_Game->window().draw(dot);
                }
            }

            if (e->has<CFollowPlayer>())
            {
                sf::VertexArray lines(sf::LinesStrip, 2);
                lines[0].position.x = e->get<CTransform>().pos.x;
                lines[0].position.y = e->get<CTransform>().pos.y;
                lines[0].color = sf::Color::Black;
                lines[1].position.x = player()->get<CTransform>().pos.x;
                lines[1].position.y = player()->get<CTransform>().pos.y;
                lines[1].color = sf::Color::Black;
                m_Game->window().draw(lines);
                dot.setPosition(e->get<CFollowPlayer>().home.x, e->get<CFollowPlayer>().home.y);
                m_Game->window().draw(dot);
            }
        }
    }
}

void Scene_Zelda::sDoAction(const Action& action)
{
    // Implement all actions described for the game here
    // Only the setting of the player's input component variables should be set here
    // Do minial logoc in this function

    if (action.type() == "START")
    {
        if (action.type() == "START")
        {
            if (action.name() == "TOGGLE_TEXTURE") { m_DrawTextures = !m_DrawTextures; }
            else if (action.name() == "TOGGLE_COLLISION") { m_DrawCollision = !m_DrawCollision; }
            else if (action.name() == "TOGGLE_GRID") { m_DrawGrid = !m_DrawGrid; }
            else if (action.name() == "TOGGLE_FOLLOW") { m_Follow = !m_Follow; }
            else if (action.name() == "PAUSE") { m_Paused = !m_Paused; }
            else if (action.name() == "QUIT") { onEnd(); }
        }
        else if (action.type() == "END")
        {

        }
    }
}
