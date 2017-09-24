#include "game_field.h"

#include "gdi_render\g_button.h"
#include "..\entities\bullet.h"
#include "gui_game_main_window.h"
#include "..\entities\tank_layout.h"
#include "../simple_tanks.h"

#include <unordered_map>
#include <string>
#include <utility>
#include <fstream>

namespace simple_tanks {

    struct Node {
        enum Type {
            null,
            brick,
            rock,
            base
        };
        Type type;
        Node* up;
        Node* down;
        Node* left;
        Node* right;
        int x;
        int y;
        std::string name;
    };




    const int GameField::kMapDim = 13*4;
    const int GameField::kBlockSize = 8;




    int UpdateBotCommandsCollection(GameField* sender) {
        sender->commandsCollectionMutex.lock();

        std::vector<Point> spawnPoints;

        //spawnPoints.push_back({ 0, 0 });
        //spawnPoints.push_back({ 12, 0 });
        spawnPoints.push_back({ 2, 3 });
        spawnPoints.push_back({ 10, 3 });
        //spawnPoints.push_back({ 2, 5 });
        //spawnPoints.push_back({ 8, 0 });

        {
            auto spawnPoint = spawnPoints[0];

            // Prolog dll call
            pUpdateNodes1(spawnPoint.X, spawnPoint.Y);

            // Parse
            std::ifstream cin("botmap.txt");
            std::vector<Direction> commands;

            std::string s;
            while (cin >> s) {
                if (s.find("up") != -1) {
                    commands.push_back(Direction::up);
                }
                if (s.find("down") != -1) {
                    commands.push_back(Direction::down);
                }
                if (s.find("left") != -1) {
                    commands.push_back(Direction::left);
                }
                if (s.find("right") != -1) {
                    commands.push_back(Direction::right);
                }
            }
            cin.close();

            sender->commandsCollection.push_back({ spawnPoint, commands });

            std::this_thread::sleep_for(std::chrono::seconds(1));


        }




        /*{
            auto spawnPoint = spawnPoints[1];

            // Prolog dll call
            pUpdateNodes2(spawnPoint.X, spawnPoint.Y);

            // Parse
            std::ifstream cin("botmap.txt");
            std::vector<Direction> commands;

            std::string s;
            while (cin >> s) {
                if (s.find("up") != -1) {
                    commands.push_back(Direction::up);
                }
                if (s.find("down") != -1) {
                    commands.push_back(Direction::down);
                }
                if (s.find("left") != -1) {
                    commands.push_back(Direction::left);
                }
                if (s.find("right") != -1) {
                    commands.push_back(Direction::right);
                }
            }
            cin.close();

            sender->commandsCollection.push_back({ spawnPoint, commands });

            std::this_thread::sleep_for(std::chrono::seconds(1));

        }*/



        sender->commandsCollectionMutex.unlock();

        return 0;
    }


    int UpdateBotCommandsCollection2(GameField* sender) {
        sender->commandsCollectionMutex.lock();

        std::vector<Point> spawnPoints;

        //spawnPoints.push_back({ 0, 0 });
        //spawnPoints.push_back({ 12, 0 });
        spawnPoints.push_back({ 2, 3 });
        spawnPoints.push_back({ 10, 3 });
        //spawnPoints.push_back({ 2, 5 });
        //spawnPoints.push_back({ 8, 0 });

        {
            auto spawnPoint = spawnPoints[1];

            // Prolog dll call
            pUpdateNodes2(spawnPoint.X, spawnPoint.Y);

            // Parse
            std::ifstream cin("botmap.txt");
            std::vector<Direction> commands;

            std::string s;
            while (cin >> s) {
                if (s.find("up") != -1) {
                    commands.push_back(Direction::up);
                }
                if (s.find("down") != -1) {
                    commands.push_back(Direction::down);
                }
                if (s.find("left") != -1) {
                    commands.push_back(Direction::left);
                }
                if (s.find("right") != -1) {
                    commands.push_back(Direction::right);
                }
            }
            cin.close();
            commands.pop_back();
            sender->commandsCollection.push_back({ spawnPoint, commands });

            std::this_thread::sleep_for(std::chrono::seconds(1));

        }



        sender->commandsCollectionMutex.unlock();

        return 0;
    }










