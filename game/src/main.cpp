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
        float speed = 200.0f;
        float damageCooldown = 1.0f;
        float damageTimer = damageCooldown;

        Player(Rectangle hitbox, int health) : 
            hitbox(hitbox),
            health(health)
        {}
};

class Obstacle {
    public:
        int damage;
        Rectangle hitbox;
        Texture2D texture;
        Obstacle(Rectangle hitbox, int damage, std::string textureFile) : 
            hitbox(hitbox),
            damage(damage),
            texture(LoadTexture(textureFile.c_str()))
        {}
        ~Obstacle() {
            UnloadTexture(texture);
        }
};

Obstacle* spawnObstacle()
{
    float x;
	// Offset = 0
	// Range = 1281
	x = 0 + (rand() % SCREEN_WIDTH + 1);
    return new Obstacle(Rectangle{x, 200.0f, 128.0f, 128.0f}, 3, "./resources/asteroid.png");
}

Obstacle* checkObstacleCollisions(Player &player, std::vector<Obstacle*> &obstacles) {
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
    std::vector<Obstacle*> obstacles;
    float obstacleTimer = 0;

    SetTargetFPS(60); 
    float frameTime;
	
    Player player(Rectangle{SCREEN_WIDTH/2, 600.0f, 20.0f, 20.0f}, 3);
    //Obstacle o(Rectangle{300.0f, 200.0f, 128.0f, 128.0f}, 3, "./resources/asteroid.png");

	// game loop
	while (!WindowShouldClose())
	{
        frameTime = GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) {
            player.hitbox.x += frameTime * player.speed;
        }
        if (IsKeyDown(KEY_LEFT)) {
            player.hitbox.x -= frameTime * player.speed;
        }
        /*if (IsKeyDown(KEY_UP)) {
            player.hitbox.y -= frameTime * player.speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            player.hitbox.y += frameTime * player.speed;
        }*/
        player.damageTimer += frameTime;
        obstacleTimer += frameTime;
        if (obstacleTimer >=5) {
            obstacles.push_back(spawnObstacle());
            obstacleTimer = 0;
        }
        if (!player.isDead && player.health <= 0) {
            std::cout << "player dead\n";
            player.isDead = true;
        } else if (!player.isDead && player.damageTimer >= player.damageCooldown) {
            Obstacle* o = checkObstacleCollisions(player, obstacles);
            if (o != NULL) {
                player.health -= o->damage;
                player.damageTimer = 0.0f;
                std::cout << "health: " << player.health << "\n";
            }
        }



		// drawing
		BeginDrawing();
            ClearBackground(BLACK);
            //std::cout << player.hitbox.x << " " << player.hitbox.y << "\n";
            DrawRectangleRec(player.hitbox, RAYWHITE);
            //DrawRectangleRec(o.hitbox, RED);
            for (auto& o : obstacles) {
                DrawTexturePro(o->texture, Rectangle{0,0,64,64}, o->hitbox, Vector2{0,0}, 0.0f, WHITE);
            }
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}
