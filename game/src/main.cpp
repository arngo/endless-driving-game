#include "raylib.h"
#include <iostream>
#include <vector>
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class Player {
    public:
        int health;
        bool isDead = false;
        Rectangle hitbox;
        float speed = 400.0f;
        float damageCooldown = 1.0f;
        float damageTimer = damageCooldown;
        float fuel = 100.0f;
        float fuelConsumption = 1.0f;

        Player(Rectangle hitbox, int health) : 
            hitbox(hitbox),
            health(health)
        {}
};

class WorldObject {
    public:
        Rectangle hitbox;
        Rectangle textureSize;
        Texture2D texture;
        WorldObject(Rectangle hitbox, std::string textureFile, Rectangle textureSize) : 
            hitbox(hitbox),
            texture(LoadTexture(textureFile.c_str())),
            textureSize(textureSize)

        {}
        virtual void action(Player& player) {
        }
        ~WorldObject() {
            UnloadTexture(texture);
        }
};

class Obstacle: public WorldObject {
    public:
        int damage;
        
        Obstacle(Rectangle hitbox, int damage, std::string textureFile, Rectangle textureSize) : WorldObject{hitbox, textureFile, textureSize},
            damage(damage)
        {}
        void action(Player& player) {
            player.health -= damage;
            player.damageTimer = 0.0f;
        }
        ~Obstacle() {
            UnloadTexture(texture);
        }
};

class Fuel: public WorldObject {
    public:
        float fuel;
        
        Fuel(Rectangle hitbox, float fuel, std::string textureFile, Rectangle textureSize) : WorldObject{hitbox, textureFile, textureSize},
            fuel(fuel)
        {}
        void action(Player& player) {
            player.fuel += fuel;
        }
        ~Fuel() {
            UnloadTexture(texture);
        }
};

Obstacle* spawnObstacle()
{
    float x;
	// Offset = 0
	// Range = 1281
	x = 0 + (rand() % SCREEN_WIDTH + 1);
    return new Obstacle(Rectangle{x, -100.0f, 128.0f, 128.0f}, 1, "./resources/asteroid.png", Rectangle{0,0,64,64});
}

Fuel* spawnFuel()
{
    float x;
	// Offset = 0
	// Range = 1281
	x = 0 + (rand() % SCREEN_WIDTH + 1);
    return new Fuel(Rectangle{x, -100.0f, 86.0f, 86.0f}, 20, "./resources/fuel.png", Rectangle{0,0,43,43});
}

WorldObject* checkObjectCollisions(Player &player, std::vector<WorldObject*> &obstacles) {
  for (auto& o : obstacles) {
    if (CheckCollisionRecs(player.hitbox, o->hitbox)) {
      return o; 
    }
  }
  return NULL;
}

int main ()
{
	// set up the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spaceship game");
    std::vector<WorldObject*> objects;
    float objectTimer = 0;
    float flyingSpeed = 300.0f;

    SetTargetFPS(60); 
    float frameTime;

    std::string deadText = "You died";
    int deadTextWidth = MeasureText(deadText.c_str(), 50);
	
    Player player(Rectangle{SCREEN_WIDTH/2, 600.0f, 20.0f, 20.0f}, 3);
    //Obstacle o(Rectangle{300.0f, 200.0f, 128.0f, 128.0f}, 3, "./resources/asteroid.png");

	// game loop
	while (!WindowShouldClose())
	{
        frameTime = GetFrameTime();

        if (!player.isDead) {
            if (IsKeyDown(KEY_RIGHT)) {
                player.hitbox.x += frameTime * player.speed;
            }
            if (IsKeyDown(KEY_LEFT)) {
                player.hitbox.x -= frameTime * player.speed;
            }
        }

        player.damageTimer += frameTime;
        objectTimer += frameTime;
        if (objectTimer >= 1) {
            float objectProbability = (float)rand() / (float)RAND_MAX;
            std::cout << objectProbability << "\n";
            WorldObject *newObject;
            if (objectProbability <= 0.3) {
                newObject = spawnFuel();
            } else {
                newObject = spawnObstacle();
            }
            objects.push_back(newObject);
            objectTimer = 0;
        }
        if (!player.isDead && player.health <= 0) {
            std::cout << "player dead\n";
            player.isDead = true;
        } else if (!player.isDead && player.damageTimer >= player.damageCooldown) {
            WorldObject* o = checkObjectCollisions(player, objects);
            if (o != NULL) {
                o->action(player);
            }
        }

        //for (auto& o : obstacles) {
        for (int i = 0; i < objects.size(); i++) {
            objects[i]->hitbox.y += frameTime * flyingSpeed;
            if (objects[i]->hitbox.y > SCREEN_HEIGHT + 200) {
                delete objects[i];
                objects.erase(objects.begin() + i);
            }
        }

        if (player.fuel >= 0) {
            player.fuel -= frameTime * player.fuelConsumption;
        } else {
            player.health = 0;
        }

		// drawing
		BeginDrawing();
            ClearBackground(BLACK);
            DrawText(std::to_string(player.fuel).c_str(), 10, 10, 30, WHITE);
            DrawText(std::to_string(player.health).c_str(), 10, 50, 30, WHITE);
            if (player.isDead) {
                DrawText(deadText.c_str(), (SCREEN_WIDTH/2) - (deadTextWidth/2), (SCREEN_HEIGHT/2)-35, 70, RED);
            }
            //std::cout << player.hitbox.x << " " << player.hitbox.y << "\n";
            DrawRectangleRec(player.hitbox, RAYWHITE);
            //DrawRectangleRec(o.hitbox, RED);
            for (auto& o : objects) {
                DrawTexturePro(o->texture, o->textureSize, o->hitbox, Vector2{0,0}, 0.0f, WHITE);
            }
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}
