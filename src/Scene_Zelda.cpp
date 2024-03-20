#include "Scene_Zelda.h"

#include "GameEngine.h"
#include "Physics.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include <fstream>

Scene_Zelda::Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_LevelPath(levelPath)
{
    init(levelPath);
}

void Scene_Zelda::update()
{
    m_EntityManager.update();

    if (!m_Paused)
    {
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();
    }
    sGUI();

    m_CurrentFrame += 1;
}

void Scene_Zelda::init(const std::string& levelPath)
{
    loadLevel(levelPath);

    m_GridText.setCharacterSize(12);
    m_GridText.setFont(m_Game->assets().getFont("Mario"));

    // Register the actions required to play the game

    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::F, "TOGGLE_FOLLOW");
    registerAction(sf::Keyboard::Z, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::Q, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "ATTACK");
}

void Scene_Zelda::loadLevel(const std::string& filename)
{
    m_EntityManager = EntityManager();

    std::ifstream fin(filename);
    std::string type;

    while (fin >> type)
    {
        if (type == "Tile")
        {
            std::string name;
            int RX, RY, TX, TY, BM, BV;
            fin >> name >> RX >> RY >> TX >> TY >> BM >> BV;

            auto e = m_EntityManager.addEntity(type);

            auto& anim = m_Game->assets().getAnimation(name);
            e->add<CAnimation>(anim, true);
            e->add<CTransform>(getPosition(RX, RY, TX, TY));
            e->add<CBoundingBox>(anim.getSize(), BM == 1, BV == 1);
        }
        else if (type == "NPC")
        {
            std::string name, AI;
            int RX, RY, TX, TY, BM, BV, H, D;
            fin >> name >> RX >> RY >> TX >> TY >> BM >> BV >> H >> D >> AI;

            auto e = m_EntityManager.addEntity(type);

            auto& anim = m_Game->assets().getAnimation(name);
            Vec2 pos = getPosition(RX, RY, TX, TY);
            e->add<CAnimation>(anim, true);
            e->add<CTransform>(pos);
            e->add<CBoundingBox>(anim.getSize(), BM == 1, BV == 1);
            e->add<CHealth>(H);
            e->add<CDamage>(D);

            if (AI == "Follow")
            {
                float s;
                fin >> s;
                e->add<CFollowPlayer>(pos, s);
            }
            else if (AI == "Patrol")
            {
                float s;
                int n;
                fin >> s >> n;

                std::vector<Vec2> positions;
                for (int i = 0; i < n; i++)
                {
                    int x, y;
                    fin >> x >> y;
                    positions.push_back(getPosition(RX, RY, x, y));
                }
                e->add<CPatrol>(positions, s);
            }
        }
        else if (type == "Player")
        {
            fin >> m_PlayerConfig.X >> m_PlayerConfig.Y >> m_PlayerConfig.CX
                >> m_PlayerConfig.CY >> m_PlayerConfig.SPEED >> m_PlayerConfig.HEALTH;
        }
    }

    spawnPlayer();
}

Vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty) const
{
    float x = rx * (float)width() + tx * m_GridSize.x + m_GridSize. x / 2;
    float y = ry * (float)height() + ty * m_GridSize.y + m_GridSize.y / 2;
    return Vec2(x, y);
}

void Scene_Zelda::spawnPlayer()
{
    auto p = m_EntityManager.addEntity("Player");
    p->add<CAnimation>(m_Game->assets().getAnimation("StandDown"), true);
    p->add<CTransform>(Vec2(m_PlayerConfig.X, m_PlayerConfig.Y));
    p->add<CBoundingBox>(Vec2(m_PlayerConfig.CX, m_PlayerConfig.CY), true, false);
    p->add<CHealth>((int)m_PlayerConfig.HEALTH);
    p->add<CInput>();
    p->add<CState>();
}

static void placeSword(std::string& animName, CTransform& sTransform, const CTransform& eTransform)
{
    if (eTransform.facing.x != 0.0f)
    {
        sTransform.pos.x = eTransform.pos.x + eTransform.facing.x * 60.0f;
        sTransform.pos.y = eTransform.pos.y;
        sTransform.scale.x = eTransform.facing.x;
        animName = "SwordRight";
    }
    else if (eTransform.facing.y != 0.0f)
    {
        sTransform.pos.y = eTransform.pos.y - eTransform.facing.y * 60.0f;
        sTransform.pos.x = eTransform.pos.x;
        sTransform.scale.y = eTransform.facing.y;
        animName = "SwordUp";
    }
}

