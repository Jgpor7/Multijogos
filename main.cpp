#include "raylib/raylib.h"
#include <iostream>
#include <cmath>
#include <random>

//#define LARGURATELA 854
//#define ALTURATELA 480

#define LARGURATELA (float)GetScreenWidth()
#define ALTURATELA (float)GetScreenHeight()

#define RAIOSPAWNZUMBI ALTURATELA/3

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

        retanguloZumbi = {posicao.x, posicao.y, ALTURATELA/20, LARGURATELA/17};
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
        retanguloZumbi.x = posicao.x;
        retanguloZumbi.y = posicao.y;
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
        retanguloJogador = {posicao.x, posicao.y, ALTURATELA/20, LARGURATELA/17};
    }

    Vector2 retornarPosicao(){
        return posicao;
    }

    void desenharJogador(){
        DrawRectangleRec(retanguloJogador, GREEN);
    }

    void moverJogador(){

        Vector2 vetorDirecao = {0,0};
        
        if(IsKeyDown(KEY_W)) vetorDirecao.y -= 1;  
        if(IsKeyDown(KEY_S)) vetorDirecao.y += 1;
        if(IsKeyDown(KEY_D)) vetorDirecao.x += 1;
        if(IsKeyDown(KEY_A)) vetorDirecao.x -= 1;

        if(vetorDirecao.x != 0 || vetorDirecao.y != 0){
            float moduloVetor = sqrt(pow(vetorDirecao.x, 2) + pow(vetorDirecao.y, 2));
            vetorDirecao = {velocidade*(vetorDirecao.x/moduloVetor), velocidade*(vetorDirecao.y/moduloVetor)};
        }

        posicao = {posicao.x + vetorDirecao.x, posicao.y + vetorDirecao.y};
    }

    void atualizarPosicao(){
        posicao.x = calcularRestoPositivo((int)posicao.x, LARGURATELA);
        posicao.y = calcularRestoPositivo((int)posicao.y, ALTURATELA);
        retanguloJogador.x = posicao.x;
        retanguloJogador.y = posicao.y;
    }

    void gerarMira(){

        Texture2D textura = LoadTexture("resources/setavermelha.png");

        Vector2 posicaoMouse = GetMousePosition();

        float tangente = (posicaoMouse.y - posicao.y)/(posicaoMouse.x - posicaoMouse.x);


        Vector2 centro = { (float)textura.width / 2.0f, (float)textura.height / 2.0f };

            // RETÂNGULO DE ORIGEM: Pega a imagem inteira (do pixel 0,0 até a largura/altura total)
            Rectangle sourceRec = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };

            // RETÂNGULO DE DESTINO: Define onde na tela e qual o tamanho final
            // Nota: x e y aqui serão onde o CENTRO da imagem vai ficar posicionado.
            Rectangle destRec = { posicao.x, posicao.y, (float)textura.width, (float)textura.height };

            // Desenha a imagem completa rotacionando no centro
            DrawTexturePro(textura, sourceRec, destRec, centro, 180, WHITE);

        UnloadTexture(textura);
        
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
            Zumbi zumbi = Zumbi(2, 20, player.retornarPosicao());
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
            player.gerarMira();
    
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