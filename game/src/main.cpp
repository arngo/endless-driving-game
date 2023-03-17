#include "raylib.h"
#include <iostream>

class Player {
    public:
        int health;
        Rectangle hitbox;

        Player(Rectangle hitbox, int health) : 
            hitbox(hitbox),
            health(health)
        {}
};

class Obstacle {
    public:
        Rectangle hitbox;
        Obstacle(Rectangle hitbox) : 
            hitbox(hitbox)
        {}
};

int main ()
{
	// set up the window
	InitWindow(1280, 800, "Spaceship game");
    Player player(Rectangle{100.0f, 100.0f, 20.0f, 20.0f}, 10);

    Obstacle o(Rectangle{300.0f, 200.0f, 30.0f, 30.0f});

    SetTargetFPS(60); 
	
	// game loop
	while (!WindowShouldClose())
	{
        if (IsKeyDown(KEY_RIGHT)) player.hitbox.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) player.hitbox.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) player.hitbox.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) player.hitbox.y += 2.0f;
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);
        std::cout << player.hitbox.x << " " << player.hitbox.y << "\n";
        DrawRectangleRec(player.hitbox, RAYWHITE);
        DrawRectangleRec(o.hitbox, RED);
		//DrawText("Hello Raylib", 200,200,20,WHITE);
        if (player.health <= 0) {
            std::cout << "player dead\n";
        } else if (CheckCollisionRecs(player.hitbox, o.hitbox)) {
            player.health -= 1;
            std::cout << "health: " << player.health << "\n";
        }
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}
