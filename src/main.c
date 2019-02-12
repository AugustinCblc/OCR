//#include "../../bmpT.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)



//NOUVEAU
char *path;

struct BitMap
{
	unsigned short int Type;
	unsigned int Size;
	unsigned short int Reserved1, Reserved2;
	unsigned int Offset;
} Header;

struct BitMapInfo
{
	unsigned int Size;
	int Width, Height;
	unsigned short int Planes;
	unsigned short int Bits;
	unsigned int Compression;
	unsigned int ImageSize;
	int xResolution, yResolution;
	unsigned int Colors;
	unsigned int ImportantColors;
} InfoHeader;

struct Pixels
{
	unsigned char Blue,Green,Red;
};


struct s_file
{
	char donnee;
	struct s_file *suivant;
};


struct vect
{
	int pos_x;
	int pos_y;
	int width;
	int height;
	int police;

	char *text;
};

typedef struct vect *t_vect;

//-------------------------FUNCTIONS-------------------------------------------------
void d_horz_block(struct Pixels **tab,int pos_x,int pos_y,int L,int H,int success,t_vect *block_vector);
void d_vert_block(struct Pixels **tab,int pos_x,int pos_y,int L,int H,int success,t_vect *block_vector);
void d_lines(struct Pixels **tab,int H,int beginx,int endx,int beginy,int endy,char *text) ;
void d_char (struct Pixels **tab,int H,int l1,int l2,int begin,int end, char *text);


int width;
int height;

