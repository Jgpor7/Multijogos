#include "raylib/raylib.h"
#include <iostream>
#include <cmath>
#include <random>

#define LARGURATELA 854
#define ALTURATELA 480

#define RAIOSPAWNZUMBI 250

int calcularRestoPositivo(int a, int b){
    return ((a % b) + b) % b;
}

int randomInt(int a, int b);

class Zumbi{

    public:

    Zumbi(float velocidadeIn, int vidaIn, Vector2 posicaoJogador){
        velocidade = velocidadeIn;
        vida = vidaIn;

        // gerando nascimento aleatorio
        bool lugar_valido = false;
        while(!lugar_valido){ // gerando o zumbi em um raio distante do jogador
            posicao.x = randomInt(0, LARGURATELA);
            posicao.y = randomInt(0, ALTURATELA);

            int posicao_x_relativa = (int)(posicao.x - posicaoJogador.x);
            int posicao_y_relativa = (int)(posicao.y - posicaoJogador.y);

            if(sqrt(pow(posicao_x_relativa, 2) + pow(posicao_y_relativa, 2)) > RAIOSPAWNZUMBI) lugar_valido = true;
        }

        retanguloZumbi = {posicao.x, posicao.y, 20, 30};
    }

    void desenharZumbi(){
        DrawRectangleRec(retanguloZumbi, RED);
    }

    Vector2 retornarPosicao(){
        return posicao;
    }

    void seguirJogador(Vector2 posicaoJogador){
        Vector2 vetorVisado = {posicaoJogador.x - posicao.x, posicaoJogador.y - posicao.y};
        float moduloVetor = sqrt(pow(vetorVisado.x, 2) + pow(vetorVisado.y, 2));
        vetorVisado = {(velocidade * vetorVisado.x)/moduloVetor, (velocidade * vetorVisado.y)/moduloVetor};
        posicao.x = posicao.x + vetorVisado.x;
        posicao.y = posicao.y + vetorVisado.y;
        retanguloZumbi = {posicao.x, posicao.y, 20, 30};
    }

    private:
        float velocidade;
        int vida;
        Rectangle retanguloZumbi;
        Vector2 posicao;
};

class jogador{

    public:

    jogador(){
        velocidade = 5;
        vida = 5;
        balas = 20;
        posicao = {LARGURATELA/2, ALTURATELA/2};
        retanguloJogador = {posicao.x, posicao.y, 20, 30};
    }

    Vector2 retornarPosicao(){
        return posicao;
    }

    void desenharJogador(){
        DrawRectangleRec(retanguloJogador, GREEN);
    }

    void moverJogador(){

        float velocidadeDiagonal = (velocidade * sqrt(2))/2;
        
        if(IsKeyDown(KEY_W)){
            if(IsKeyDown(KEY_D)){
                posicao.x += velocidadeDiagonal;
                posicao.y -= velocidadeDiagonal;
            }
            else if(IsKeyDown(KEY_A)){
                posicao.x -= velocidadeDiagonal;
                posicao.y -= velocidadeDiagonal;
            }
            else {posicao.y -= velocidade;}
        } 
        if(IsKeyDown(KEY_S)){
            if(IsKeyDown(KEY_D)){
                posicao.x += velocidadeDiagonal;
                posicao.y += velocidadeDiagonal;
            }
            else if(IsKeyDown(KEY_A)){
                posicao.x -= velocidadeDiagonal;
                posicao.y += velocidadeDiagonal;
            }
            else {posicao.y += velocidade;}
        }
        if(IsKeyDown(KEY_D)) posicao.x += velocidade;
        if(IsKeyDown(KEY_A)) posicao.x -= velocidade;   
    }

    void atualizarPosicao(){
        posicao.x = calcularRestoPositivo((int)posicao.x, LARGURATELA);
        posicao.y = calcularRestoPositivo((int)posicao.y, ALTURATELA);
        retanguloJogador.x = posicao.x;
        retanguloJogador.y = posicao.y;
    }

    private:
        float velocidade;
        int vida;
        int balas;
        Rectangle retanguloJogador;
        Vector2 posicao;
};

int main(){

    // --- 1. Inicialização ---
    const int larguraTela = LARGURATELA;
    const int alturaTela = ALTURATELA;

    // Abre a janela e o contexto da GPU
    InitWindow(larguraTela, alturaTela, "Multijogos");

    // Define a meta de frames por segundo (60fps é o padrão)
    SetTargetFPS(60);

    jogador player = jogador();

    int tempoNascimentoZumbi = 10;

    std::vector<Zumbi> listaZumbis;

    // --- 2. Game Loop Principal ---

    int contadorFrames = 0;

    while (!WindowShouldClose()) {    

        if(calcularRestoPositivo(contadorFrames, 60*tempoNascimentoZumbi) == 0){
            Zumbi zumbi = Zumbi(5, 20, player.retornarPosicao());
            listaZumbis.push_back(zumbi);
        }
        
        BeginDrawing();

            ClearBackground(BLACK);

            
            for(int i = 0; i < listaZumbis.size(); i++) {
                listaZumbis[i].seguirJogador(player.retornarPosicao());
                listaZumbis[i].desenharZumbi();
            }

            DrawText(TextFormat("x: %.2f\ny: %.2f", player.retornarPosicao().x, player.retornarPosicao().y), 700, 50, 20, WHITE);

            player.desenharJogador();
    
        EndDrawing();

        player.moverJogador();
        player.atualizarPosicao();

        contadorFrames++;
    }

    // --- 5. Finalização ---
    CloseWindow();        // Fecha a janela e libera a memória
    return 0;
}

int randomInt(int a, int b){
    // setando a seed do random
    std::random_device rd;
    std::mt19937 gen(rd());
    // ----------------------

    std::uniform_int_distribution<> distr(a, b);
    return distr(gen);
}