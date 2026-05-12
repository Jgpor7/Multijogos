#include "raylib/raylib.h"
#include <iostream>
#include <cmath>
#include <random>

#include <unordered_map> // adicionando os dicionarios hash

//#define LARGURATELA 854
//#define ALTURATELA 480

#define LARGURATELA (float)GetScreenWidth()
#define ALTURATELA (float)GetScreenHeight()

#define RAIOSPAWNZUMBI ALTURATELA/3
#define RAIOMIRA 150

#define PI 3.1415

int calcularRestoPositivo(int a, int b){
    return ((a % b) + b) % b;
}

float converterRadianoGraus(float rad){
    return (rad*180)/PI;
}

int randomInt(int a, int b);
float det2x2(Vector2 a, Vector2 b);

Vector2 calcularModulo(Vector2 v){
    float modulo = sqrt(pow(v.x, 2) + pow(v.y, 2));
    return (Vector2){v.x/modulo, v.y/modulo};
}

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
        retanguloJogador = {posicao.x - ALTURATELA/40, posicao.y - ALTURATELA/40, ALTURATELA/20, ALTURATELA/20};
        Texture2D mira = LoadTexture("resources/setavermelha.png");
        texturas["mira"] = mira;
    }

    Vector2 retornarPosicao(){
        return posicao;
    }

    void desenharJogador(){
        DrawRectangleRec(retanguloJogador, GREEN);
        DrawText("X", posicao.x, posicao.y, 10, RED);
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
        retanguloJogador.x = posicao.x - ALTURATELA/40;
        retanguloJogador.y = posicao.y - ALTURATELA/40;
    }

    void gerarMira(){

        Vector2 posicaoMouse = GetMousePosition();
        Vector2 vetorBase = {1,0};
        Vector2 vetorDirecao = {posicaoMouse.x -posicao.x, posicaoMouse.y - posicao.y};
        
        float angulorad = std::atan2(det2x2(vetorBase, vetorDirecao), vetorBase.x*vetorDirecao.x + vetorBase.y*vetorDirecao.y); // θ=atan2(ux​vy​−uy​vx​,ux​vx​+uy​vy​)

        float angulo = converterRadianoGraus(angulorad) + 90;

        if(angulo <0) angulo += 360;

        Vector2 centroImagem = {posicao.x + RAIOMIRA*std::cos(angulorad), posicao.y + RAIOMIRA*std::sin(angulorad)};

        Vector2 centro = { (float)texturas["mira"].width / 2.0f, (float)texturas["mira"].height / 2.0f };

            // RETÂNGULO DE ORIGEM: Pega a imagem inteira (do pixel 0,0 até a largura/altura total)
            Rectangle sourceRec = { 0.0f, 0.0f, (float)texturas["mira"].width, (float)texturas["mira"].height };

            // RETÂNGULO DE DESTINO: Define onde na tela e qual o tamanho final
            // Nota: x e y aqui serão onde o CENTRO da imagem vai ficar posicionado.
            Rectangle destRec = { centroImagem.x, centroImagem.y, (float)texturas["mira"].width, (float)texturas["mira"].height };

            // Desenha a imagem completa rotacionando no centro
            DrawTexturePro(texturas["mira"], sourceRec, destRec, centro, angulo, WHITE);
        
    }

    private:
        float velocidade;
        int vida;
        int balas;
        Rectangle retanguloJogador;
        Vector2 posicao;
        std::unordered_map<std::string, Texture2D> texturas; //  hash de texturas
};

class Arma{
    public:

        Arma(std::string n){
            nome = n;

            if(nome == "G18"){
                qtdMunicao = 6; dano = 1;  peso = 0.3f; frequencia = 1; velocidade = 3;
            } else if(nome == "AK47"){
                qtdMunicao = 24; dano = 3; peso = 2; frequencia = 0.1f; velocidade = 5;
            }
        }

        void atirar(Vector2 posicaoJogador){ // diminui a munição e joga na lista de projeteis
            qtdMunicao--;
            Vector2 posicaomouse = GetMousePosition();
            Vector2 vetordirecao = {posicaomouse.x - posicaoJogador.x, posicaomouse.y - posicaoJogador.y};
            vetordirecao = calcularModulo(vetordirecao);
            Projetil p = {{posicaoJogador.x, posicaoJogador.y, 20, 20}, vetordirecao, dano};
            listaProjeteis.push_back(p);
        }

        std::vector<int> percorrerProjeteis(std::vector<Zumbi> listaZumbis){
            std::vector<int> indices;

            for(int i = 0; i < listaProjeteis.size(); i++){
                Projetil projetil = listaProjeteis[i];
                DrawRectangleRec(projetil.retangulo, BLUE);

                for(int n = 0; n < listaZumbis.size(); n++){
                    //if()
                }

                projetil.retangulo.x += projetil.direcao.x*velocidade;
                projetil.retangulo.y += projetil.direcao.y*velocidade;                
            }

            return indices;
        }

    private:
        std::string nome;
        int qtdMunicao;
        int dano;
        float peso;
        float frequencia;
        float velocidade;
    
        typedef struct Projetil{
            Rectangle retangulo;
            Vector2 direcao;
            int danoProjetil;
        };
        
        std::vector<Projetil> listaProjeteis;

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

            ClearBackground(WHITE);
            player.gerarMira();

            
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

float det2x2(Vector2 a, Vector2 b){
    return a.x*b.y - a.y*b.x;
}