void bmpToArray(char *file, t_vect *block_vector)
{
	int i=0, j=0;
	int size_spix;
	int padding = 0;
	char temp[4];
	FILE *BMP_in = fopen (file, "rb");
	if (BMP_in == NULL) {
		printf("\nCannot open file %s\n", file);
		exit(1);
	}

	fread(&Header, sizeof(Header), 1, BMP_in);
	fread(&InfoHeader, sizeof(InfoHeader), 1, BMP_in);
	width = InfoHeader.Width;
	height = InfoHeader.Height;
	padding = InfoHeader.Width % 4;
	if(padding != 0 ) {
		padding = 4 - padding;
	}

	size_spix = sizeof(struct Pixels);
	struct Pixels **pixel_arrayp;

	pixel_arrayp = (struct Pixels **)calloc(InfoHeader.Height,sizeof(struct Pixel*));

	for(i=0;i<InfoHeader.Height; i++) {
		pixel_arrayp[i] = (struct Pixels *)calloc(InfoHeader.Width,size_spix);
	}

	//-----------------------------------IMG ---> GREY ----------------------------------
	for(i=0; i < (InfoHeader.Height)*1; i++) {
		for(j=0; j < InfoHeader.Width; j++) {

			fread(&pixel_arrayp[i][j], 3,1,  BMP_in);
			unsigned char k=(pixel_arrayp[i][j].Red+pixel_arrayp[i][j].Green+pixel_arrayp[i][j].Blue)/3;

			pixel_arrayp[i][j].Red = k;
			pixel_arrayp[i][j].Green = k;
			pixel_arrayp[i][j].Blue = k;
		}
		if(padding != 0) {
			fread(&temp, padding, 1,  BMP_in);
		}
	}
	// ----------------------------- SAVE IN GREY --------------------------------------

	FILE *BMP_out = fopen("out/1saveGrey.bmp", "wb");
	if (BMP_out == NULL) {
		exit(1);
	}


	fwrite(&Header, sizeof(Header), 1,  BMP_out);
	fwrite(&InfoHeader, sizeof(InfoHeader), 1, BMP_out);


	for(i=0; i < InfoHeader.Height ; i++) {
		for(j=0; j < InfoHeader.Width; j++) {
			fwrite(&pixel_arrayp[i][j], size_spix, 1, BMP_out);
		}
		if(padding != 0) {

			fwrite(&temp, padding, 1,BMP_out);
		}
	}
	printf("\n\nSAVE IN GREY  ===>  DONE !!\n");


	fclose(BMP_out);

	// ------------------------------ IMG ---> B & W  -------------------------------------
	for(i=0; i < InfoHeader.Height; i++) {
		for(j=0; j < InfoHeader.Width; j++) {

			fread(&pixel_arrayp[i][j], 3,1,  BMP_in);

			if (pixel_arrayp[i][j].Red > 160){
				pixel_arrayp[i][j].Red = 255;
				pixel_arrayp[i][j].Green = 255;
				pixel_arrayp[i][j].Blue = 255;
			}
			if (pixel_arrayp[i][j].Red <= 160){
				pixel_arrayp[i][j].Red = 0;
				pixel_arrayp[i][j].Green = 0;
				pixel_arrayp[i][j].Blue = 0;
			}
		}
		if(padding != 0) {
			fread(&temp, padding, 1,  BMP_in);
		}
	}

	fclose(BMP_in);

	//-------------------------------- SAVE IN B&W -------------------------------------------
	FILE *BMP_out2 = fopen("out/2saveBW.bmp", "wb");
	if (BMP_out2 == NULL) {
		exit(1);
	}


	fwrite(&Header, sizeof(Header), 1,  BMP_out2);
	fwrite(&InfoHeader, sizeof(InfoHeader), 1, BMP_out2);


	for(i=0; i < InfoHeader.Height ; i++) {
		for(j=0; j < InfoHeader.Width; j++) {
			fwrite(&pixel_arrayp[i][j], size_spix, 1, BMP_out2);
		}
		if(padding != 0) {

			fwrite(&temp, padding, 1,BMP_out2);
		}
	}
	printf("\n\nSAVE IN BLACK & WHITE ===>  DONE !!\n\n");


	fclose(BMP_out2);

	//------------------------------- IMG CUTTING --------------------------------------------

	struct Pixels **t;
	t = (struct Pixels **)calloc(InfoHeader.Height,sizeof(struct Pixel*));
	for(i=0;i<InfoHeader.Height; i++) {
		t[i] = (struct Pixels *)calloc(InfoHeader.Width,size_spix);
	}
	int c = height-1;
	for(i=0; i<height; i++)
	{

		for(j=0;j<width;j++)
		{
			t[i][j]=pixel_arrayp[c][j];
		}
		c--;
	}



	d_horz_block(t,1,1,InfoHeader.Width-1,InfoHeader.Height-1,1,block_vector);

	c = height-1;
	for(i=0; i<height; i++)
	{

		for(j=0;j<width;j++)
		{
			pixel_arrayp[i][j] = t[c][j];
		}
		c--;
	}



	//------------------------------ SAVE IN CUTTING ----------------------------------------

	FILE *BMP_out3 = fopen("out/3saveCut.bmp", "wb");
	if (BMP_out3 == NULL) {
		exit(1);
	}


	fwrite(&Header, sizeof(Header), 1,  BMP_out3);
	fwrite(&InfoHeader, sizeof(InfoHeader), 1, BMP_out3);


	for(i=0; i < InfoHeader.Height ; i++) {
		for(j=0; j < InfoHeader.Width; j++) {
			fwrite(&pixel_arrayp[i][j], size_spix, 1, BMP_out3);
		}
		if(padding != 0) {

			fwrite(&temp, padding, 1,BMP_out3);

		}
	}
	printf("\n\nIMAGE CUTTING ===>  DONE !!\n\n");


	fclose(BMP_out3);

	//return pixel_arrayp;
}


//--------------------------CHAR------------------------------------------------------

