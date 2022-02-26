#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


#define NAVEGACAO 0
#define BATALHA 1
#define VITORIA 2
#define DERROTA 3
#define DANO 4
#define DANOE 5
#define TAM_HEROI 40
#define TAM_MONSTRO 5
#define PASSO 20
#define TAM_OBJ 100
#define MAX_DIST 35
#define MAX_DISTM 100
#define MONSTROS 19
#define ATACAR 0
#define ESPECIAL 1
#define FUGIR 2
#define RAIO 20

const float FPS = 10;  

const int BOUNCER_SIZE = 32;

const int SCREEN_W = 960;
const int SCREEN_H = 540;

// ================================================================================================================== globais

int CEU_H;
int X_OPT;
int Y_OPT;
int tam_font;
int tam_font1;
int VITORIA_X;
int VITORIA_Y;

ALLEGRO_FONT *FONT;
ALLEGRO_FONT *FONT1;

//aleaório entre 0 e n-1
int random(int n){
  return rand()%n;
}

//aleaório entre int min e max (inclusive)
int randInt(int min, int max){
  return min + random(max - min + 1);
}

//=================================================================================================================== estruturas

typedef struct Heroi
{
	int x, y, centrox, centroy, batalhaX, batalhaY;
	int dano;
	ALLEGRO_COLOR cor;
	int hp;
	char score[20];
	int pontos;

	//batalha
	int acao;
	int executar;
	int x_old, y_old, centrox_old, centroy_old;

}Heroi;

typedef struct Monstro
{
	int x, y;
	int batalhaXm;
	int batalhaYm;
	ALLEGRO_COLOR cor;
	int hp;
	int dano;
	int pontos;

}Monstro;

typedef struct Cursor
{
	int x;
	int y;
}Cursor;

typedef struct Bala
{
	int x;
	int y;
	ALLEGRO_COLOR cor;
}Bala;

//=================================================================================================================== iniciadores

void initHeroi(Heroi *h){
	h->x = 0;
	h->y = SCREEN_H - TAM_HEROI;
	h->pontos = 0;
	
	h->centrox = h->x + TAM_HEROI/2;
	h->centroy = h->y + TAM_HEROI/2;

	h->batalhaX = 9*SCREEN_W/16;
	h->batalhaY = SCREEN_H/9;


	h->dano = 20;

	h->cor = al_map_rgb(10, 10, 10);
	h->hp = 100;
	h->acao = ATACAR;
	h->executar = 0;
	h->x_old = h->x;
	h->y_old = h->y;
	h->centrox_old = h->centrox;
	h->centroy_old = h->centroy;

}

void initMonstro(Monstro *m, Heroi *h){
	m->x = rand()%SCREEN_W;
	m->y = rand()%SCREEN_H;
	m->batalhaXm = SCREEN_W/16;
	m->batalhaYm = (SCREEN_H/9) + 20;
	do
	{
		m->x = rand()%SCREEN_W;
		m->y = rand()%SCREEN_H;
	} while ((m->x >= SCREEN_W - TAM_OBJ - 20 && m->y <= TAM_OBJ + 20) || (m->x >= h->x + 20 && m->y >= h->y - 20));
	
	m->cor = al_map_rgb(0, 0, 0);
	m->hp = randInt(50, 100);
	m->dano = randInt(5, 10);
	if (m->x >= 480 && m->y >= 270)
	{
		m->hp = 200;
		m->dano = 150;
	}
	if(m->dano < 5){
		m->pontos = 2;
	}
	if(m->dano < 10){
		m->pontos = 5;
	}
	if (m->dano < 15)
	{
		m->pontos = 10;
	}
	if (m->dano == 15){
		m->pontos = 20;
	}

	
}

void initBala(Bala *b, Heroi *h, Monstro *m){
	b->x = h->batalhaX + 20;
	b->y = h->batalhaY + 20;
	b->cor = al_map_rgb(0, 0, 0);
}

