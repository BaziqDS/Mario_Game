#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <list>
#include <unordered_map>
#include <fstream>
using namespace std;
using namespace sf;
#define marioScale
vector<Vector2f> pos;
vector<Vector2f> mush;
class Character;
class Mushroom;
class Goomba;
class Mario;
class Object
{
public:
    Sprite objSprite, coinSprite;
    IntRect objRect;
    bool hit, coin, coinComplete;
    Vector2f originalPos;
    Clock coinClock, brickClock;
    string marioSize;
    Sprite brickAnim[4];
    Object *flagObject;
    Object()
    {
        coinComplete = false;
        coin = false;
        marioSize = "small";
        hit = false;
    }
    void animateCoinBlock(RenderWindow &window)
    {
        if (hit)
        {
            objRect = IntRect(858, 0, 32, 32);
            objSprite.setTextureRect(objRect);
            if (coin)
            {
                if (coinClock.getElapsedTime().asMilliseconds() <= 250.f)
                {
                    coinSprite.move(0.0f, -5.62);
                    window.draw(coinSprite);
                }
                else if (coinClock.getElapsedTime().asMilliseconds() > 250.f && coinClock.getElapsedTime().asMilliseconds() <= 350)
                {
                    coinSprite.move(0.0f, 4.62);
                    window.draw(coinSprite);
                }
                else
                {

                    coinSprite = Sprite();
                    coin = false;
                }
            }
            else
            {
                coin = false;
            }
        }
        else
        {

            if (coinClock.getElapsedTime().asMilliseconds() >= 300.f)
            {

                objRect.left += 33;
                if (objRect.left == 858)
                {
                    objRect.left = 759;
                }
                coinClock.restart();
            }

            objSprite.setTextureRect(objRect);
        }
    }
    void animateInvisibleBlock()
    {
        if (hit)
        {
            objRect = IntRect(858, 0, 32, 32);
            objSprite.setTextureRect(objRect);
        }
    }
    void animateBrick(RenderWindow &window)
    {

        if (hit && marioSize == "small")
        {
            if (brickClock.getElapsedTime().asMilliseconds() <= 60.0f)
            {
                objSprite.move(0, -6);
            }
            else
            {
                if (objSprite.getPosition() != originalPos)
                {
                    objSprite.move(0, 6);
                }
            }
        }
        else if (hit && marioSize == "BIG")
        {
            objSprite = Sprite();
            if (brickClock.getElapsedTime().asMilliseconds() <= 150.0f)
            {
                brickAnim[0].move(-5.0f, -9.0f);
                brickAnim[1].move(-5.0f, -6.0f);
                brickAnim[2].move(5.0f, -9.0f);
                brickAnim[3].move(5.0f, -6.0f);
            }
            else if (brickClock.getElapsedTime().asMilliseconds() > 150.0f && brickClock.getElapsedTime().asMilliseconds() <= 400.0f)
            {
                brickAnim[0].move(-1.0f, 10.0f);
                brickAnim[1].move(-1.0f, 10.0f);
                brickAnim[2].move(1.0f, 10.0f);
                brickAnim[3].move(1.0f, 10.0f);
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    brickAnim[i] = Sprite();
                }
            }
            for (int i = 0; i < 4; i++)
            {
                window.draw(brickAnim[i]);
            }
        }
    }
};
class Entity
{
public:
    vector<Sprite> backgroundObj;                       // Background Objects
    unordered_map<string, list<Object *>> collisionObj; // Collision Objects (Hash Table)
};
string gameState;
SoundBuffer gameBuffer;
Sound gameSound;
class Map : public Entity
{
private:
    Texture mapTexture;

public:
    Map()
    {
        if (!mapTexture.loadFromFile("Resources/Tilesheet.png"))
        {
            exit(1);
        }
    }
    void loadMap()
    {
        ifstream inputFile("Levels/NEW MAP.txt");
        string line;
        while (getline(inputFile, line))
        {
            if (line.substr(0, 12) == "Ground Block")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 13; i < line.length(); i++)
                {
                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *groundblock = new Object;
                groundblock->objSprite = Sprite(mapTexture, rect);
                groundblock->objRect = rect;
                groundblock->objSprite.setPosition(position);
                groundblock->objSprite.setScale(2.f, 2.f);
                collisionObj["GroundBlocks"].push_back(groundblock);
            }
            else if (line.substr(0, 10) == "Background")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 11; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Sprite sprite(mapTexture, rect);
                sprite.setPosition(position);
                sprite.setScale(2.0f, 2.0f);
                backgroundObj.push_back(sprite);
            }
            else if (line.substr(0, 16) == "Invisible Blocks")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 17; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *inviBlock = new Object;
                inviBlock->objSprite = Sprite(mapTexture, rect);
                inviBlock->objRect = rect;
                inviBlock->objSprite.setPosition(position);
                inviBlock->objSprite.setScale(2.f, 2.f);
                collisionObj["InvisibleBlocks"].push_back(inviBlock);
            }
            else if (line.substr(0, 6) == "Stairs")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 7; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *stair = new Object;
                stair->objSprite = Sprite(mapTexture, rect);
                stair->objRect = rect;
                stair->objSprite.setPosition(position);
                stair->objSprite.setScale(2.f, 2.f);
                collisionObj["Stairs"].push_back(stair);
            }
            else if (line.substr(0, 5) == "Brick")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 6; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *brick = new Object;
                brick->objSprite = Sprite(mapTexture, rect);
                brick->objRect = rect;
                brick->objSprite.setPosition(position);
                brick->objSprite.setScale(2.f, 2.f);
                for (int i = 0; i < 4; i++)
                {
                    brick->brickAnim[i].setTexture(*brick->objSprite.getTexture());
                    if (i == 0)
                    {
                        brick->brickAnim[i].setPosition(brick->objSprite.getPosition());
                        brick->brickAnim[i].setTextureRect(IntRect(0, 0, 16, 16));
                    }
                    else if (i == 1)
                    {
                        brick->brickAnim[i].setPosition(brick->objSprite.getPosition().x, brick->objSprite.getPosition().y + brick->objSprite.getGlobalBounds().height / 2);
                        brick->brickAnim[i].setTextureRect(IntRect(0, 16, 16, 16));
                    }
                    else if (i == 2)
                    {
                        brick->brickAnim[i].setPosition(brick->objSprite.getPosition().x + brick->objSprite.getGlobalBounds().width / 2, brick->objSprite.getPosition().y);
                        brick->brickAnim[i].setTextureRect(IntRect(16, 0, 16, 16));
                    }
                    else if (i == 3)
                    {
                        brick->brickAnim[i].setPosition(brick->objSprite.getPosition().x + brick->objSprite.getGlobalBounds().width / 2, brick->objSprite.getPosition().y + brick->objSprite.getGlobalBounds().height / 2);
                        brick->brickAnim[i].setTextureRect(IntRect(16, 16, 16, 16));
                    }
                    brick->brickAnim[i].setScale(brick->brickAnim[i].getScale().x * 1.5f, brick->brickAnim[i].getScale().y * 1.5f);
                }
                collisionObj["Bricks"].push_back(brick);
            }
            else if (line.substr(0, 4) == "Pipe")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 5; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *pipe = new Object;
                pipe->objSprite = Sprite(mapTexture, rect);
                pipe->objRect = rect;
                pipe->objSprite.setPosition(position);
                pipe->objSprite.setScale(2.f, 2.f);
                collisionObj["Pipes"].push_back(pipe);
            }
            else if (line.substr(0, 4) == "Flag")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 5; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *flag = new Object;
                flag->objSprite = Sprite(mapTexture, rect);
                flag->objRect = rect;
                flag->objSprite.setPosition(position);
                flag->objSprite.setScale(2.f, 2.f);
                collisionObj["Flag"].push_back(flag);
            }
            else if (line.substr(0, 9) == "CoinBlock")
            {
                int index = 0;
                vector<string> properties = {"", "", "", "", "", ""};
                for (int i = 10; i < line.length(); i++)
                {

                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
                Object *coinblock = new Object;
                coinblock->objSprite = Sprite(mapTexture, rect);
                coinblock->coinSprite = Sprite(mapTexture, IntRect(759, 33, 32, 32));
                coinblock->coinSprite.setPosition(position);
                coinblock->coinSprite.setScale(2.f, 2.f);
                coinblock->objRect = rect;
                coinblock->objSprite.setPosition(position);
                coinblock->objSprite.setScale(2.f, 2.f);
                collisionObj["CoinBlocks"].push_back(coinblock);
            }
            else if (line.substr(0, 6) == "Goomba")
            {
                int index = 0;
                vector<string> properties = {" ", " "};

                for (int i = 7; i < line.length(); i++)
                {
                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                pos.push_back(position);
            }
            else if (line.substr(0, 8) == "Mushroom")
            {
                int index = 0;
                vector<string> properties = {" ", " "};
                for (int i = 9; i < line.length(); i++)
                {
                    if (line[i] == ',')
                    {
                        index++;
                    }
                    else
                    {
                        properties[index] += line[i];
                    }
                }
                Vector2f position(stoi(properties[0]), stoi(properties[1]));
                mush.push_back(position);
            }
        }
    }
    void drawMapBackground(RenderWindow &window)
    {
        for (int i = 0; i < backgroundObj.size(); i++)
        {
            window.draw(backgroundObj[i]);
        }
    }
    void drawMapCollisionOBJ(RenderWindow &window, bool finish)
    {

        for (auto &pair : collisionObj)
        {
            for (auto &obj : pair.second)
            {
                if (pair.first == "CoinBlocks")
                {
                    obj->animateCoinBlock(window);
                }
                if (pair.first == "InvisibleBlocks")
                {
                    obj->animateInvisibleBlock();
                }
                if (pair.first == "Bricks")
                {
                    obj->animateBrick(window);
                }
                if (pair.first == "Flag" && obj->objSprite.getPosition().x == 13728 && finish)
                {
                    window.draw(obj->objSprite);
                }
                window.draw(obj->objSprite);
            }
        }
    }
};
void resolveCollision(Map &, Character *, list<Mushroom *> &, RenderWindow &);
void checkGoombaCollision(Character *, list<Goomba *> &);
void checkMushroomCollision(Mario &, list<Mushroom *> &);
class Character
{
protected:
    Texture charTexture;
    Sprite charSprite;
    float gravity;
    Vector2f velocity;
    string currentState;
    Clock animationTimer;
    SoundBuffer characterBuffer;
    Sound characterSound;

public:
    virtual void update(Map &, list<Goomba *> &, list<Mushroom *> &, RenderWindow &) = 0;
    Sprite &getSprite()
    {
        return charSprite;
    }
    Vector2f getVelocity()
    {
        return velocity;
    }
    void setState(string newState)
    {
        currentState = newState;
    }
    void setVelocity(Vector2f newVelocity)
    {
        velocity = newVelocity;
    }
    void resetAnimationTimer()
    {
        animationTimer.restart();
    }
    void setScale(Vector2f newScale)
    {
        charSprite.setScale(newScale);
    }
    string &getState()
    {
        return currentState;
    }
};
class Mario : public Character
{
private:
    float velocityMin, velocityMax, velocityMaxY, acceleration, drag;
    bool canJump, rightTurn, leftTurn;
    string size;
    // used for animation
    Clock sizeClock;
    bool animSwitch;
    float frame;
    Color originalColor;
    bool invincibility;
    int i;

public:
    IntRect marioRect;
    Mario() : velocityMin(2.f), velocityMax(22.f), velocityMaxY(30.0f), drag(0.80f), acceleration(2.5f)
    {
        if (!charTexture.loadFromFile("Resources/mariosheet.png"))
        {
            exit(1);
        }
        gravity = 3.f;
        velocity = Vector2f(0.0f, 0.0f);
        marioRect = IntRect(209, 0, 17, 16);
        charSprite = Sprite(charTexture, marioRect);
        charSprite.setScale(3.764705882352941, 4.f);
        charSprite.setPosition(40.0f, 300.0f);
        canJump = false;
        rightTurn = true;
        leftTurn = false;
        size = "small";
        animSwitch = false;
        frame = 1.8f;
        originalColor = charSprite.getColor();
        characterSound.setVolume(90);
        i = 0;
    }
    void setMarioRect(float newLeft)
    {

        marioRect.left += newLeft;
    }
    int getRect()
    {
        return marioRect.left;
    }
    IntRect getMarioRect()
    {
        return marioRect;
    }
    void setInvincibility(bool newIn)
    {
        invincibility = newIn;
    }
    bool getInvincibility()
    {
        return invincibility;
    }
    string getSize()
    {
        return size;
    }
    void restartSizeClock()
    {
        sizeClock.restart();
        animSwitch = true;
    }
    void setSize(string newSize)
    {
        size = newSize;
    }
    void drawMario(RenderWindow &window)
    {
        window.draw(charSprite);
    }
    void setCanJump(bool jump)
    {
        canJump = jump;
    }
    void move(float dirX)
    {
        velocity.x += dirX * acceleration;
        if (abs(velocity.x) > velocityMax)
        {
            velocity.x = velocityMax * ((velocity.x < 0.f) ? -1.f : 1.f);
        }
    }
    void updateMovement()
    {
        if (currentState != "DEATH")
        {
            if (size == "BIG" && animSwitch)
            {
                if (sizeClock.getElapsedTime().asMilliseconds() <= 400.0f)
                {
                    charSprite.setScale(charSprite.getScale().x - frame, charSprite.getScale().x - frame);
                    frame = -frame;
                    sleep((milliseconds(100)));
                }
                else
                {
                    frame = 1.8f;
                    animSwitch = false;
                    charSprite.setScale(3.764705882352941, 3.5f);
                }
            }
            else if (size == "small" && animSwitch)
            {

                if (sizeClock.getElapsedTime().asMilliseconds() < 3500.0f)
                {
                    Color newColor = originalColor;
                    if (i == 0)
                    {
                        newColor.a = 100;
                        charSprite.setColor(newColor);
                        i = 1;
                    }
                    else if (i == 1)
                    {
                        charSprite.setColor(newColor);
                        i = 0;
                    }
                }
                else
                {
                    i = 0;
                    charSprite.setColor(originalColor);
                    invincibility = false;
                }
            }
            currentState = "IDLE";

            if (Keyboard::isKeyPressed(Keyboard::Up) && canJump)
            {
                currentState = "JUMP";
                velocity.y = -105.f;
                canJump = false;
                if (size == "small")
                {
                    characterBuffer.loadFromFile("Resources/marioAudio/JUMPSMALL.wav");
                }
                else
                {
                    characterBuffer.loadFromFile("Resources/marioAudio/JUMPBIG.wav");
                }
                characterSound.setBuffer(characterBuffer);
                characterSound.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                if (canJump)
                {
                    currentState = "MOVING_RIGHT";
                }
                rightTurn = true;
                leftTurn = false;
                move(1.f);
            }
            else if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                if (canJump)
                {
                    currentState = "MOVING_LEFT";
                }
                rightTurn = false;
                leftTurn = true;
                move(-1.f);
            }
        }
    }
    void updateAnimation()
    {
        float speedPercent = (abs(velocity.x) / velocityMax);
        if (currentState == "IDLE")
        {
            if (rightTurn)
            {
                if (canJump)
                {
                    marioRect.left = 209; // Rect Value of Mario Right Idle
                }
                else
                {
                    marioRect.left = 358; // Rect Value of Mario Left Jump
                }
            }
            else if (leftTurn)
            {
                if (canJump)
                {
                    marioRect.left = 179; // Rect Value of Mario Left Idle
                }
                else
                {
                    marioRect.left = 29; // Rect Value of Mario Right Jump
                }
            }
            charSprite.setTextureRect(marioRect);
            animationTimer.restart();
        }
        else if (currentState == "MOVING_RIGHT")
        {
            if (animationTimer.getElapsedTime().asMilliseconds() >= 60.f / speedPercent)
            {
                marioRect.left += 30;
                if (marioRect.left == 329)
                {
                    marioRect.left = 239;
                }

                animationTimer.restart();
                charSprite.setTextureRect(marioRect);
            }
        }
        else if (currentState == "MOVING_LEFT")
        {
            if (animationTimer.getElapsedTime().asMilliseconds() >= 60.f / speedPercent)
            {
                marioRect.left -= 30;
                if (marioRect.left == 59)
                {
                    marioRect.left = 149;
                }

                animationTimer.restart();
                charSprite.setTextureRect(marioRect);
            }
        }
        else if (currentState == "DEATH")
        {
            marioRect = IntRect(0, 14, 17, 16);
            charSprite.setTextureRect(marioRect);
            if (animationTimer.getElapsedTime().asMilliseconds() >= 80.f / speedPercent)
            {

                this->animationTimer.restart();
                velocity.y += 4.f;
            }
        }
    }
    void updatePhysics(Map &map, list<Goomba *> &goombaList, list<Mushroom *> &mushroomList, RenderWindow &window)
    {
        velocity.y += 1.0f * gravity;
        if (abs(velocity.x) > velocityMaxY)
        {
            velocity.y = velocityMaxY * ((velocity.y < 0.0f) ? -1.f : 1.f);
        }
        velocity *= drag; // Air Resistance / Friction

        if (abs(velocity.x) < velocityMin)
        {
            velocity.x = 0.f;
        }
        if (abs(velocity.y) < velocityMin)
        {
            velocity.y = 0.f;
        }
        if (abs(velocity.x) <= 1.f)
        {
            velocity.x = 0.f;
        }
        if (currentState != "DEATH")
        {
            resolveCollision(map, this, mushroomList, window);
            checkGoombaCollision(this, goombaList);
            checkMushroomCollision(*this, mushroomList);
        }
        charSprite.move(velocity);
    }
    void update(Map &map, list<Goomba *> &goombaList, list<Mushroom *> &mushroomList, RenderWindow &window)
    {
        updateMovement();
        updatePhysics(map, goombaList, mushroomList, window);
        updateAnimation();
    }

    void updateRect(int top, int height)
    {
        marioRect.top = top;
        marioRect.height = height;
        charSprite.setTextureRect(marioRect);
    }
    void setFinishState(int newLeft, int newTop)
    {
        marioRect.left = newLeft;
        marioRect.top = newTop;
        charSprite.setTextureRect(marioRect);
    }
};