void d_char (struct Pixels **tab,int H,int l1,int l2,int begin,int end, char *text)
{
	int i,j, black, pre_black, bgn_blc, bgn_black, m, n;
	char *text2, *text3, *text4, *text5;
	FILE   *out;
	H = H *1;

	out = fopen("out/listChar.txt", "a");
	text2 = malloc(sizeof(char)*3);

	text2 = "x";

	text3 = malloc(sizeof(char)*3);

	text3 = " ";

	text4 = malloc(sizeof(char)*3);

	text4 = "\n";

	text5 = malloc(sizeof(char)*3);

	text5 = "\t";
	black=0;
	pre_black=0;
	bgn_black=begin;
	bgn_blc=begin;
	for (j=begin; j< end; j++)
	{
		pre_black=black;
		black = 0;
		for (i=l1; i< l2;i++)
		{
			if (tab[i][j].Red==0)
				black=1;
		}

		if ((pre_black == 0) && (black == 1))
		{
			bgn_black=j;
			i=bgn_blc;
			while(i < j-48)
			{

				strcat(text,text5);
				i = i+48;
			}

			while(i < j-6)
			{
				strcat(text,text3);
				i = i+6;
			}

			for (i=l1; i< l2; i++)
			{
				tab[i][j-1].Green=0;
				tab[i][j-1].Blue=252;
			}
			strcat(text, text2);

		}

		if ((pre_black == 1) && (black == 0))
		{

			fprintf(out, "%i %i ", (j-bgn_black), (l2-l1));
			for(m=l1; m<l2; m++)

			{
				for(n=bgn_black; n<j; n++)
				{
					if ((tab[m][n].Red)==0)
					{
						fprintf(out, "1");
					}
					else
						fprintf(out, "0");
				}
			}
			fprintf(out, "\n");
			bgn_blc=j;
			for (i=l1; i< l2; i++)
			{
				tab[i][j].Green=252;
				tab[i][j].Blue=45;
			}
		}
	}
	strcat(text,text4);
	fclose(out);
}



//------------------------LINES-------------------------------------------------------

void d_lines(struct Pixels **tab,int H,int beginx,int endx,int beginy,int endy,char *text)
{
	int *res;
	int i, j, start_line_h;

	res = malloc(sizeof(int*) * H);

	for(i = beginy; i < endy; i++)
	{
		res[i] = 0;
		for (j = beginx; j < endx; j++)
			if (tab[i][j].Red==0)
				res[i] = 1;
	}
	start_line_h = beginy;
	for (i = 1+beginy; i < endy; i++)
	{

		if ((res[i-1]==0)&&(res[i]==1))
			start_line_h = i;

		if ((res[i-1]==1)&&(res[i]==0) && (i >= start_line_h + 5))
		{
			for (j = beginx ; j < endx; j++)
			{
				tab[start_line_h-1][j].Green=0;
				tab[start_line_h-1][j].Blue=254;
			}
			for (j = beginx ; j < endx; j++)
			{
				tab[i][j].Green = 0;
				tab[i][j].Blue = 253;
			}
			d_char (tab, height, start_line_h, i, beginx, endx,text);
		}
	}

	for (j = beginx ; j < endx; j++)
	{
		tab[start_line_h-1][j].Green=0;
		tab[start_line_h-1][j].Blue=254;

	}
}



//--------------------------VERTICAL BLOCKS------------------------------------------

void d_vert_block(struct Pixels **tab,int pos_x,int pos_y,int L,int H,int success,t_vect *block_vector)
{
	int *res;
	int i,j, max_blc, max_id, max, l, n;

	char *text;

	res = malloc(sizeof(int*)*(L));
	max_blc = 0;

	for(i = pos_y; i < L; i++)
	{
		res[i] = 0;
		for (j = pos_y; j < H; j++)
			if (tab[i][j].Red==0)
				res[i] = 1;
	}
	max = 0;
	max_id = 0;
	for (i = pos_x; i<L; i++)
	{
		if (res[i] == 0)
			max = max + 1;
		else
		{
			if (max_blc < max)
			{
				max_id = i;
				max_blc = max;
			}

			max = 0;
		}
	}
	if ((max_blc > 20) && (max_blc < 150))
	{
		l = (max_id-(max_blc / 2));
		for (j = pos_y ; j < H; j++)
		{
			tab[i][j].Green=50;
			tab[i][j].Blue=120;
		}
		d_horz_block(tab, pos_x, pos_y, l, H, 1, block_vector);
		d_horz_block(tab, l, pos_y, L, H, 1, block_vector);
	}
	else
	{
		if (success == 0)
		{
			n = 0;


			while(block_vector[n])
			{
				n++;
			}

			text = malloc(sizeof(char)*5000);
			block_vector[n] = malloc(sizeof(struct vect));

			block_vector[n]->pos_x = pos_x;
			block_vector[n]->pos_y = pos_y;
			block_vector[n]->height = H - pos_y;
			block_vector[n]->width = L - pos_x;
			text[0] = '\0';

			d_lines(tab,height, pos_x,L, pos_y, H,text);
			block_vector[n]->text = text;


		}
		else
			d_horz_block(tab, pos_x, pos_y, L,
					H, 0,
					block_vector);
	}
}