static void direction(std::string& animName, CTransform& transform)
{
    if (transform.facing.x != 0.0f)
    {
        animName.append("Right");
        transform.scale.x = transform.facing.x;
    }
    else if (transform.facing.y == -1.0f)
    {
        animName.append("Down");
        transform.scale.x = 1.0f;
    }
    else if (transform.facing.y == 1.0f)
    {
        animName.append("Up");
        transform.scale.x = 1.0f;
    }
}

void Scene_Zelda::sAnimation()
{
    auto& pTransform = player()->get<CTransform>();
    auto& pState = player()->get<CState>();

    if (pState.changed)
    {
        std::string animName = pState.state;
        direction(animName, pTransform);
        player()->add<CAnimation>(m_Game->assets().getAnimation(animName), true);
        pState.changed = false;
    }

    for (auto entity : m_EntityManager.getEntities())
    {
        if (entity->has<CAnimation>())
        {
            auto& anim = entity->get<CAnimation>();
            anim.animation.update();

            if (!anim.repeat && anim.animation.hasEnded())
            {
                entity->destroy();
            }
        }
    }
    // Implement player facing direction animation
    // Implement sword animation based on player facing
    //  The sword should move if the player changes direction mid swing
    // Implement destruction of entities with non repeating finished animations
}

void Scene_Zelda::sCamera()
{
    sf::View view = m_Game->window().getView();
    auto& pos = player()->get<CTransform>().pos;

    if (m_Follow)
    {
        view.setCenter(pos.x, pos.y);
    }
    else
    {
        int w = (int)width();
        int h = (int)height();

        int rx = (int)pos.x / w;
        if (pos.x < 0.0f) rx -= 1;

        int ry = (int)pos.y / h;
        if (pos.y < 0.0f) ry -= 1;

        view.setCenter(rx * w + w / 2.0f, ry * h + h / 2.0f);
    }

    m_Game->window().setView(view);
}

void Scene_Zelda::moveEntities(const std::string& tag)
{
    for (auto e : m_EntityManager.getEntities(tag))
    {
        auto& transform = e->get<CTransform>();
        transform.prevPos = transform.pos;
        transform.pos += transform.velocity;
    }
}

void Scene_Zelda::sMovement()
{
    auto& input = player()->get<CInput>();
    auto& transform = player()->get<CTransform>();
    auto& state = player()->get<CState>();

    float speed = m_PlayerConfig.SPEED;
    Vec2 facing = transform.facing;
    bool run = true;
    if (input.up)
    {
        transform.velocity = Vec2(0.0f, -speed);
        facing = Vec2(0.0, 1.0f);
    }
    else if (input.down)
    {
        transform.velocity = Vec2(0.0f, speed);
        facing = Vec2(0.0, -1.0f);
    }
    else if (input.right)
    {
        transform.velocity = Vec2(speed, 0.0f);
        facing = Vec2(1.0, 0.0f);
    }
    else if (input.left)
    {
        transform.velocity = Vec2(-speed, 0.0f);
        facing = Vec2(-1.0, 0.0f);
    }
    else
    {
        transform.velocity = Vec2();
        run = false;
    }

    if (transform.facing != facing)
    {
        transform.facing = facing;
        state.changed = true;
    }
    if (input.attack)
    {
        if (state.state != "Atk")
        {
            state.state = "Atk";
            state.changed = true;
        }
    }
    else if (run)
    {
        if (state.state != "Run")
        {
            state.state = "Run";
            state.changed = true;
        }
    }
    else
    {
        if (state.state != "Stand")
        {
            state.state = "Stand";
            state.changed = true;
        }
    }

    moveEntities("Player");
    moveEntities("NPC");

    for (auto sword : m_EntityManager.getEntities("Sword"))
    {
        std::string animName;
        placeSword(animName, sword->get<CTransform>(), player()->get<CTransform>());
        sword->add<CAnimation>(m_Game->assets().getAnimation(animName), true);
    }
}

