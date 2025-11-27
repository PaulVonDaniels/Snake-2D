#include <raylib.h>
#include <raymath.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "deque.h"
int cellSize = 30;
int cellCount = 25;

Color green = {255, 255, 255, 255};
Color darkGreen = {19, 228, 68, 255};

double lastUpdateTime = 0.0;

typedef struct {
    Deque* deque;
    Vector2 direction;
} Snake;

typedef struct {
    Vector2 position;
    Texture2D texture;
    Image image;
} Food;

typedef struct {
    Snake *snake;
    Food *food;
} Game;


Vector2 generateRandomsPos(Game* game);
bool evenTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool elementInDeque(Deque* deque, Vector2 item) {
    for(int i = 0; i != deque->size; i++) {
        if(deque->data[i].x == item.x && deque->data[i].y == item.y)
            return true;
    }
    return false;
}

Vector2 generateRandomCell() {
    double x = GetRandomValue(0, cellCount-1);
    double y = GetRandomValue(0, cellCount-1);
    return (Vector2){x, y};
}
Vector2 generateRandomsPos(Game* game)
{
    Vector2 pos = generateRandomCell();
    while(elementInDeque(game->snake->deque, pos)) {
        pos = generateRandomCell();
    }
    return pos;
}

void 
draw_apple(Food* food)
{
    DrawTexture(food->texture, food->position.x * cellSize, food->position.y * cellSize, green);
}
void 
init_image(Food* food)
{
    
    food->image = LoadImage("../images/apple.png");
    ImageResize(&food->image, 30, 30);
    food->texture = LoadTextureFromImage(food->image);
    UnloadImage(food->image);
    int width = food->texture.width;
    int height = food->texture.height;
}
void 
destroy_image(Texture2D* texture) { UnloadTexture(*texture); }

void draw_snake(Deque* deque)
{
    for(int i = 0; i < deque->size; i++) {
        Vector2 coords = get(deque, i);
        DrawRectangleRounded((Rectangle){coords.x * cellSize, coords.y * cellSize, 
            cellSize, cellSize}, 0.6f, 6, darkGreen);
    }
}
void update_move(Snake* snake)
{
    pop_back(snake->deque);
    push_front(snake->deque, Vector2Add(snake->deque->data[0], snake->direction));
}

bool checkCollisionSnake(Deque* dq) {
    if(dq->size < 4) return false;
    for(int i = 1; i < dq->size; i++) {
        if(dq->data[0].x == dq->data[i].x && dq->data[0].y == dq->data[i].y)
            return true;
    }
    return false;
}


void draw(Game* game)
{
    draw_apple(game->food);
    draw_snake(game->snake);
}
void update_game(Game* game)
{
    update_move(game->snake);
}
void checkCollisionWidthFood(Game* game)
{
    if(Vector2Equals(game->snake->deque->data[0], game->food->position)) {
            printf("%s\n","Eating food");
            game->food->position = generateRandomsPos(game);
            draw_apple(game->food);
            push_back(game->snake->deque, (Vector2){1,0});
    }
}

int 
main(void) 
{
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Game");
    SetTargetFPS(60);

    Game *game = (Game*)calloc(1, sizeof(Game));
    game->snake = (Snake*)calloc(1, sizeof(Snake));
    game->food = (Food*)calloc(1, sizeof(Food));

    game->snake->deque = create_deque();
    push_back(game->snake->deque, (Vector2){6, 9});
    push_back(game->snake->deque, (Vector2){5, 9});
    push_back(game->snake->deque, (Vector2){4, 9});
    game->snake->direction = (Vector2){1, 0};

    printf("Initial snake size: %d\n", game->snake->deque->size);

    game->food->position = generateRandomsPos(game);
    printf("Food position: %.0f, %.0f\n", game->food->position.x, game->food->position.y);

    init_image(game->food);
    
    while(WindowShouldClose() == false) {
        BeginDrawing();

        if(evenTriggered(0.1)) {
            update_move(game->snake);
            printf("Snake head: %.0f, %.0f, Size: %d\n", 
                   game->snake->deque->data[0].x, 
                   game->snake->deque->data[0].y,
                   game->snake->deque->size);
        }

        if(IsKeyPressed(KEY_W) && (game->snake->direction.x != 0 && game->snake->direction.y != 1)) {
            game->snake->direction = (Vector2){0, -1};
            printf("Direction: UP\n");
        }
        else if(IsKeyPressed(KEY_S) && (game->snake->direction.x != 0 && game->snake->direction.y != -1)) {
            game->snake->direction = (Vector2){0, 1};
            printf("Direction: DOWN\n");
        }
        else if(IsKeyPressed(KEY_A) && (game->snake->direction.x != 1 && game->snake->direction.y != 0)) {
            game->snake->direction = (Vector2){-1, 0};
            printf("Direction: LEFT\n");
        }
        else if(IsKeyPressed(KEY_D) && (game->snake->direction.x != -1 && game->snake->direction.y != 0)) {
            game->snake->direction = (Vector2){1, 0};
            printf("Direction: RIGHT\n");
        }
        bool collision = checkCollisionSnake(game->snake->deque);
        if(collision) {
            printf("COLLISION DETECTED! Game over.\n");
            break;
        }
        
        ClearBackground(green);
        checkCollisionWidthFood(game);
        draw_apple(game->food);
        draw_snake(game->snake->deque);

        DrawText(TextFormat("Snake size: %d", game->snake->deque->size), 10, 10, 37, BLACK);
        DrawText(TextFormat("Head: %.0f,%.0f", game->snake->deque->data[0].x, game->snake->deque->data[0].y), 10, 50, 30, BLACK);

        EndDrawing();
    }
    destroy_image(&game->food->texture);
    free(game->snake->deque->data);
    free(game->snake->deque);
    free(game->snake);
    free(game->food);
    free(game);
    CloseWindow();
    return 0;
}
