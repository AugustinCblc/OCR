# include <SDL.h>
# include <SDL_image.h>
# include <assert.h>
# include <err.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>


void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}


void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}



SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}



SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}


int main(int argc, char *argv[])
{
	if(argc > 0)
	{
	init_sdl();
	SDL_Surface *image = load_image(argv[1]);
	SDL_Surface *image2 = display_image(image);
	SDL_FreeSurface(image2);
	/*for(int j = 0; j < image->w; j++)
	{
		for(int k = 0; k < image->h; k++)
		{
		Uint32 pixel = getpixel(image, (unsigned)j, (unsigned)k);
		Uint8 r;
		Uint8 g;
		Uint8 b;
		SDL_GetRGB(pixel, image->format, &r, &g, &b);
		Uint8 lol = 0.3*r+0.59*g+0.11*b;
		pixel = SDL_MapRGB(image->format, lol, lol, lol);
		putpixel(image, (unsigned)j, (unsigned)k, pixel);
		}
	}*/
	image2 = display_image(image);
	SDL_FreeSurface(image);
	SDL_FreeSurface(image2);
	}
	return 0;
}
