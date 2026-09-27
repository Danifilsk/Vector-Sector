#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define LARGURA 985
#define ALTURA 545
#define MAX_TEXTO 256


// telas que o jogo tem por enquanto
typedef enum {
    TELA_MENU,
    TELA_SELECAO_FASES,
    TELA_FASE1
} Tela;


// dados de cada caixa onde o jogador pode escrever
typedef struct {
    float x;
    float y;
    float largura;
    float altura;

    char texto[MAX_TEXTO];

    bool ativa;
} CaixaTexto;


// coloca uma imagem no tamanho da janela
void desenhar_imagem_tela(ALLEGRO_BITMAP *imagem) {

    int larguraImagem = al_get_bitmap_width(imagem);
    int alturaImagem = al_get_bitmap_height(imagem);

    al_draw_scaled_bitmap(
        imagem,
        0, 0,
        larguraImagem, alturaImagem,
        0, 0,
        LARGURA, ALTURA,
        0
    );

    al_flip_display();
}


// desenha uma caixa e o texto dela
void desenhar_caixa_texto(
    CaixaTexto *caixa,
    ALLEGRO_FONT *fonte
) {

    ALLEGRO_COLOR fundo = al_map_rgb(20, 20, 30);
    ALLEGRO_COLOR bordaNormal = al_map_rgb(150, 150, 150);
    ALLEGRO_COLOR bordaAtiva = al_map_rgb(0, 200, 255);
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);

    al_draw_filled_rectangle(
        caixa->x,
        caixa->y,
        caixa->x + caixa->largura,
        caixa->y + caixa->altura,
        fundo
    );

    // deixa a borda diferente quando a caixa está selecionada
    if (caixa->ativa) {

        al_draw_rectangle(
            caixa->x,
            caixa->y,
            caixa->x + caixa->largura,
            caixa->y + caixa->altura,
            bordaAtiva,
            3
        );

    } else {

        al_draw_rectangle(
            caixa->x,
            caixa->y,
            caixa->x + caixa->largura,
            caixa->y + caixa->altura,
            bordaNormal,
            2
        );
    }

    al_draw_text(
        fonte,
        branco,
        caixa->x + 10,
        caixa->y + 12,
        0,
        caixa->texto
    );
}


// redesenha o fundo da fase e coloca as caixas por cima
void desenhar_fase1(
    ALLEGRO_BITMAP *fase1,
    ALLEGRO_FONT *fonte,
    CaixaTexto caixas[],
    int quantidadeCaixas
) {

    int larguraImagem = al_get_bitmap_width(fase1);
    int alturaImagem = al_get_bitmap_height(fase1);

    al_draw_scaled_bitmap(
        fase1,
        0, 0,
        larguraImagem,
        alturaImagem,
        0, 0,
        LARGURA,
        ALTURA,
        0
    );

    for (int i = 0; i < quantidadeCaixas; i++) {
        desenhar_caixa_texto(&caixas[i], fonte);
    }

    al_flip_display();
}


// confere se o mouse clicou dentro da caixa
bool mouse_dentro_caixa(
    CaixaTexto *caixa,
    int mouseX,
    int mouseY
) {

    return (
        mouseX >= caixa->x &&
        mouseX <= caixa->x + caixa->largura &&
        mouseY >= caixa->y &&
        mouseY <= caixa->y + caixa->altura
    );
}