void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity)
{
    auto& input = entity->get<CInput>();
    if (input.attack)
        return;
 
    auto sword = m_EntityManager.addEntity("Sword");
    auto& eTransform = entity->get<CTransform>();
    sword->add<CTransform>();
    std::string animName;
    placeSword(animName, sword->get<CTransform>(), eTransform);
    auto& anim = m_Game->assets().getAnimation(animName);
    sword->add<CAnimation>(anim, true);
    sword->add<CBoundingBox>(anim.getSize());
    sword->add<CDamage>(1);
    sword->add<CLifeSpan>(10, m_CurrentFrame);
    input.attack = true;
    m_Game->playSound("Slash");
}

void Scene_Zelda::onEnd()
{
    m_Game->changeScene("MENU", nullptr, true);
}

std::shared_ptr<Entity> Scene_Zelda::player()
{
    auto& entities = m_EntityManager.getEntities("Player");
    if (entities.size() != 1)
    {
        std::cerr << "There is currently no player entity" << std::endl;
        exit(-1);
    }

    return entities[0];
}

void Scene_Zelda::sStatus()
{
    for (auto sword : m_EntityManager.getEntities("Sword"))
    {
        auto& lifespan = sword->get<CLifeSpan>();

        if ((int)m_CurrentFrame - lifespan.frameCreated > lifespan.lifespan)
        {
            sword->destroy();
            player()->get<CInput>().attack = false;
        }
    }
    
    if (player()->has<CInvincibility>())
    {
        auto& invincibility = player()->get<CInvincibility>();
        invincibility.iframes -= 1;

        if (invincibility.iframes < 0)
        {
            player()->remove<CInvincibility>();
        }
    }
}

void Scene_Zelda::sAI()
{
    // Implement Follow behavior
    // Implement Patrol behavior
    for (auto enemy : m_EntityManager.getEntities("NPC"))
    {
        if (enemy->has<CFollowPlayer>())
        {

        }
        
        if (enemy->has<CPatrol>())
        {
            auto& patrol = enemy->get<CPatrol>();
            auto& transform = enemy->get<CTransform>();
            
            if (patrol.positions[patrol.currentPosition].distSq(transform.pos) < 25.0f)
            {
                patrol.currentPosition += 1;
                patrol.currentPosition %= patrol.positions.size();
            }

            transform.velocity = patrol.positions[patrol.currentPosition] - transform.pos;
            transform.velocity.setMag(patrol.speed);
        }
    }
}

static void resolveTileCollision(std::shared_ptr<Entity> tile, std::shared_ptr<Entity> entity)
{
    Vec2 overlap = Physics::GetOverlap(tile, entity);
    Vec2 prevOverlap = Physics::GetPreviousOverlap(tile, entity);

    if (overlap.x >= 0.0f && overlap.y >= 0.0f)
    {
        auto& tilePos = tile->get<CTransform>().pos;
        auto& entityPos = entity->get<CTransform>().pos;
        auto& entityPrevPos = entity->get<CTransform>().prevPos;

        if (prevOverlap.y > 0.0f)
        {
            entityPos.x += entityPos.x < tilePos.x ? -overlap.x : overlap.x;
        }

        if (prevOverlap.x > 0.0f)
        {
            entityPos.y += entityPos.y < tilePos.y ? -overlap.y : overlap.y;
        }
    }
}

void Scene_Zelda::tileCollision()
{
    for (auto tile : m_EntityManager.getEntities("Tile"))
    {
        resolveTileCollision(tile, player());

        for (auto npc : m_EntityManager.getEntities("NPC"))
        {
            resolveTileCollision(tile, npc);
        }
    }
}

void Scene_Zelda::playerEnemyCollision()
{
    auto p = player();

    if (p->has<CInvincibility>())
        return;

    for (auto enemy : m_EntityManager.getEntities("NPC"))
    {
        Vec2 overlap = Physics::GetOverlap(enemy, p);
        if (overlap.x >= 0.0f && overlap.y >= 0.0f)
        {
            p->get<CHealth>().current -= enemy->get<CDamage>().damage;
            p->add<CInvincibility>(30);

            if (p->get<CHealth>().current <= 0)
            {
                p->destroy();
                spawnPlayer();
                m_Game->playSound("LinkDie");
            }
            else
            {
                m_Game->playSound("LinkHurt");
            }
        }
    }
}

