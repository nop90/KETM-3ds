#include "main.h"
#include <3ds.h>

extern SDL_Surface *screen;
extern int keyboard[];

GAMESTATE state;
GAMESTATE laststate;

int score;
int lives;
int volume=2; /* 0-3 */
int difficulty=DIFF_EASY;

char mods[20][20];
int modcount=0;

void wait(){
        int i,j;
        for (i=0; i<10000; i++) {
                for(j=0; j<2000; j++)
			j++;
	}
}


void readModDirs( ) {
/*
	fprintf(stdout,"readDataDirs\n");
  DIR *dp;
  struct dirent *dir;
  if ( (dp = opendir(".")) == NULL ) {
    fprintf(stderr, "Can't open directory: ");
    exit(1);
  }
  while ((dir = readdir(dp)) != NULL) {
	if (dir->d_name[0]!='_') continue;
    fprintf(stdout,"%s\n", dir->d_name);
    strcpy(mods[modcount],dir->d_name);
    modcount++;
  }
  closedir(dp);
*/
    strcpy(mods[0],"KETM-episode1");
    strcpy(mods[1],"1941");
	strcpy(mods[2],"Star Wars");
    strcpy(mods[3],"Iron Man");
    modcount=4;
}


void chooseModDir() {
	int i;
	int selected=0;
	int done = 0;
    u32 keys;
	memset(moddir,0,20);

	readModDirs();
	while (!done) {
		consoleClear();
printf("\n \
 _  __ ___  _____  __  __ \n \
| |/ /| __||_   _||  \\/  | \n \
| ' < | _|   | |  | |\\/| | \n \
|_|\\_\\|___|  |_|  |_|  |_| \n\n"); 

		printf("\t\t version 0.8\n\n\n");
		printf("Please choose the mod:\n\n");
		for (i=0; i<modcount; i++) {
			if (i==selected)
				printf(">>> ");
			else
				printf("    ");
			printf("%s\n",mods[i]);		


		}
		while (aptMainLoop()) {
           hidScanInput();
           keys = hidKeysDown();
			if (keys & KEY_UP){
				if (selected>0) selected--;
				break;

			}
			if (keys & KEY_DOWN){
				if (selected<modcount-1) selected++;
				break;

			}
			
			 if (keys & KEY_A){ 
				sprintf(moddir,"romfs:/%s",mods[selected]);
	//			strcpy(moddir,mods[selected]);
				done=1;
				break;
			}
		}
		wait();
	}

}


int main(int argc, char *argv[])
{
//	strcpy(moddir,"romfs:/_KETM-episode1"); //default
	 gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);
	printf("Starting K.E.T.M.\n");

	chooseModDir(); 
	romfsInit();

	game_init(argc, argv);
	playMusic("intro");
	while (state.mainstate!=ST_GAME_QUIT) {
		keyboard_poll();

		switch(state.mainstate) {
			case ST_START_INTRO:
				if(state.newstate) {
					startintro_init();
					state.newstate=0;
				}
				startintro_work();
				break;
			case ST_INTRO:
				if(state.newstate) {
					intro_init();
					state.newstate=0;
				}
				intro_work();
				break;
			case ST_GAME_PLAY:
				if(state.newstate) {
					playChunk(1);
					playMusic("stage1");
					thegame_init();
					state.newstate=0;
				}
				thegame_work();
				break;
			case ST_GAME_OVER:
				if(state.newstate) {
					playChunk(4);
					stopMusic();
					gameover_init();
					playMusic("intro");
					state.newstate=0;
				}
				gameover_work();
                                //newstate(ST_INTRO,0,1);
				break;
			case ST_MENU:
				if(state.newstate) {
					menu_init();
					state.newstate=0;
				}
				menu_work();
				break;
			case ST_SHOW_HCLIST:
				if(state.newstate) {
					hsc_show_init();
					state.newstate=0;
				}
				hsc_show_work();
				break;
			case ST_ENTRY_HCLIST:
				if(state.newstate) {
					hsc_entry_init();
					state.newstate=0;
				}
				hsc_entry_work();
				break;
		}
		
		fps_show();
		SDL_Flip(screen);
		fps_newframe();
	}

	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */

	fprintf(stdout,"Thank you for playing\n");
	romfsExit();
	exit(0);
}