void initGlobais(){
	CEU_H = SCREEN_H/4;
	X_OPT = 3*SCREEN_W/4;
	Y_OPT = 3*SCREEN_H/4;
	
	tam_font = 25;
	tam_font1 = 50;
	if (SCREEN_W < 450)
	{
		tam_font = 17;
	}

	VITORIA_Y = SCREEN_H/2 - (2*tam_font1);
	VITORIA_X = SCREEN_W/5 - 10;

	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    FONT = al_load_font("Minecraft.ttf", tam_font, 1);   
	if(FONT == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

	FONT1 = al_load_font("Minecraft.ttf", tam_font1, 1);   
	if(FONT == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

}

void initCursor(Cursor *c){
	c->x = SCREEN_W - TAM_HEROI - 50;
	c->y = Y_OPT + 22;
}

// ==================================== NAVEG ===============================================================================

void desenhaHeroiNaveg(Heroi h){

	ALLEGRO_BITMAP *image = al_load_bitmap("nave.png");

	
	al_draw_bitmap(image, h.x, h.y, 0);
}

void desenhaHeroiNavegD(Heroi h){

	ALLEGRO_BITMAP *image = al_load_bitmap("naveD.png");


	al_draw_bitmap(image, h.x, h.y, 0);
}

void desenhaHeroiNavegL(Heroi h){
	
	ALLEGRO_BITMAP *image = al_load_bitmap("naveL.png");

	al_draw_bitmap(image, h.x, h.y, 0);
}

void desenhaHeroiNavegR(Heroi h){

	ALLEGRO_BITMAP *image = al_load_bitmap("naveR.png");


	al_draw_bitmap(image, h.x, h.y, 0);
}

void desenhaMonstroNaveg(Monstro m){
	al_draw_filled_rectangle(m.x, m.y, m.x + TAM_MONSTRO, m.y + TAM_MONSTRO, m.cor);
}

void desenhaCenarioNaveg(Heroi *h){

	al_clear_to_color(al_map_rgb(240, 242, 242));

	al_draw_filled_circle( SCREEN_W - TAM_OBJ/2 - 10, TAM_OBJ/2 + 10, 55, al_map_rgb(120, 120, 120));
	al_draw_filled_circle( SCREEN_W - TAM_OBJ/2 - 10 - 10, TAM_OBJ/2 + 10, 25, al_map_rgb(100, 100, 100));
	al_draw_filled_rectangle(SCREEN_W - TAM_OBJ - 15, TAM_OBJ/2 + 10, SCREEN_W, TAM_OBJ + 38, al_map_rgb(240, 242, 242));

	sprintf(h->score, "%d", h->pontos);
	al_draw_text(FONT, al_map_rgb(0, 0, 0), 10, 10, 0, h->score);

}

int processaTeclaNaveg(Heroi *h, int tecla){

	h->x_old = h->x;
	h->y_old = h->y;
	h->centrox_old = h->centrox;
	h->centroy_old = h->centroy;



	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
		if (h->y - PASSO >= 0)
		{
			h->y -= PASSO;
			h->centroy -= PASSO;
		}
		return 1;
		break;
	
	case ALLEGRO_KEY_DOWN:
		if (h->y + TAM_HEROI + PASSO <= SCREEN_H)
		{
			h->y += PASSO;
			h->centroy += PASSO;
		}
		return 2;
		break;
	
	case ALLEGRO_KEY_LEFT:
		if (h->x - PASSO >= 0)
		{
			h->x -= PASSO;
			h->centrox -= PASSO;
		
		}
		return 3;
		break;
	
	case ALLEGRO_KEY_RIGHT:
		if (h->x + TAM_HEROI + PASSO <= SCREEN_W)
		{
			h->x += PASSO;
			h->centrox += PASSO;

		}
		return 4;
		break;
	}
}

float dist(Heroi *h, Monstro *m){

	// distancia euclidiana ==============================================
	return sqrt(pow(h->centrox - m->x,2) + pow(h->centroy - m->y,2));

}

float distMonstro(Monstro *m1, Monstro *m2){

	// distancia euclidiana ==============================================
	return sqrt(pow(m1->x - m2->x,2) + pow(m1->y - m2->y,2));

}

int defineEspaco(Monstro m1, Monstro m2, Heroi h){
	if (distMonstro(&m1, &m2) <= MAX_DISTM){
		return 1;
	}
	else{
		return 0;
	}

}

int detectouMonstro(Heroi h, Monstro m){
	if (dist(&h, &m) <= MAX_DIST)
	{
		return 1;
	}
	return 0;
	
}

int chegouObjetivoHerois(Heroi h){

	if (h.x >= SCREEN_W - TAM_OBJ && h.y + TAM_HEROI <= TAM_OBJ)
	{
		return 1;
	}
	return 0;
}

// ==================================== BATALHA=======================================================================

void desenhaCenarioBatalha(){
	al_clear_to_color(al_map_rgb(189, 189, 189));
	al_draw_filled_rectangle(0, 0, SCREEN_W, CEU_H, al_map_rgb(232, 232, 232));
	al_draw_filled_rectangle(X_OPT - 10, Y_OPT -10, SCREEN_W, SCREEN_H, al_map_rgb(100, 100, 100));
	al_draw_filled_rectangle(X_OPT, Y_OPT, SCREEN_W - 10, SCREEN_H -10, al_map_rgb(50, 50, 50));
	al_draw_text(FONT, al_map_rgb(250, 250, 250), X_OPT + 40, Y_OPT + 10, 0, "Atacar");
	al_draw_text(FONT, al_map_rgb(250, 250, 250), X_OPT + 40, Y_OPT + 50, 0, "Especial");
	al_draw_text(FONT, al_map_rgb(250, 250, 250), X_OPT + 40, Y_OPT + 90, 0, "Fugir");

}

void desenhaBatalha(Heroi h, Monstro m){
	
	al_draw_filled_rectangle(h.batalhaX + 85, h.batalhaY , h.batalhaX + 215, h.batalhaY + 20, al_map_rgb(158, 158, 158));

	al_draw_filled_rectangle(h.batalhaX + 100, h.batalhaY + 5, h.batalhaX + 100 + h.hp, h.batalhaY + 15, al_map_rgb(220, 220, 220));

	al_draw_filled_rectangle(m.batalhaXm + 115, m.batalhaYm - 20 , m.batalhaXm + 245, m.batalhaYm, al_map_rgb(158, 158, 158));

	al_draw_filled_rectangle(m.batalhaXm + 130, m.batalhaYm - 15, (m.batalhaXm + 80) + 50 + m.hp, m.batalhaYm - 5, al_map_rgb(220, 220, 220));

	ALLEGRO_BITMAP *image = al_load_bitmap("heroi.png");

	al_draw_bitmap(image, h.batalhaX, h.batalhaY, 0);

	ALLEGRO_BITMAP *image1 = al_load_bitmap("monstro.png");

	al_draw_bitmap(image1, m.batalhaXm, m.batalhaYm, 0);
}

void desenhaCursor(Cursor c){
	al_draw_filled_circle(c.x, c.y, 5, al_map_rgb(150, 150, 150));
}

void processaTeclaBatalha(Heroi *h, int tecla, Cursor *c){
	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
		if (h->acao == 0)
		{
			break;
		}
		else{
			c->y -= 40;
			h->acao--;
		}
		break;
	
	case ALLEGRO_KEY_DOWN:
		if (h->acao == 2)
		{
			break;
		}
		else{
			c->y += 40;
			h->acao++;
		}
		break;

	case ALLEGRO_KEY_ENTER:
		h->executar = 1;
		break;
	
	}
}

