#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define LARGURA 985
#define ALTURA 545

// Estados/telas do jogo
typedef enum {
    TELA_MENU,
    TELA_SELECAO_FASES,
    TELA_FASE1
} Tela;


// Desenha uma imagem ocupando a janela inteira
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


int main() {

    // ==========================
    // INICIALIZAÇÃO DO ALLEGRO
    // ==========================

    if (!al_init()) {
        printf("Erro ao iniciar Allegro!\n");
        return 1;
    }

    if (!al_init_image_addon()) {
        printf("Erro ao iniciar addon de imagens!\n");
        return 1;
    }

    if (!al_install_mouse()) {
        printf("Erro ao iniciar mouse!\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao iniciar teclado!\n");
        return 1;
    }


    // ==========================
    // CRIAÇÃO DA JANELA
    // ==========================

    ALLEGRO_DISPLAY *display =
        al_create_display(LARGURA, ALTURA);

    if (!display) {
        printf("Erro ao criar janela!\n");
        return 1;
    }

    al_set_window_title(display, "Vector Sector");


    // ==========================
    // CARREGAMENTO DAS IMAGENS
    // ==========================

    ALLEGRO_BITMAP *menu =
        al_load_bitmap("menu.png");

    ALLEGRO_BITMAP *selecaoFases =
        al_load_bitmap("selecao_fases.jpg");

    ALLEGRO_BITMAP *fase1 =
        al_load_bitmap("fase1.jpg");


    // Confere se as imagens carregaram
    if (!menu) {
        printf("Erro ao carregar menu.png!\n");

        al_destroy_display(display);
        return 1;
    }

    if (!selecaoFases) {
        printf("Erro ao carregar selecao_fases.jpg!\n");

        al_destroy_bitmap(menu);
        al_destroy_display(display);
        return 1;
    }

    if (!fase1) {
        printf("Erro ao carregar fase1.jpg!\n");

        al_destroy_bitmap(menu);
        al_destroy_bitmap(selecaoFases);
        al_destroy_display(display);

        return 1;
    }


    // ==========================
    // FILA DE EVENTOS
    // ==========================

    ALLEGRO_EVENT_QUEUE *fila =
        al_create_event_queue();

    if (!fila) {
        printf("Erro ao criar fila de eventos!\n");

        al_destroy_bitmap(menu);
        al_destroy_bitmap(selecaoFases);
        al_destroy_bitmap(fase1);
        al_destroy_display(display);

        return 1;
    }


    // Eventos da janela
    al_register_event_source(
        fila,
        al_get_display_event_source(display)
    );

    // Eventos do mouse
    al_register_event_source(
        fila,
        al_get_mouse_event_source()
    );

    // Eventos do teclado
    al_register_event_source(
        fila,
        al_get_keyboard_event_source()
    );


    // ==========================
    // ÁREAS CLICÁVEIS DO MENU
    // ==========================

    // START
    int startX1 = 395;
    int startY1 = 180;
    int startX2 = 580;
    int startY2 = 255;

    // EXIT
    int exitX1 = 390;
    int exitY1 = 298;
    int exitX2 = 583;
    int exitY2 = 360;

    // SETTINGS
    int settingsX1 = 363;
    int settingsY1 = 392;
    int settingsX2 = 611;
    int settingsY2 = 456;


    // ==========================
    // ÁREAS DA SELEÇÃO DE FASES
    // ==========================

    // Botão VOLTAR
    int voltarX1 = 0;
    int voltarY1 = 0;
    int voltarX2 = 160;
    int voltarY2 = 70;


    // SETOR 01 / PLANETA TERRA
    int setor1X1 = 65;
    int setor1Y1 = 155;
    int setor1X2 = 220;
    int setor1Y2 = 345;


    // ==========================
    // ESTADO INICIAL DO JOGO
    // ==========================

    bool rodando = true;

    Tela telaAtual = TELA_MENU;


    // Desenha o menu principal
    desenhar_imagem_tela(menu);


    // ==========================
    // LOOP PRINCIPAL
    // ==========================

    while (rodando) {

        ALLEGRO_EVENT evento;

        // Espera algum evento acontecer
        al_wait_for_event(fila, &evento);


        // ==========================
        // FECHAR JANELA
        // ==========================

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            rodando = false;
        }


        // ==========================
        // TECLADO
        // ==========================

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {

            // Apertou ESC
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

                // Se estiver na fase 1,
                // volta para seleção de fases
                if (telaAtual == TELA_FASE1) {

                    telaAtual = TELA_SELECAO_FASES;

                    desenhar_imagem_tela(selecaoFases);

                    printf("Voltando para selecao de fases...\n");
                }


                // Se estiver na seleção,
                // volta para o menu
                else if (telaAtual == TELA_SELECAO_FASES) {

                    telaAtual = TELA_MENU;

                    desenhar_imagem_tela(menu);

                    printf("Voltando para o menu...\n");
                }


                // Se estiver no menu,
                // fecha o jogo
                else if (telaAtual == TELA_MENU) {

                    rodando = false;
                }
            }
        }


        // ==========================
        // MOUSE
        // ==========================

        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            int mouseX = evento.mouse.x;
            int mouseY = evento.mouse.y;


            // Botão esquerdo do mouse
            if (evento.mouse.button == 1) {


                // ==========================================
                // MENU PRINCIPAL
                // ==========================================

                if (telaAtual == TELA_MENU) {


                    // START
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


                    // EXIT
                    else if (
                        mouseX >= exitX1 &&
                        mouseX <= exitX2 &&
                        mouseY >= exitY1 &&
                        mouseY <= exitY2
                    ) {

                        printf("EXIT clicado!\n");

                        rodando = false;
                    }


                    // SETTINGS
                    else if (
                        mouseX >= settingsX1 &&
                        mouseX <= settingsX2 &&
                        mouseY >= settingsY1 &&
                        mouseY <= settingsY2
                    ) {

                        printf("SETTINGS clicado!\n");

                        // Depois fazemos a tela de configurações
                    }
                }


                // ==========================================
                // SELEÇÃO DE FASES
                // ==========================================

                else if (telaAtual == TELA_SELECAO_FASES) {


                    // BOTÃO VOLTAR
                    if (
                        mouseX >= voltarX1 &&
                        mouseX <= voltarX2 &&
                        mouseY >= voltarY1 &&
                        mouseY <= voltarY2
                    ) {

                        printf("Voltando para o menu...\n");

                        telaAtual = TELA_MENU;

                        desenhar_imagem_tela(menu);
                    }


                    // SETOR 01
                    else if (
                        mouseX >= setor1X1 &&
                        mouseX <= setor1X2 &&
                        mouseY >= setor1Y1 &&
                        mouseY <= setor1Y2
                    ) {

                        printf("Entrando na Fase 1!\n");

                        telaAtual = TELA_FASE1;

                        desenhar_imagem_tela(fase1);
                    }
                }


                // ==========================================
                // FASE 1
                // ==========================================

                else if (telaAtual == TELA_FASE1) {

                    /*
                        Por enquanto não há nada clicável aqui.

                        Depois entram:
                        - nave
                        - inimigos
                        - vetores
                        - movimento
                        - combate
                        etc.
                    */
                }
            }
        }
    }


    // ==========================
    // LIBERAR MEMÓRIA
    // ==========================

    al_destroy_bitmap(menu);

    al_destroy_bitmap(selecaoFases);

    al_destroy_bitmap(fase1);

    al_destroy_event_queue(fila);

    al_destroy_display(display);

    al_shutdown_image_addon();


    return 0;
}