// cuida do texto digitado
void processar_digitacao(
    CaixaTexto *caixa,
    ALLEGRO_EVENT *evento
) {

    int tamanho = strlen(caixa->texto);

    if (evento->keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {

        if (tamanho > 0) {
            caixa->texto[tamanho - 1] = '\0';
        }

        return;
    }

    if (evento->keyboard.keycode == ALLEGRO_KEY_ENTER) {

        caixa->ativa = false;

        return;
    }

    int caractere = evento->keyboard.unichar;

    // caracteres comuns
    if (caractere >= 32 && caractere <= 126) {

        if (tamanho < MAX_TEXTO - 1) {

            caixa->texto[tamanho] = (char)caractere;
            caixa->texto[tamanho + 1] = '\0';
        }
    }
}


int main(void) {

    if (!al_init()) {
        printf("Erro ao iniciar Allegro!\n");
        return 1;
    }

    if (!al_init_image_addon()) {
        printf("Erro ao iniciar imagens!\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        printf("Erro ao iniciar primitives!\n");
        return 1;
    }

    al_init_font_addon();

    if (!al_install_mouse()) {
        printf("Erro ao iniciar mouse!\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao iniciar teclado!\n");
        return 1;
    }


    // janela
    ALLEGRO_DISPLAY *display =
        al_create_display(LARGURA, ALTURA);

    if (!display) {
        printf("Erro ao criar janela!\n");
        return 1;
    }

    al_set_window_title(display, "Vector Sector");


    // fonte padrão do Allegro
    ALLEGRO_FONT *fonte =
        al_create_builtin_font();

    if (!fonte) {
        printf("Erro ao criar fonte!\n");
        return 1;
    }


    // imagens
    ALLEGRO_BITMAP *menu =
        al_load_bitmap("menu.png");

    ALLEGRO_BITMAP *selecaoFases =
        al_load_bitmap("selecao_fases.jpg");

    ALLEGRO_BITMAP *fase1 =
        al_load_bitmap("fase1.jpg");


    if (!menu) {
        printf("Erro ao carregar menu.png!\n");
        return 1;
    }

    if (!selecaoFases) {
        printf("Erro ao carregar selecao_fases.jpg!\n");
        return 1;
    }

    if (!fase1) {
        printf("Erro ao carregar fase1.jpg!\n");
        return 1;
    }


    // fila de eventos
    ALLEGRO_EVENT_QUEUE *fila =
        al_create_event_queue();

    if (!fila) {
        printf("Erro ao criar fila!\n");
        return 1;
    }

    al_register_event_source(
        fila,
        al_get_display_event_source(display)
    );

    al_register_event_source(
        fila,
        al_get_mouse_event_source()
    );

    al_register_event_source(
        fila,
        al_get_keyboard_event_source()
    );


    // botão start
    int startX1 = 395;
    int startY1 = 180;
    int startX2 = 580;
    int startY2 = 255;

    // botão exit
    int exitX1 = 390;
    int exitY1 = 298;
    int exitX2 = 583;
    int exitY2 = 360;

    // botão settings
    int settingsX1 = 363;
    int settingsY1 = 392;
    int settingsX2 = 611;
    int settingsY2 = 456;


    // botão voltar
    int voltarX1 = 0;
    int voltarY1 = 0;
    int voltarX2 = 160;
    int voltarY2 = 70;


    // setor 1
    int setor1X1 = 65;
    int setor1Y1 = 155;
    int setor1X2 = 220;
    int setor1Y2 = 345;


    // caixas de texto da fase 1
    CaixaTexto caixas[2];


    caixas[0].x = 100;
    caixas[0].y = 400;
    caixas[0].largura = 350;
    caixas[0].altura = 50;
    caixas[0].texto[0] = '\0';
    caixas[0].ativa = false;


    caixas[1].x = 520;
    caixas[1].y = 400;
    caixas[1].largura = 350;
    caixas[1].altura = 50;
    caixas[1].texto[0] = '\0';
    caixas[1].ativa = false;


    int quantidadeCaixas = 2;

    bool rodando = true;

    Tela telaAtual = TELA_MENU;


    desenhar_imagem_tela(menu);


    while (rodando) {

        ALLEGRO_EVENT evento;

        al_wait_for_event(fila, &evento);


        // fechar pelo X
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }


        // ESC
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {

            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

                if (telaAtual == TELA_FASE1) {

                    telaAtual = TELA_SELECAO_FASES;

                    desenhar_imagem_tela(selecaoFases);

                } else if (telaAtual == TELA_SELECAO_FASES) {

                    telaAtual = TELA_MENU;

                    desenhar_imagem_tela(menu);

                } else if (telaAtual == TELA_MENU) {

                    rodando = false;
                }
            }
        }


        // texto digitado nas caixas
        if (
            evento.type == ALLEGRO_EVENT_KEY_CHAR &&
            telaAtual == TELA_FASE1
        ) {

            for (int i = 0; i < quantidadeCaixas; i++) {

                if (caixas[i].ativa) {

                    processar_digitacao(
                        &caixas[i],
                        &evento
                    );

                    desenhar_fase1(
                        fase1,
                        fonte,
                        caixas,
                        quantidadeCaixas
                    );

                    break;
                }
            }
        }


        // mouse
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            int mouseX = evento.mouse.x;
            int mouseY = evento.mouse.y;

            if (evento.mouse.button == 1) {


                // menu
                if (telaAtual == TELA_MENU) {

                    if (
                        mouseX >= startX1 &&
                        mouseX <= startX2 &&
                        mouseY >= startY1 &&
                        mouseY <= startY2
                    ) {

                        printf("START clicado!\n");

                        telaAtual = TELA_SELECAO_FASES;

                        desenhar_imagem_tela(selecaoFases);
                    }


                    else if (
                        mouseX >= exitX1 &&
                        mouseX <= exitX2 &&
                        mouseY >= exitY1 &&
                        mouseY <= exitY2
                    ) {

                        printf("EXIT clicado!\n");

                        rodando = false;
                    }


                    else if (
                        mouseX >= settingsX1 &&
                        mouseX <= settingsX2 &&
                        mouseY >= settingsY1 &&
                        mouseY <= settingsY2
                    ) {

                        printf("SETTINGS clicado!\n");
                    }
                }


                // seleção de fases
                else if (telaAtual == TELA_SELECAO_FASES) {

                    if (
                        mouseX >= voltarX1 &&
                        mouseX <= voltarX2 &&
                        mouseY >= voltarY1 &&
                        mouseY <= voltarY2
                    ) {

                        telaAtual = TELA_MENU;

                        desenhar_imagem_tela(menu);
                    }


                    else if (
                        mouseX >= setor1X1 &&
                        mouseX <= setor1X2 &&
                        mouseY >= setor1Y1 &&
                        mouseY <= setor1Y2
                    ) {

                        printf("Entrando na fase 1!\n");

                        telaAtual = TELA_FASE1;

                        desenhar_fase1(
                            fase1,
                            fonte,
                            caixas,
                            quantidadeCaixas
                        );
                    }
                }


                // caixas da fase 1
                else if (telaAtual == TELA_FASE1) {

                    // primeiro tira o foco de todas
                    for (int i = 0; i < quantidadeCaixas; i++) {
                        caixas[i].ativa = false;
                    }


                    // depois procura qual foi clicada
                    for (int i = 0; i < quantidadeCaixas; i++) {

                        if (
                            mouse_dentro_caixa(
                                &caixas[i],
                                mouseX,
                                mouseY
                            )
                        ) {

                            caixas[i].ativa = true;

                            break;
                        }
                    }


                    desenhar_fase1(
                        fase1,
                        fonte,
                        caixas,
                        quantidadeCaixas
                    );
                }
            }
        }
    }


    al_destroy_font(fonte);

    al_destroy_bitmap(menu);
    al_destroy_bitmap(selecaoFases);
    al_destroy_bitmap(fase1);

    al_destroy_event_queue(fila);
    al_destroy_display(display);

    al_shutdown_primitives_addon();
    al_shutdown_image_addon();

    return 0;
}