    GameField::GameField() : 
        gameover(false),
        mapTexture(new Bitmap(L"resources/map.png")),
        brickTexture(new Image(L"resources/block.png")),
        rockTexture(new Image(L"resources/rock.png")),
        blackBrush(Color::Black), 
        brickBrush(brickTexture.get()),
        rockBrush(rockTexture.get()),
        map(kMapDim, std::vector<Block>(kMapDim, Block())),
        nodemap(kMapDim / 4, std::vector<Node>(kMapDim / 4)),
        tanksSpawnerTerminate(false),
        worldStateRegeneratorTerminate(false) {

        SetWidth(kBlockSize * kMapDim); //416
        SetHeight(kBlockSize * kMapDim); //416

        // Map init
        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                Color color;
                mapTexture->GetPixel(i, j, &color);
                if (color.GetValue() == Color::Red) {
                    map[i][j].SetType(Block::Type::brick);
                }
                if (color.GetValue() == Color::White) {
                    map[i][j].SetType(Block::Type::rock);
                }
				map[i][j].SetPos(i*kBlockSize, j*kBlockSize);
            }
        }

        Base* base = new Base();
        this->base = base;
        base->SetPosition(192, 384);
        Add(base);

        // user tank

        Tank* userTank = new Tank(this, TankLayout::GetGreenTankLayout(TankLayout::Direction::Up));
        this->userTank = userTank;
        tanks.push_back(userTank);
        userTank->MoveTo(4 * 4 * 8, 12 * 4 * 8);

        auto manip = [&](KeyEvent* keyEvent, Tank* tank, bool act) {
            switch (keyEvent->keyCode) {
                case KeyEvent::KeyCodes::alphaW:
                    tank->MoveUp(act);
                    break;
                case KeyEvent::KeyCodes::alphaS:
                    tank->MoveDown(act);
                    break;
                case KeyEvent::KeyCodes::alphaA:
                    tank->MoveLeft(act);
                    break;
                case KeyEvent::KeyCodes::alphaD:
                    tank->MoveRight(act);
                    break;
                case KeyEvent::KeyCodes::space:
                    if (act) {
                        tank->Shoot();
                    }
                    break;
                case KeyEvent::KeyCodes::f1:
                    if (act) {
                        isdebug = !isdebug;
                    }
                    break;
                default:
                    break;
            }
        };

        userTank->AddKeyPressListener([=](Frame* frame, KeyEvent* keyEvent) {
            manip(keyEvent, (Tank*)frame, true);
        });

        userTank->AddKeyReleaseListener([=](Frame* frame, KeyEvent* keyEvent) {
            manip(keyEvent, (Tank*)frame, false);
        });
        

        AddLeftMouseButtonPressListener([=](Frame* frame, MouseEvent* keyEvent) {
            ((Tank*)frame)->AutoMoveTo(keyEvent->xAbs - (800 - this->GetWidth()) / 2, keyEvent->yAbs);
        });


        Add(userTank);


        // World state regenerator

        // Node init
        for (int i = 0; i < nodemap.size(); i++) {
            for (int j = 0; j < nodemap.size(); j++) {
                Node& node = nodemap[i][j];
                if (i - 1 >= 0) {
                    node.left = &nodemap[i-1][j];
                }
                if (i + 1 < nodemap.size()) {
                    node.right = &nodemap[i+1][j];
                }
                if (j - 1 >= 0) {
                    node.up = &nodemap[i][j-1];
                }
                if (j + 1 < nodemap.size()) {
                    node.down = &nodemap[i][j+1];
                }
                node.x = i*kBlockSize*4;
                node.y = j*kBlockSize*4;
            }
        }


        UpdateNodeState();

        new std::thread([=]() {UpdateBotCommandsCollection(this); });
        new std::thread([=]() {UpdateBotCommandsCollection2(this); });
        


        // Enemy tanks spawner


        tanksSpawner.reset(new std::thread([&]() {
            //DEPRECATED
            while (!tanksSpawnerTerminate) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                commandsCollectionMutex.lock();
                if (commandsCollection.empty()) {
                    continue;
                }
                tanksMutex.lock();

                std::pair<Point, std::vector<Direction>> botData = commandsCollection[rand() % commandsCollection.size()];

                if (tanks.size() < 2) {
                    Tank* enemy = new Tank(this, TankLayout::GetWhiteTankLayout(TankLayout::Direction::Down), botData.second);
                    tanks.push_back(enemy);
                    enemy->MoveTo(botData.first.X * 32, botData.first.Y * 32);
                    Add(enemy);
                }

                tanksMutex.unlock();
                commandsCollectionMutex.unlock();
            }

        }));



        worldStateRegenerator.reset(new std::thread([&]() {

            while (!worldStateRegeneratorTerminate) {

                if (tanks.size() < 2) {

                    //UpdateNodeState();

                    //UpdateBotCommandsCollection();

                }

                std::this_thread::sleep_for(std::chrono::seconds(10));

            }


        }));





        AddKeyReleaseListener([&](Frame* frame, KeyEvent* keyEvent) {
            if ((keyEvent->keyCode == KeyEvent::KeyCodes::escape) && gameover && GuiGameMainWindow::GetInstance()) {
                GuiGameMainWindow::GetInstance()->Reclose();
            }
        });
    }


    GameField::~GameField() {
        tanksSpawnerTerminate = true;
        worldStateRegeneratorTerminate = true;
        tanksSpawner->join();
    }



    void GameField::SpawnBullet(Tank* tank) {
        Bullet* b = new Bullet(this, tank);
        bullets.push_back(b);
        Add(b);
    }


    void GameField::PaintPre(Graphics graphics) {

        if (gameover) {
            graphics.Clear(Color::Black);

            graphics.DrawString("Game over", &Font(L"Arial", 48), PointF(50, 150), &SolidBrush(Color::Red));
            if (userTank == nullptr && base == nullptr) {
                graphics.DrawString("You were completely defeated", &Font(L"Arial", 12), PointF(115, 220), &SolidBrush(Color::Red));
            } else if (userTank == nullptr) {
                graphics.DrawString("Your tank was destroyed", &Font(L"Arial", 12), PointF(130, 220), &SolidBrush(Color::Red));
            } else if (base == nullptr) {
                graphics.DrawString("Your base has been destroyed", &Font(L"Arial", 12), PointF(115, 220), &SolidBrush(Color::Red));
            }

            return;
        }

        // Test Bullet


        for (auto bullet : bullets) {
            TestBullet(bullet);
        }
        

        // Garbage remove
        Bullet* b = nullptr;
        do {
            b = nullptr;
            for (auto ob : bullets) {
                if (!ob->IsAlive()) {
                    b = ob;
                }
            }
            if (b != nullptr) {
                bullets.erase(std::find(bullets.begin(), bullets.end(), b));
                Erase(b);
                delete b;
            }
        } while (b != nullptr);
        
        tanksMutex.lock();
        Tank* t = nullptr;
        do {
            t = nullptr;
            for (auto ob : tanks) {
                if (!ob->IsAlive()) {
                    t = ob;
                }
            }
            if (t != nullptr) {
                tanks.erase(std::find(tanks.begin(), tanks.end(), t));
                Erase(t);
                if (userTank == t) {
                    userTank = nullptr;
                    gameover = true;
                }
                delete t;
            }
        } while (t != nullptr);
        tanksMutex.unlock();


        if (base && !base->IsAlive()) {
            Erase(base);
            delete base;
            base = nullptr;
            gameover = true;
        }



        graphics.FillRectangle(&blackBrush, Rect(0, 0, width, height));

        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                int x = i * kBlockSize;
                int y = j * kBlockSize;
                if (map[i][j].GetType() == Block::Type::brick) {
                    graphics.FillRectangle(&brickBrush, Rect(x, y, kBlockSize, kBlockSize));
                }
                if (map[i][j].GetType() == Block::Type::rock) {
                    graphics.FillRectangle(&rockBrush, Rect(x, y, kBlockSize, kBlockSize));
                }
            }
        }

        SolidBrush brush(Color(50, 0, 255, 0));
        SolidBrush bbrush(Color(50, 0, 0, 255));