int processaAcaoHeroi(Heroi *h, Monstro *m){

	if (h->executar == 1)
	{
		h->executar = 0;

		if (h->acao == FUGIR)
		{
			int fuga = randInt(0, 10);
			if (fuga < 8)
			{
				h->x = h->x_old;
				h->y = h->y_old;
				h->centrox = h->centrox_old;
				h->centroy = h->centroy_old;

				return NAVEGACAO;
			}
			al_rest(0.1);
			h->hp -= m->dano;

			if(h->hp <= 0){
				return DERROTA;
			}
			
		}
		if (h->acao == ATACAR)
		{
			return DANO;
		}
		if (h->acao == ESPECIAL)
		{
			return DANOE;
		}
		
	}
	return BATALHA;
	
}

int processaAtaque(ALLEGRO_EVENT *ev, Heroi *h, Monstro *m){
	m->hp -= h->dano;
	if(m->hp <= 0){
		h->pontos += m->pontos;
		m->x = -20;
		m->y = -20;
		return NAVEGACAO;
	}
	
	h->hp -= m->dano;
	if (h->hp < 0)
	{
		return DERROTA;
	}
	return BATALHA;
}

int processaAtaqueE(ALLEGRO_EVENT *ev, Heroi *h, Monstro *m){
	m->hp -= randInt(5, 50);
	if(m->hp <= 0){
		h->pontos += m->pontos;
		m->x = -20;
		m->y = -20;
		return NAVEGACAO;
	}
	h->hp -= m->dano;
	if (h->hp < 0)
	{
		return DERROTA;
	}
	return BATALHA;
}
//================================================================= FIM DE GAME ================================================