class Mushroom : public Character
{
private:
    float velocityMaxY;
    IntRect mushroomRect;

public:
    bool mushroomFirstActive;
    Mushroom()
    {
        mushroomFirstActive = false;
        currentState = "NOTACTIVE";
        if (!charTexture.loadFromFile("Resources/Tilesheet.png"))
        {
            exit(1);
        }
        velocityMaxY = 20.f;
        gravity = 2.f;
        velocity = Vector2f(3.0f, 0.0f);
        mushroomRect = IntRect(825, 264, 32, 32);
        charSprite = Sprite(charTexture, mushroomRect);
        charSprite.setScale(2.0f, 2.0f);
    }
    void update(Map &map, list<Goomba *> &goombaList, list<Mushroom *> &mushroomList, RenderWindow &window)
    {
        updateMovement(map, mushroomList, window);
    }
    void updateMovement(Map &map, list<Mushroom *> &mushroomList, RenderWindow &window)
    {
        if (mushroomFirstActive)
        {
            if (animationTimer.getElapsedTime().asMilliseconds() <= 600.0f)
            {
                charSprite.move(0.0f, -1.80f);
            }
            else
            {
                mushroomFirstActive = false;
            }
        }
        if (currentState == "ACTIVE" && !mushroomFirstActive)
        {

            velocity.y += 1.0 * gravity;
            if (abs(velocity.x) > velocityMaxY)
            {
                velocity.y = velocityMaxY * ((velocity.y < 0.f) ? -1.f : 1.f);
            }
            resolveCollision(map, this, mushroomList, window);
            charSprite.move(velocity);
        }
    }
    void drawMushroom(RenderWindow &window, Mario &mario)
    {
        if (currentState == "ACTIVE")
        {
            if (mario.getSize() == "BIG")
            {
                mushroomRect.top = 330;
                charSprite.setTextureRect(mushroomRect);
                velocity.x = 0.0f;
            }

            window.draw(charSprite);
        }
    }
};
class Goomba : public Character
{
private:
    IntRect goombaRect;
    float velocityMaxY;

public:
    Goomba()
    {
        if (!charTexture.loadFromFile("Resources/enemies_sprites.png"))
        {
            exit(1);
        }
        velocityMaxY = 20.f;
        gravity = 2.f;
        velocity = Vector2f(-3.5f, 0.0f);
        goombaRect = IntRect(0, 4, 16, 16);
        charSprite = Sprite(charTexture, goombaRect);
        charSprite.setScale(4.0f, 4.0f);
    }
    void drawGoomba(RenderWindow &window)
    {
        window.draw(charSprite);
    }
    void update(Map &map, list<Goomba *> &goombaList, list<Mushroom *> &mushroomList, RenderWindow &window)
    {
        updateMovement(map, goombaList, mushroomList, window);
    }
    void updateMovement(Map &map, list<Goomba *> &goombaList, list<Mushroom *> &mushroomList, RenderWindow &window)
    {

        if (currentState != "DEATH")
        {

            velocity.y += 1.0 * gravity;
            if (abs(velocity.x) > velocityMaxY)
            {
                velocity.y = velocityMaxY * ((velocity.y < 0.f) ? -1.f : 1.f);
            }
            resolveCollision(map, this, mushroomList, window);

            if (animationTimer.getElapsedTime().asMilliseconds() >= 90.f)
            {
                goombaRect.left += 30;
                if (goombaRect.left == 60)
                {
                    goombaRect.left = 0;
                }
                animationTimer.restart();
                charSprite.setTextureRect(goombaRect);
            }
            checkGoombaCollision(this, goombaList);

            charSprite.move(velocity);
        }
        else
        {
            if (animationTimer.getElapsedTime().asMilliseconds() >= 250.f)
            {
                charSprite = Sprite();
            }
            else
            {
                goombaRect = IntRect(60, 8, 16, 8);
                charSprite.setTextureRect(goombaRect);
            }
        }
    }
};