//#if _DEBUG
        if (isdebug) {
            for (int i = 0; i < nodemap.size(); i++) {
                for (int j = 0; j < nodemap.size(); j++) {
                    Node &node = nodemap[i][j];
                    if (node.type == node.null) {
                        graphics.FillRectangle(&brush, Rect(node.x, node.y, kBlockSize * 4, kBlockSize * 4));
                    }
                    if (node.type == node.base) {
                        graphics.FillRectangle(&bbrush, Rect(node.x, node.y, kBlockSize * 4, kBlockSize * 4));
                    }
                }
            }
        }
        
//#endif

    }

    void GameField::PaintChildBuffers(Graphics graphics) {
        if (!gameover) {
            GdiControl::PaintChildBuffers(graphics);
        }
    }

    void GameField::PaintPost(Graphics graphics) {}





    void GameField::UpdateNodeState() {
        std::ofstream cout("map.txt");

        for (int i = 0; i < map.size(); i += 4) {
            for (int j = 0; j < map[i].size(); j += 4) {

                bool isNull = true;
                for (int k = i; k < i + 4 && isNull; k++) {
                    for (int l = j; l < j + 4 && isNull; l++) {
                        if (map[k][l].GetType() != Block::Type::null) {
                            isNull = false;
                        }
                    }
                }
                if (isNull) {
                    nodemap[i / 4][j / 4].type = Node::Type::null;
                } else {
                    nodemap[i / 4][j / 4].type = Node::Type::brick;
                }

            }
        }
        nodemap[6][12].type = Node::Type::base;
        if (nodemap[5][12].type != Node::Type::null) {
            nodemap[5][12].type = Node::Type::base;
        }
        if (nodemap[7][12].type != Node::Type::null) {
            nodemap[7][12].type = Node::Type::base;
        }
        if (nodemap[6][11].type != Node::Type::null) {
            nodemap[6][11].type = Node::Type::base;
        }


        // Import to db
        for (int i = 0; i < nodemap.size(); i++) {
            for (int j = 0; j < nodemap[i].size(); j++) {
                if (nodemap[i][j].type == Node::Type::base) {
                    cout << "types(node(" << i << ", " << j << "), base)\n";
                } else if (nodemap[i][j].type == Node::Type::null) {
                    cout << "types(node(" << i << ", " << j << "), road)\n";
                }

            }
        }


        int sz = nodemap.size();
        for (int i = 0; i < nodemap.size(); i++) {
            for (int j = 0; j < nodemap[i].size(); j++) {
                if (nodemap[i][j].type == Node::Type::brick || nodemap[i][j].type == Node::Type::rock) {
                    continue;
                }
                if (i - 1 >= 0 && nodemap[i - 1][j].type == Node::Type::null) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i - 1 << ", " << j << ")), " << "left)\n";
                }
                if (i + 1 < sz && nodemap[i + 1][j].type == Node::Type::null) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i + 1 << ", " << j << ")), " << "right)\n";
                }
                if (j - 1 >= 0 && nodemap[i][j - 1].type == Node::Type::null) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i << ", " << j - 1 << ")), " << "up)\n";
                }
                if (j + 1 < sz && nodemap[i][j + 1].type == Node::Type::null) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i << ", " << j + 1 << ")), " << "down)\n";
                }
                if (i - 1 >= 0 && nodemap[i - 1][j].type == Node::Type::base) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i - 1 << ", " << j << ")), " << "left)\n";
                }
                if (i + 1 < sz && nodemap[i + 1][j].type == Node::Type::base) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i + 1 << ", " << j << ")), " << "right)\n";
                }
                if (j - 1 >= 0 && nodemap[i][j - 1].type == Node::Type::base) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i << ", " << j - 1 << ")), " << "up)\n";
                }
                if (j + 1 < sz && nodemap[i][j + 1].type == Node::Type::base) {
                    cout << "commands(edge(node(" << i << ", " << j << "), node(" << i << ", " << j + 1 << ")), " << "down)\n";
                }
            }
        }

        cout.close();
    }

}