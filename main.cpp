#include "raylib/raylib.h"
#include <iostream>

int main(){

    // --- 1. Inicialização ---
    const int larguraTela = 854;
    const int alturaTela = 480;

    Vector2 pontomouse;

    Vector2 coordenadasbotaoJogo1 = {34, 282};
    Rectangle botaoJogo1 = { 34, 282, 140, 50 };

    // Abre a janela e o contexto da GPU
    InitWindow(larguraTela, alturaTela, "Multijogos");

    // Define a meta de frames por segundo (60fps é o padrão)
    SetTargetFPS(60);

    // --- 2. Game Loop Principal ---
    while (!WindowShouldClose()) {    

        pontomouse = GetMousePosition();

        BeginDrawing();

            ClearBackground(RAYWHITE); // Limpa a tela com branco

            DrawText("Multijogos", 34, 25, 40, BLUE);

            DrawRectangleRec(botaoJogo1, GREEN);
            DrawText("Jogo 1", coordenadasbotaoJogo1.x + 5, coordenadasbotaoJogo1.y +5, 40, BLACK);
            DrawText(TextFormat("x=%.2f\ny=%.2f", pontomouse.x, pontomouse.y), 750, 20, 20, BLACK);

            if(CheckCollisionPointRec(pontomouse, botaoJogo1) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                // ao pressionar o botão
            }

        EndDrawing();
    }

    // --- 5. Finalização ---
    CloseWindow();        // Fecha a janela e libera a memória
    return 0;
}