void desenhaVitoria(Heroi *h){
	al_clear_to_color(al_map_rgb(0, 0, 0));

	FILE *arq;
	arq = fopen("score.txt", "r");
	char score[20];
	char token2[20];
	fscanf(arq, "%s", score);
	int token;
	token = atoi(strtok(score, "\0"));

	al_draw_text(FONT, al_map_rgb(250, 250, 250), 410, 300, 0, "Record: ");

	sprintf(token2, "%d", token);
	al_draw_text(FONT, al_map_rgb(250, 250, 250), 520, 300, 0, token2);
	fclose(arq);

	if(h->pontos > token){
		sprintf(h->score, "%d", h->pontos);
		al_draw_text(FONT, al_map_rgb(250, 250, 250), 380, 350, 0, "Novo record: ");
		al_draw_text(FONT, al_map_rgb(250, 250, 250), 545, 350, 0, h->score);
	}
	else{
		sprintf(h->score, "%d", h->pontos);
		al_draw_text(FONT, al_map_rgb(250, 250, 250), 395, 350, 0, "Pontuacao: ");
		al_draw_text(FONT, al_map_rgb(250, 250, 250), 545, 350, 0, h->score);
	}
	
	al_draw_text(FONT1, al_map_rgb(250, 250, 250), VITORIA_X - 5, VITORIA_Y, 0, "Voce Chegou ao Objetivo");
	al_draw_text(FONT, al_map_rgb(250, 250, 250), VITORIA_X + SCREEN_W/9, VITORIA_Y + SCREEN_H/2, 0, "Precione ENTER para finalizar");

}

int processaTeclaVitoria(int teclaV, Heroi *h){
	switch (teclaV)
	{
	case ALLEGRO_KEY_ENTER:
		return 1;
		break;
	
	default:
		return 0;
		break;
	}
}

void desenhaDerrota(){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(FONT1, al_map_rgb(250, 250, 250), VITORIA_X, VITORIA_Y, 0, "VOCE FOI ANIQUILADO");
	al_draw_text(FONT, al_map_rgb(250, 250, 250), VITORIA_X + SCREEN_W/10, VITORIA_Y + SCREEN_H/2, 0, "Precione ENTER para finalizar");
}