//-----------------------------HORIZONTAL BLOCKS-------------------------------------


void d_horz_block(struct Pixels **tab,int pos_x,int pos_y,int L,int H,int success,t_vect *block_vector)
{
	int *res;
	int i,j, max_blc, max_id, max, h, n;


	char *text;

	res = malloc(sizeof(int*)*(height));

	max_blc = 0;

	for(i = pos_y; i < H; i++)
	{
		res[i] = 0;
		for (j = pos_x; j < L; j++)
			if (tab[i][j].Red ==0)
				res[i] = 1;
	}
	max = 0;
	max_id = 0;
	for (i = pos_y; i<H; i++)
	{
		if (res[i] == 0)
		{
			max = max + 1;

		}
		else
		{
			if (max_blc < max)
			{
				max_id = i;
				max_blc = max;
			}

			max = 0;
		}
	}
	if ((max_blc > 20) && (max_blc < 200))
	{
		h = (max_id-(max_blc / 2));
		for (j = pos_x ; j < L; j++)
		{
			tab[h][j].Green = 50;
			tab[h][j].Blue = 120;
		}
		d_vert_block(tab, pos_x, pos_y, L, (h), 1, block_vector);
		d_vert_block(tab, pos_x, (h), L, H, 1, block_vector);
	}
	else
	{
		if (success == 0)
		{

			n = 0;

			while(block_vector[n])
			{
				n++;
			}
			text = malloc(sizeof(char)*5000) ;

			block_vector[n] = malloc(sizeof(struct vect));

			block_vector[n]->pos_x = pos_x;
			block_vector[n]->pos_y = pos_y;
			block_vector[n]->height = H - pos_y;
			block_vector[n]->width = L - pos_x;

			text[0] = '\0';
			d_lines(tab,height, pos_x, L, pos_y, H,text);

			block_vector[n]->text = text;

		}
		else
		{

			d_vert_block(tab, pos_x, pos_y,L, H, 0,block_vector);
		}
	}

}
void toTxt(t_vect *vect_bloc)
{
	FILE   *txt;
	int i;


	txt = fopen("out/model.txt", "w+");

	i = 0;

	while (vect_bloc[i] != NULL)
	{


		fprintf(txt, "%s", vect_bloc[i]->text);
		fprintf(txt, "\n");

		i++;
	}
	fclose(txt);
	printf("**rendu txt         [OK]\n");
}

void output()
{
	FILE *letters;
	letters = fopen("out/listChar.txt","r");
	FILE *model;
	model = fopen("out/model.txt","r+");
	FILE *out;
	out = fopen("out/out.txt","w");
	int carLetters = 0;
	int carOut = 0;
	int cpt = 0;
	if(letters != NULL){
		while (cpt != 334)
		{
			carOut = fgetc(model);
			if(carOut == 'x')
			{
				carLetters = fgetc(letters);
				fprintf(out,"%c",carLetters);
				cpt++;
			}
			else
			{
				fprintf(out,"%c",carOut);
			}
		}
	}
	fclose(letters);
	fclose(out);
	fclose(model);
}
//GtkWidget *g_lbl_hello;

int main(int argc, char *argv[])
{
	GtkBuilder      *builder; 
	GtkWidget       *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);
	//	g_lbl_hello = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hello")); 
	g_object_unref(builder);

	gtk_widget_show(window);                
	gtk_main();
/*	int n;
	t_vect block_vector[1000];
	for (n=0; n<999; n++)
	{
		block_vector[n] = NULL;
	}

	bmpToArray("test6.bmp",block_vector);
	toTxt(block_vector);
*/
	return 0;
}
void on_image_file_set(GtkFileChooser *filechooserbutton1, GtkWidget *img)
{
	//char *cNome;
	/*cNome*/path =gtk_file_chooser_get_preview_filename(filechooserbutton1);

	printf("\n%s", /*cNome*/ path);
	gtk_image_set_from_file(GTK_IMAGE(img), /*cNome*/ path);	

}
void on_start_clicked()
{
	int n;
	t_vect block_vector[1000];
	for (n=0; n<999; n++)
	{
		block_vector[n] = NULL;
	}

	bmpToArray("image.bmp",block_vector);
	toTxt(block_vector);

} 
// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}