void Scene_Zelda::resolveHeartCollision(std::shared_ptr<Entity> heart, std::shared_ptr<Entity> entity)
{
    Vec2 overlap = Physics::GetOverlap(heart, entity);
    Vec2 prevOverlap = Physics::GetPreviousOverlap(heart, entity);

    if (overlap.x >= 0.0f && overlap.y >= 0.0f)
    {
        entity->get<CHealth>().current = entity->get<CHealth>().max;
        heart->destroy();
        m_Game->playSound("GetItem");
    }
}

void Scene_Zelda::heartCollision()
{
    for (auto tile : m_EntityManager.getEntities("Tile"))
    {
        if (tile->get<CAnimation>().animation.getName() != "Heart")
            continue;

        resolveHeartCollision(tile, player());

        for (auto enemy : m_EntityManager.getEntities("NPC"))
        {
            resolveHeartCollision(tile, enemy);
        }
    }
}

void Scene_Zelda::swordEnemyCollision()
{
    for (auto sword : m_EntityManager.getEntities("Sword"))
    {
        if (!sword->has<CDamage>())
            continue;

        for (auto enemy : m_EntityManager.getEntities("NPC"))
        {
            Vec2 overlap = Physics::GetOverlap(sword, enemy);

            if (overlap.x >= 0.0f && overlap.y >= 0.0f)
            {
                enemy->get<CHealth>().current -= sword->get<CDamage>().damage;
                sword->remove<CDamage>();

                if (enemy->get<CHealth>().current <= 0)
                {
                    enemy->destroy();
                    m_Game->playSound("EnemyDie");
                }
                else
                {
                    m_Game->playSound("EnemyHit");
                }
            }
        }
    }
}

void Scene_Zelda::sCollision()
{
    tileCollision();
    playerEnemyCollision();
    swordEnemyCollision();
    heartCollision();
    // Implement black tile collisions / teleporting
}

static void entityGUI(std::shared_ptr<Entity> e)
{
    ImGui::Text("%i", e->id());
    ImGui::SameLine();
    ImGui::Text(e->tag().c_str());
    ImGui::SameLine();
    ImGui::Text(e->get<CAnimation>().animation.getName().c_str());
    ImGui::SameLine();
    ImGui::Text("(%i,%i)", (int)e->get<CTransform>().pos.x, (int)e->get<CTransform>().pos.y);
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
            if (ImGui::CollapsingHeader("All entities", ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                for (auto e : m_EntityManager.getEntities())
                {
                    entityGUI(e);
                }
            }

            if (ImGui::CollapsingHeader("By tag"))
            {
                for (auto& [tag, entityVec] : m_EntityManager.getEntityMap())
                {
                    if (ImGui::CollapsingHeader(tag.c_str()))
                    {
                        for (auto e : entityVec)
                        {
                            entityGUI(e);
                        }
                    }
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Scene_Zelda::sRender()
{
    if (m_Paused)
        m_Game->window().clear(sf::Color(205, 142, 72));
    else
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
            drawLine(Vec2(x, topY), Vec2(x, bottomY));
        }

        for (float y = nextGridY; y < bottomY; y += m_GridSize.y)
        {
            drawLine(Vec2(leftX, y), Vec2(rightX, y));

            for (float x = nextGridX; x < rightX; x += m_GridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_GridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_GridSize.y);
                m_GridText.setString("(" + xCell + "," + yCell + ")");
                m_GridText.setPosition(x + 3.0f, y + 2.0f);
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
    // Do minial logic in this function
    auto& input = player()->get<CInput>();

    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE") { m_DrawTextures = !m_DrawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_DrawCollision = !m_DrawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_DrawGrid = !m_DrawGrid; }
        else if (action.name() == "TOGGLE_FOLLOW") { m_Follow = !m_Follow; }
        else if (action.name() == "PAUSE") { m_Paused = !m_Paused; }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "UP") { input.up = true; }
        else if (action.name() == "DOWN") { input.down = true; }
        else if (action.name() == "LEFT") { input.left = true; }
        else if (action.name() == "RIGHT") { input.right = true; }
        else if (action.name() == "ATTACK") { spawnSword(player()); }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "UP") { input.up = false; }
        else if (action.name() == "DOWN") { input.down = false; }
        else if (action.name() == "LEFT") { input.left = false; }
        else if (action.name() == "RIGHT") { input.right = false; }
    }
}