int processaTeclaDerrota(int teclaV){

	switch (teclaV)
	{
	case ALLEGRO_KEY_ENTER:
		return 1;
		break;
	
	default:
		return 0;
		break;
	}
}
//========================================================================================================================== main
 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//========================================================= rotinas dde inicialização
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	
 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	


	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;

	int modo_jogo = NAVEGACAO;

	Heroi heroi;

	initGlobais();

	initHeroi(&heroi);

	int teclaAperta = 0;
	int fim = 0;
	int detecta;

	Monstro mob[MONSTROS];
	Monstro m;
	int i = 1, j = 0;

	for (i = 0; i < MONSTROS; i++)
	{
		
		initMonstro(&m, &heroi);
		mob[i] = m;

		for ( j = 0; j < i; j++)
		{
			while(defineEspaco(mob[i], mob[j], heroi) == 1){
				initMonstro(&m, &heroi);
				mob[i] = m;
				continue;
			}
		}
		printf("\nELES TE OBSERVAM %d, %d, %d\n", mob[i].x, mob[i].y, mob[i].dano);
	}


	Cursor cursor;

	initCursor(&cursor);
	
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			if (modo_jogo == NAVEGACAO)
			{
				desenhaCenarioNaveg(&heroi);
				if (teclaAperta == 0)
				{
					desenhaHeroiNaveg(heroi);
				}
				if (teclaAperta == 1)
				{
					desenhaHeroiNaveg(heroi);
				}
				if (teclaAperta == 2)
				{
					desenhaHeroiNavegD(heroi);
				}
				if (teclaAperta == 3)
				{
					desenhaHeroiNavegL(heroi);
				}
				if (teclaAperta == 4)
				{
					desenhaHeroiNavegR(heroi);
				}


				for (int i = 0; i < MONSTROS; i++)
				{
					desenhaMonstroNaveg(mob[i]);
				}
				

				for (int i = 0; i < MONSTROS; i++)
				{	
					if (detectouMonstro(heroi, mob[i]))
					{
						detecta = i;
						modo_jogo = BATALHA;
					}

				}
				

				if (chegouObjetivoHerois(heroi))
				{
					modo_jogo = VITORIA;
				}
				
			}

			//============================================== vitoria
			else if(modo_jogo == VITORIA){
				desenhaVitoria(&heroi);
			}
			
			//=============================================== batalha
			else if(modo_jogo == BATALHA)
			{
				desenhaCenarioBatalha();
				desenhaBatalha(heroi, mob[detecta]);
				desenhaCursor(cursor);
				modo_jogo = processaAcaoHeroi(&heroi, &mob[detecta]);
				
			}
			else if(modo_jogo == DANO)
			{
				desenhaCenarioBatalha();
				desenhaBatalha(heroi, mob[detecta]);
				desenhaCursor(cursor);
				modo_jogo = processaAtaque(&ev, &heroi, &mob[detecta]);
			}
			else if(modo_jogo == DANOE)
			{
				desenhaCenarioBatalha();
				desenhaBatalha(heroi, mob[detecta]);
				desenhaCursor(cursor);
				modo_jogo = processaAtaqueE(&ev, &heroi, &mob[detecta]);
			}

			else if(modo_jogo == DERROTA){
				desenhaDerrota();
			}
			

			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			if (modo_jogo == NAVEGACAO)
			{
				teclaAperta = processaTeclaNaveg(&heroi, ev.keyboard.keycode);
			}
			else if(modo_jogo == VITORIA)
			{
				fim = processaTeclaVitoria(ev.keyboard.keycode, &heroi);
				if(fim == 1){
					FILE *arq;
					arq = fopen("score.txt", "r");
					char record[10];
					fgets(record, 10, arq);
					int token;
					token = atoi(strtok(record, "\0"));
					fclose(arq);
					if (heroi.pontos > token)
					{
						arq = fopen("score.txt", "w");
						sprintf(heroi.score, "%d", heroi.pontos);
						fprintf(arq, heroi.score);
						fclose(arq);
					}
					playing = 0;
				}
			}
			else if(modo_jogo == DERROTA)
			{
				fim = processaTeclaDerrota(ev.keyboard.keycode);
				if(fim == 1){
					playing = 0;
				}
			}
			else{
				processaTeclaBatalha(&heroi, ev.keyboard.keycode, &cursor);
			}
			
		}

	} //fim do while

     
	//============================================================================================================
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}