unordered_map<string, Object *> *getCollisionList(Map &map, RectangleShape &rect)
{
    unordered_map<string, Object *> *collisionList =
        new unordered_map<string, Object *>;
    bool setFlag = false;
    for (auto &pair : map.collisionObj)
    {
        for (auto &obj : pair.second)
        {
            if (obj->objSprite.getGlobalBounds().intersects(rect.getGlobalBounds()))
            {
                if (pair.first == "Flag" && !setFlag)
                {
                    auto it = pair.second.begin();
                    collisionList->insert(make_pair(pair.first, (*it)));
                    setFlag = true;
                }
                collisionList->insert(make_pair(pair.first, obj));
            }
        }
    }
    return collisionList;
}

void resolveCollision(Map &map, Character *character, list<Mushroom *> &mushroomList, RenderWindow &window)
{
    if (Mario *mario = dynamic_cast<Mario *>(character))
    {
        RectangleShape rect1(Vector2f(mario->getSprite().getGlobalBounds().width, mario->getSprite().getGlobalBounds().height)),
            rect2(Vector2f(mario->getSprite().getGlobalBounds().width / 2, mario->getSprite().getGlobalBounds().height));

        // Checking and Resolving X
        rect1.setPosition(mario->getSprite().getPosition().x + mario->getVelocity().x, mario->getSprite().getPosition().y);
        unordered_map<string, Object *> *list = getCollisionList(map, rect1);
        if (!list->empty())
        {
            for (auto &pair : *list)
            {

                if (pair.first == "Flag")
                {

                    if (mario->getSize() == "small")
                    {
                        mario->setFinishState(327, 29);
                    }
                    else if (mario->getSize() == "BIG")
                    {
                        mario->setFinishState(360, 84);
                    }
                    mario->getSprite().setPosition(mario->getSprite().getPosition().x + (mario->getSprite().getGlobalBounds().width), mario->getSprite().getPosition().y);
                    mario->setVelocity(Vector2f(0.0f, 0.0f));
                    if (pair.second->objSprite.getPosition().x == 13728)
                    {
                        while (pair.second->objSprite.getPosition().y <= 640.0f || mario->getSprite().getPosition().y <= 640.0f)
                        {

                            window.clear(Color(92, 148, 252));
                            if (pair.second->objSprite.getPosition().y <= 640)
                            {
                                pair.second->objSprite.move(0, 4.0f);
                            }
                            map.drawMapBackground(window);
                            map.drawMapCollisionOBJ(window, true);
                            if (mario->getSprite().getPosition().y <= 640.0f)
                            {
                                mario->getSprite().move(0.0f, 3.0f);
                            }
                            mario->drawMario(window);
                            window.display();
                        }
                        SoundBuffer endBuffer;
                        endBuffer.loadFromFile("Resources/marioAudio/smb_stage_clear.wav");
                        Sound endSound(endBuffer);
                        gameSound.stop();
                        endSound.play();
                        if (mario->getSize() == "small")
                        {
                            mario->setFinishState(209, 0);
                            mario->getSprite().setPosition(13888.0f, 704.0f);
                        }
                        else if (mario->getSize() == "BIG")
                        {
                            mario->setFinishState(209, 0);
                            mario->updateRect(52, 32);
                            mario->setScale(Vector2f(3.764705882352941, 3.5f));
                            mario->getSprite().setPosition(13888.0f, 656.0f);
                        }

                        window.clear(Color(92, 148, 252));
                        map.drawMapBackground(window);
                        map.drawMapCollisionOBJ(window, true);
                        mario->drawMario(window);
                        window.display();
                        Clock newClock;
                        newClock.restart();

                        while (mario->getSprite().getPosition().x <= 14144.0f)
                        {
                            window.clear(Color(92, 148, 252));
                            map.drawMapBackground(window);
                            map.drawMapCollisionOBJ(window, true);

                            mario->getSprite().move(2.5f, 0.0f);
                            if (newClock.getElapsedTime().asMilliseconds() >= 90.0f)
                            {
                                mario->marioRect.left += 30;
                                if (mario->marioRect.left == 329)
                                {
                                    mario->marioRect.left = 239;
                                }
                                newClock.restart();
                                mario->getSprite().setTextureRect(mario->marioRect);
                            }

                            mario->drawMario(window);
                            window.display();
                        }
                        mario->getSprite().setColor(Color::Black);
                        gameState = "ENDED";
                    }
                }
            }

            mario->setState("IDLE");
            mario->setVelocity(Vector2f(0.0f, mario->getVelocity().y));
        }
        // Checking and Resolving Y
        if (mario->getVelocity().y > 0) // Mario Jumps & Hits Bottom of Object
        {
            rect1.setPosition(mario->getSprite().getPosition().x, mario->getSprite().getPosition().y + mario->getVelocity().y);
            list = getCollisionList(map, rect1);
        }
        else if (mario->getVelocity().y < 0)
        {
            rect2.setPosition(mario->getSprite().getPosition().x + (mario->getSprite().getGlobalBounds().width / 4), mario->getSprite().getPosition().y + mario->getVelocity().y);
            list = getCollisionList(map, rect2);
        }

        if (!list->empty())
        {
            if (mario->getVelocity().y < 0) // Mario Jumps & Hits Bottom of Object
            {
                for (auto &pair : *list)
                {

                    pair.second->hit = true;
                    if (pair.first == "Bricks")
                    {
                        pair.second->marioSize = mario->getSize();
                        pair.second->originalPos = pair.second->objSprite.getPosition();
                        pair.second->brickClock.restart();
                    }
                    if (pair.first == "CoinBlocks" || pair.first == "InvisibleBlocks")
                    {
                        bool flag = false;
                        for (auto &mushroom : mushroomList)
                        {
                            if ((pair.second->objSprite.getPosition() == mushroom->getSprite().getPosition()) && mushroom->getState() != "ACTIVE")
                            {
                                pair.second->coinSprite = Sprite();
                                mushroom->setState("ACTIVE");
                                mushroom->mushroomFirstActive = true;
                                mushroom->resetAnimationTimer();
                                flag = true;
                            }
                        }
                        if (!flag && !pair.second->coinComplete)
                        {

                            pair.second->coin = true;
                            pair.second->coinClock.restart();
                            pair.second->coinComplete = true;
                        }
                    }

                    // Setting Marios Position
                    mario->getSprite().setPosition(mario->getSprite().getPosition().x, pair.second->objSprite.getPosition().y + pair.second->objSprite.getGlobalBounds().height);
                    mario->setVelocity(Vector2f(mario->getVelocity().x, 0.0f));
                }
            }
            else if (mario->getVelocity().y > 0) // Mario is Falling & Hits Top of Object
            {
                auto it = list->begin();
                mario->getSprite().setPosition(mario->getSprite().getPosition().x, it->second->objSprite.getPosition().y - mario->getSprite().getGlobalBounds().height);
                mario->setVelocity(Vector2f(mario->getVelocity().x, 0.0f));
                mario->setCanJump(true);
            }
        }
        else
        {
            mario->setCanJump(false);
        }
    }
    else if (Goomba *goomba = dynamic_cast<Goomba *>(character))
    {
        RectangleShape rect1(Vector2f(goomba->getSprite().getGlobalBounds().width, goomba->getSprite().getGlobalBounds().height)),
            rect2(Vector2f(goomba->getSprite().getGlobalBounds().width, goomba->getSprite().getGlobalBounds().height));

        // Checking and Resolving X
        rect1.setPosition(goomba->getSprite().getPosition().x + goomba->getVelocity().x, goomba->getSprite().getPosition().y);
        unordered_map<string, Object *> *list = getCollisionList(map, rect1);
        if (!list->empty())
        {
            goomba->setVelocity(Vector2f(-goomba->getVelocity().x, goomba->getVelocity().y));
        }
        // Checking and Resolving Y
        rect2.setPosition(goomba->getSprite().getPosition().x, goomba->getSprite().getPosition().y + goomba->getVelocity().y);
        list = getCollisionList(map, rect2);

        if (!list->empty())
        {
            if (goomba->getVelocity().y > 0)
            {
                auto it = list->begin();
                goomba->getSprite().setPosition(goomba->getSprite().getPosition().x, it->second->objSprite.getPosition().y - goomba->getSprite().getGlobalBounds().height);
                goomba->setVelocity(Vector2f(goomba->getVelocity().x, 0.0f));
            }
        }
    }

    else if (Mushroom *mushroom = dynamic_cast<Mushroom *>(character))
    {
        RectangleShape rect1(Vector2f(mushroom->getSprite().getGlobalBounds().width, mushroom->getSprite().getGlobalBounds().height)),
            rect2(Vector2f(mushroom->getSprite().getGlobalBounds().width, mushroom->getSprite().getGlobalBounds().height));

        // Checking and Resolving X
        rect1.setPosition(mushroom->getSprite().getPosition().x + mushroom->getVelocity().x, mushroom->getSprite().getPosition().y);
        unordered_map<string, Object *> *list = getCollisionList(map, rect1);
        if (!list->empty())
        {
            mushroom->setVelocity(Vector2f(-mushroom->getVelocity().x, mushroom->getVelocity().y));
        }
        // Checking and Resolving Y
        rect2.setPosition(mushroom->getSprite().getPosition().x, mushroom->getSprite().getPosition().y + mushroom->getVelocity().y);
        list = getCollisionList(map, rect2);

        if (!list->empty())
        {

            if (mushroom->getVelocity().y > 0)
            {
                auto it = list->begin();
                mushroom->getSprite().setPosition(mushroom->getSprite().getPosition().x, it->second->objSprite.getPosition().y - mushroom->getSprite().getGlobalBounds().height);
                mushroom->setVelocity(Vector2f(mushroom->getVelocity().x, 0.0f));
            }
        }
    }
}
void checkGoombaCollision(Character *character, list<Goomba *> &goombaList)
{
    if (Goomba *goomba = dynamic_cast<Goomba *>(character))
    {
        for (auto &pair : goombaList)
        {
            if (goomba->getSprite().getGlobalBounds().intersects(pair->getSprite().getGlobalBounds()) && (goomba->getSprite().getPosition().x != pair->getSprite().getPosition().x))
            {
                pair->setVelocity(Vector2f(goomba->getVelocity().x, pair->getVelocity().y));
            }
        }
    }
    else if (Mario *mario = dynamic_cast<Mario *>(character))
    {
        for (auto &pair : goombaList)
        {
            RectangleShape rect1(Vector2f(mario->getSprite().getGlobalBounds().width, mario->getSprite().getGlobalBounds().height)),
                rect2(Vector2f(mario->getSprite().getGlobalBounds().width, mario->getSprite().getGlobalBounds().height));

            rect1.setPosition(mario->getSprite().getPosition().x + mario->getVelocity().x, mario->getSprite().getPosition().y);
            rect2.setPosition(mario->getSprite().getPosition().x, mario->getSprite().getPosition().y + mario->getVelocity().y);
            if (!mario->getInvincibility())
            {
                if (rect1.getGlobalBounds().intersects(pair->getSprite().getGlobalBounds()))
                {
                    if (mario->getSize() == "small")
                    {
                        mario->setState("DEATH");
                        mario->setVelocity(Vector2f(0.0f, -80.f));
                    }
                    else if (mario->getSize() == "BIG")
                    {
                        mario->setSize("small");
                        mario->updateRect(0, 16);

                        mario->setInvincibility(true);
                        mario->restartSizeClock();
                    }
                }

                else if (rect2.getGlobalBounds().intersects(pair->getSprite().getGlobalBounds()))
                {
                    mario->setVelocity(Vector2f(mario->getVelocity().x, mario->getVelocity().y - 50.0f));
                    pair->setState("DEATH");
                    pair->getSprite().setPosition(pair->getSprite().getPosition().x, pair->getSprite().getPosition().y + 32.0f);
                    pair->resetAnimationTimer();
                }
            }
        }
    }
}
void checkMushroomCollision(Mario &mario, list<Mushroom *> &mushroomList)
{
    for (auto &pair : mushroomList)
    {
        if (mario.getSprite().getGlobalBounds().intersects(pair->getSprite().getGlobalBounds()) && pair->getState() == "ACTIVE")
        {
            if (mario.getSize() == "small")
            {
                mario.updateRect(52, 32);
                mario.setSize("BIG");
                mario.restartSizeClock();
            }
            pair->getSprite() = Sprite();
        }
    }
}
class MarioGame
{
private:
    RenderWindow window;
    Map map;
    Mario mario;
    list<Goomba *> goomba;
    list<Mushroom *> mushroom;
    View view;
    unsigned view_x;

public:
    MarioGame()
    {
        view.setSize(1600.0f, 896.0f);
        view.setCenter(1600.0f / 2, 896.0f / 2);
        view_x = 0;
        window.create(VideoMode(1600, 896), "Super Mario Bros");
        window.setPosition(Vector2i(VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2, VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));
        window.setFramerateLimit(60);
        window.setView(view);
        map.loadMap();

        for (int i = 0; i < pos.size(); i++)
        {
            Goomba *newGoomba = new Goomba();
            newGoomba->getSprite().setPosition(pos[i]);
            goomba.push_back(newGoomba);
        }
        for (int i = 0; i < mush.size(); i++)
        {
            Mushroom *newMushroom = new Mushroom();
            newMushroom->getSprite().setPosition(mush[i]);
            mushroom.push_back(newMushroom);
        }
    }
    bool mainMenu()
    {
        while (true)
        {
            Texture mainMenu;
            mainMenu.loadFromFile("Resources/Title.png");
            Sprite menu(mainMenu);
            menu.setScale(6.299212598425197,3.912663755458515);
            window.draw(menu);
            window.display();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::Return))
            {
                return false;
            }
            
            
        }
    }
    void endingText()
    {
        gameSound.stop();
        window.clear(Color::Black);
        window.display();
        sleep(seconds(1));
        window.close();
        cout << "GAME OVER";
    }
    void update()
    {
        if (mario.getSprite().getPosition().y >= 896.0f)
        {
            mario.setState("DEATH");
            gameState = "ENDED";
        }
        if (gameState != "ENDED")
        {
            mario.update(map, goomba, mushroom, window);
        }
        if (gameState != "ENDED")
        {
            for (auto &pair : goomba)
            {
                if ((pair->getSprite().getPosition().x - mario.getSprite().getPosition().x) < 960)
                    pair->update(map, goomba, mushroom, window);
            }
            for (auto &pair : mushroom)
            {
                if ((pair->getSprite().getPosition().x - mario.getSprite().getPosition().x) < 960)
                    pair->update(map, goomba, mushroom, window);
            }
        }
    }
    void handleGame()
    {

        while (window.isOpen())
        {

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                    window.close();

                if (
                    event.type == Event::KeyReleased &&
                    (event.key.code == Keyboard::Left ||
                     event.key.code == Keyboard::Right ||
                     event.key.code == Keyboard::Up))
                {
                    mario.resetAnimationTimer();
                }
            }
            update();
            window.clear(Color(92, 148, 252));
            if (mario.getSprite().getPosition().x >= 1600 / 2)
            {
                view_x = mario.getSprite().getPosition().x - 1600 / 2;
            }

            view.reset(FloatRect(view_x, 0, 1600, 896));
            window.setView(view);
            if (gameState == "ENDED")
            {
                endingText();
                window.close();
                sleep(seconds(2));
                exit(1);
            }
            map.drawMapBackground(window);
            for (auto &pair : goomba)
            {
                pair->drawGoomba(window);
            }
            for (auto &pair : mushroom)
            {
                pair->drawMushroom(window, mario);
            }
            map.drawMapCollisionOBJ(window, false);
            mario.drawMario(window);
            window.display();
        }
    }
};
int main()
{
    MarioGame marioGame;
    if (!marioGame.mainMenu())
    {
        if (!gameBuffer.loadFromFile("Resources/marioAudio/1-1.wav"))
        {
            exit(1);
        }
        gameSound = Sound(gameBuffer);
        gameSound.setVolume(100);
        gameSound.setLoop(true);

        gameSound.play();

        marioGame.handleGame();
    }
}