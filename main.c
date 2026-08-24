#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define LARGURA 985
#define ALTURA 545

int main(){
    //situações de possivel erro
    if(!al_init()){
        printf("Erro ao iniciar allegro\n");
        return 1;
    }
    if(!al_init_image_addon()){
        printf("Erro ao iniciar o addon de imagens!\n");
        return 1;
    }
    if(!al_install_mouse()){
        printf("Erro ao iniciar o mouse!\n");
        return 1;
    }
    //inicializador da janela
    ALLEGRO_DISPLAY *display =
    al_create_display(LARGURA, ALTURA);
    if(!display){
        printf("Erro ao criar janela!\n");
        return 1;
    }
    al_set_window_title(display, "Vector Sector");

    //carrega a imagem do canva la
    ALLEGRO_BITMAP *menu =
    al_load_bitmap("menu.png");

    if(!menu){
        printf("Erro ao carregar o menu.png!");
        al_destroy_display(display);
        return 1;
    }
    //fila de eventos
    ALLEGRO_EVENT_QUEUE *fila =
    al_create_event_queue();

    al_register_event_source(
        fila,
        al_get_display_event_source(display)
    );
    al_register_event_source(
        fila,
        al_get_mouse_event_source()
    );

    /*
    AREAS CLICAVEIS (não sei exatamente onde estão os botões na tela
    mas o gpt me deu uma aproximação dos pixeis)
    */

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

   bool rodando = true;

   //desenha o menu inicialmente
   al_draw_bitmap(menu, 0, 0, 0);
   al_flip_display();

   while(rodando){
    ALLEGRO_EVENT evento;

    //espera algum evento acontecer
    al_wait_for_event(fila, &evento);

    //usuario fechou a janela]
    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        rodando = false;
    }

    //usuario clicou com o mouse
    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        int mouseX = evento.mouse.x;
        int mousey = evento.mouse.y;
        //botão esquerdo
        if(evento.mouse.button == 1){
            //start
            if(mouseX >= startX1 && mouseX <= startX2 && mousey >= startY1 && mousey <= startY2){
                printf("start clicado!");
                //aqui começa o jogo então depois a gente muda aqui
            }
            //exit
            else if(mouseX >= exitX1 && mouseX <= exitX2 && mousey >= exitY1 && mousey <= exitY2){
                printf("EXIT clicado!");
                
                rodando = false;
            }
            if(mouseX >= settingsX1 && mouseX <= settingsX2 && mousey >= settingsY1 && mousey <= settingsY2){
                printf("settings clicado!");

                //aqui a gente meche no settings
            }
        }
    }
   }
   //libera memoria
   al_destroy_bitmap(menu);
   al_destroy_event_queue(fila);
   al_destroy_display(display);

   return 0;
}