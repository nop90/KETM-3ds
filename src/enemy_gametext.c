#include "enemy.h"

extern double fps_factor;

/* not a real enemy - just a line of text */

typedef struct {
	int state;
	double speed;
	double destx;
	double desty;
	double wait;
} GAMETEXT_DATA;

void enemy_gametext_add(char *text, int ypos)
{
	SDL_Surface *sur;
	SPRITE *s;
	GAMETEXT_DATA *g;


	sur=font_render(text,FONT04);
	s=sprite_add(sur,1,PR_BONUS,1);
	s->flags|=SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type=SP_ETC;
	s->x=WIDTH;
	s->y=ypos;
	s->mover=enemy_gametext_mover;
	g=mmalloc(sizeof(GAMETEXT_DATA));
	s->data=g;
	g->destx=(WIDTH/2)-sur->w/2;
	g->desty=ypos;
	g->speed=5;
	g->wait=0;
	g->state=0;
}

void enemy_gametext_mover(SPRITE *s)
{
	GAMETEXT_DATA *g=(GAMETEXT_DATA *)s->data;
	SDL_Surface *sur;

	switch(g->state) {
	case 0:
		s->x-=g->speed*fps_factor;
		if(s->x<=g->destx) {
			g->wait=50;
			g->state=1;
		}
		break;
	case 1:
		g->wait-=fps_factor;
		if(g->wait<=0) {
			g->state=2;
		}
		break;
	case 2:
		//sur=sprite_getcurrimg(s);
		//parsys_add(sur,s->w,1,s->x,s->y,20,0,0,100,LINESPLIT,NULL);
		//SDL_FreeSurface(sur);
		s->type=-1;
	}
}
