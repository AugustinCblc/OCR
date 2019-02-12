/******************************************************************************
                            D E C L A R A T I O N S
 ******************************************************************************/


#include <stdlib.h>
#include <stdio.h>

#define LO            -1
#define HI            +1
#define BIAS           1

#define sqr(x)        ((x)*(x))
#define MIN(x,y)      ((x)<(y) ? (x) : (y))
#define MAX(x,y)      ((x)>(y) ? (x) : (y))

typedef struct {                     /* A LAYER OF A NET:                     */
        int           Units;         /* - number of units in this layer       */
        double*         Activation;    /* - activation of ith unit              */
        int*          Output;        /* - output of ith unit                  */
        double*         Error;         /* - Error term of ith unit              */
        double**        Weight;        /* - connection weights to ith unit      */
} LAYER;

typedef struct {                     /* A NET:                                */
        LAYER*        InputLayer;    /* - input layer                         */
        LAYER*        OutputLayer;   /* - output layer                        */
        double          Eta;           /* - learning rate                       */
        double          Error;         /* - total net Error                     */
        double          Epsilon;       /* - net Error to terminate training     */
} NET;


/******************************************************************************
        R A N D O M S   D R A W N   F R O M   D I S T R I B U T I O N S
 ******************************************************************************/


void Initializerandoms()
{
  srand(4711);
}


int RandomEqualint(int Low, int High)
{
  return rand() % (High-Low+1) + Low;
}      


double RandomEqualdouble(double Low, double High)
{
  return ((double) rand() / RAND_MAX) * (High-Low) + Low;
}      


/******************************************************************************
               A P P L I C A T I O N - S P E C I F I C   C O D E
 ******************************************************************************/

#define NUM_DATA      32
#define X             16
#define Y             16

#define N             (X * Y)
#define M             58

 char                   Pattern[NUM_DATA][Y][X]={ { "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0111000000000000",
                                                    "1000100000000000",
                                                    "0001100000000000",
													"0110100000000000",
													"1000100000000000",
													"1001100000000000",
													"1110110000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000" },

                                                  { "0100000000000000",
                                                    "1100000000000000",
                                                    "0100000000000000",
                                                    "0100000000000000",
                                                    "0101100000000000",
                                                    "0110010000000000",
                                                    "0100001000000000",
													"0100001000000000",
													"0100001000000000",
													"0100010000000000",
													"0011100000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0111000000000000",
                                                    "1000100000000000",
                                                    "1000000000000000",
													"1000000000000000",
													"1000000000000000",
													"1000100000000000",
													"0111000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0000010000000000",
                                                    "0000110000000000",
                                                    "0000010000000000",
                                                    "0000010000000000",
                                                    "0011010000000000",
                                                    "0100110000000000",
                                                    "1000010000000000",
													"1000010000000000",
													"1000010000000000",
													"0100111000000000",
													"0011010000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0111000000000000",
                                                    "1000100000000000",
                                                    "1111100000000000",
													"1000000000000000",
													"1000000000000000",
													"1000100000000000",
													"0111000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0111000000000000",
                                                    "1000100000000000",
                                                    "1000000000000000",
                                                    "1000000000000000",
                                                    "1110000000000000",
                                                    "1000000000000000",
                                                    "1000000000000000",
													"1000000000000000",
													"1000000000000000",
													"1000000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000011111000",
                                                    "0000000100010000",
                                                    "0000000100010000",
													"0000000100010000",
													"0000000011100000",
													"0000000100000000",
													"0000000111110000",
													"0000001000001000",
													"0000001000001000",
													"0000000111110000",
													"0000000000000000",
													"0000000000000000"},


                                                 { "0100000000000000",
                                                    "1100000000000000",
                                                    "0100000000000000",
                                                    "0100000000000000",
                                                    "0101100000000000",
                                                    "0110010000000000",
                                                    "0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"1110111000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0100000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0100000000000000",
                                                    "1100000000000000",
                                                    "0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},

                                                  { "0000000000000000",
                                                    "0010000000000000",
                                                    "0000000000000000",
                                                    "0000000000000000",
                                                    "0010000000000000",
                                                    "0010000000000000",
                                                    "0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"1010000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0010000000000000",
													"0010000010000000",
													"0010000100000000",
													"0010001000000000",
													"0010010000000000",
													"0010100000000000",
													"0011000000000000",
													"0010100000000000",
													"0010010000000000",
													"0010001000000000",
													"0010000100000000",
													"0010000010000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
													
												  { "0100000000000000",
													"1100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0101100110000000",
													"1110011001000000",
													"0100010001000000",
													"0100010001000000",
													"0100010001000000",
													"0100010001000000",
													"1110111011100000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0101100000000000",
													"1110010000000000",
													"0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"1110111000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0011000000000000",
													"0100100000000000",
													"1000010000000000",
													"1000010000000000",
													"1000010000000000",
													"0100100000000000",
													"0011000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0101100000000000",
													"1110010000000000",
													"0100001000000000",
													"0100001000000000",
													"0100001000000000",
													"0110010000000000",
													"0101100000000000",
													"0100000000000000",
													"0100000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0011010000000000",
													"0100110000000000",
													"1000010000000000",
													"1000010000000000",
													"1000010000000000",
													"1000110000000000",
													"0111010000000000",
													"0000010000000000",
													"0000010000000000",
													"0000111000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0101100000000000",
													"1110000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0111000000000000",
													"1001000000000000",
													"1000000000000000",
													"0110000000000000",
													"0001000000000000",
													"1001000000000000",
													"1110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0100000000000000",
													"0100000000000000",
													"1111000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"0101000000000000",
													"0110000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1100110000000000",
													"0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"0100010000000000",
													"0100111000000000",
													"0011010000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1110111000000000",
													"0100010000000000",
													"0100010000000000",
													"0010100000000000",
													"0010100000000000",
													"0010100000000000",
													"0001000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1100011000000000",
													"1000001000000000",
													"1000001000000000",
													"1001001000000000",
													"1010101000000000",
													"1100011000000000",
													"1110111000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1110111000000000",
													"0100010000000000",
													"0010100000000000",
													"0001000000000000",
													"0010100000000000",
													"0100010000000000",
													"1110111000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
													
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1110111000000000",
													"0100010000000000",
													"0100010000000000",
													"0010100000000000",
													"0010100000000000",
													"0001100000000000",
													"0001000000000000",
													"0001000000000000",
													"0010000000000000",
													"1100000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1111110000000000",
													"0000100000000000",
													"0001000000000000",
													"0010000000000000",
													"0100000000000000",
													"1000000000000000",
													"1111110000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "1111110000000000",
													"0100001000000000",
													"0100000100000000",
													"0100000010000000",
													"0100000010000000",
													"0100000010000000",
													"0100000010000000",
													"0100000010000000",
													"0100000100000000",
													"0100001000000000",
													"1111110000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "1111100000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000000000000",
													"0010000010000000",
													"0010000100000000",
													"1111111100000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "1111100000000000",
													"0100010000000000",
													"0100001000000000",
													"0100001000000000",
													"0100001000000000",
													"0100010000000000",
													"0111100000000000",
													"0100000000000000",
													"0100000000000000",
													"0100000000000000",
													"1111000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "1000000000000000",
													"1000000000000000",
													"1000000000000000",
													"1000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000"},
													
												  { "0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000",
													"1100000000000000",
													"0100000000000000",
													"1000000000000000",
													"0000000000000000",
													"0000000000000000",
													"0000000000000000" }};
													

/*#define NUM_DATA      26
#define X             5
#define Y             7

#define N             (X * Y)
#define M             10


char                  Pattern[NUM_DATA][Y][X] = { { " OOO ",
                                                    "O   O",
                                                    "O   O",
                                                    "OOOOO",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O"  },

                                                  { "OOO  ",
                                                    "O  O ",
                                                    "O  O ",
                                                    "OOO  ",
                                                    "O   O",
                                                    "O   O",
                                                    "OOO  "  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "O    ",
                                                    "O    ",
                                                    "O    ",
                                                    "O   O",
                                                    " OOO "  },

                                                  { "OOOO ",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "OOOO "  },

                                                  { "OOOOO",
                                                    "O    ",
                                                    "O    ",
                                                    "OOOOO",
                                                    "O    ",
                                                    "O    ",
                                                    "OOOOO"  },

                                                  { "OOOOO",
                                                    "O    ",
                                                    "O    ",
                                                    "OOO  ",
                                                    "O    ",
                                                    "O    ",
                                                    "O    "  },

                                                  { " OOOO",
                                                    "O    ",
                                                    "O    ",
                                                    "O  OO",
                                                    "O   O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "OOOOO",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O"  },

                                                  { "OOOOO",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "OOOOO"  },

                                                  { "OOOOO",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "OO   "  } };*/

int                   Input [NUM_DATA][N];
int                   Output[NUM_DATA][M] =
                      
                                 {  {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
                                    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
                                    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
								    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
                                    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
                                    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO},
								    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO},
								    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI},
									{LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
								    {LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
									{HI, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO},
								    {LO, HI, LO, LO, LO, LO, LO, LO, LO, LO, LO,LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO, LO}};

FILE*                 f;
FILE*				  texte;
FILE*				  out;


void InitializeApplication(NET* Net)
{
  int n,i,j;

  Net->Eta     = 0.001;
  Net->Epsilon = 0.0001;
  for (n=0; n<NUM_DATA; n++) {
    for (i=0; i<Y; i++) {
      for (j=0; j<X; j++) {
        Input[n][i*X+j] = (Pattern[n][i][j] == '1') ? HI : LO;
      }
    }
  }
  f = fopen("Out.txt", "w");
}


void WriteInput(NET* Net, int* Input)
{
  Net = Net;
  int i;
   
  for (i=0; i<N; i++) {
    if (i%X == 0) {
      fprintf(f, "\n");
    }
    fprintf(f, "%c", (Input[i] == HI) ? '1' : '0');
  }
  fprintf(f, " -> ");
}


void WriteOutput(NET* Net, int* Output)
{
  Net = Net;
  int i;
  int Count, Index;
   
  Count = 0;
  for (i=0; i<M; i++) {
    if (Output[i] == HI) {
      Count++;
      Index = i;
    }
  }
  if (Count == 1)
  {
	if(Index == 0)
		 fprintf(f, "%c\n", '.');
	if(Index == 1)
		fprintf(f, "%c\n", ',');
	else 
		if(Index > 1)
		 fprintf(f, "%c\n", (char)(Index + 65));
  }	  
  else
    fprintf(f, "%s\n", "invalid");
}

int cmpWait(char mat1[16][16], char mat2[16][16], int *p)
{
	int diff = 0;
	int preci = 90;
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			if(mat1[i][j] != mat2[i][j])
			{
				diff++;
				if(diff >= 16*16 - 16*16/100*preci)
					return 0;
			}
		}
	}
	if(*p < diff)
		return 1;
	else
		return 0;
}

void FinalizeApplication(NET* Net)
{
  Net = Net;
  fclose(f);
}


/******************************************************************************
                          I N I T I A L I Z A T I O N
 ******************************************************************************/


void GenerateNetwork(NET* Net)
{
  int i;

  Net->InputLayer  = (LAYER*) malloc(sizeof(LAYER));
  Net->OutputLayer = (LAYER*) malloc(sizeof(LAYER));

  Net->InputLayer->Units       = N;
  Net->InputLayer->Output      = (int*)   calloc(N+1, sizeof(int));
  Net->InputLayer->Output[0]   = BIAS;

  Net->OutputLayer->Units      = M;
  Net->OutputLayer->Activation = (double*)  calloc(M+1, sizeof(double));
  Net->OutputLayer->Output     = (int*)   calloc(M+1, sizeof(int));
  Net->OutputLayer->Error      = (double*)  calloc(M+1, sizeof(double));
  Net->OutputLayer->Weight     = (double**) calloc(M+1, sizeof(double*));
      
  for (i=1; i<=M; i++) {
    Net->OutputLayer->Weight[i] = (double*) calloc(N+1, sizeof(double));
  }

  Net->Eta     = 0.1;
  Net->Epsilon = 0.01;
}


void randomWeights(NET* Net)
{
  int i,j;
   
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    for (j=0; j<=Net->InputLayer->Units; j++) {
      Net->OutputLayer->Weight[i][j] = RandomEqualdouble(-0.5, 0.5);
    }
  }
}


void SetInput(NET* Net, int* Input, int Protocoling)
{
  int i;
   
  for (i=1; i<=Net->InputLayer->Units; i++) {
    Net->InputLayer->Output[i] = Input[i-1];
  }
  if (Protocoling) {
    WriteInput(Net, Input);
  }
}


void GetOutput(NET* Net, int* Output, int Protocoling)
{
  int i;
   
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    Output[i-1] = Net->OutputLayer->Output[i];
  }
  if (Protocoling) {
    WriteOutput(Net, Output);
  }
}


/******************************************************************************
                     P R O P A G A T I N G   S I G N A L S
 ******************************************************************************/


void PropagateNet(NET* Net)
{
  int  i,j;
  double Sum;

  for (i=1; i<=Net->OutputLayer->Units; i++) {
    Sum = 0;
    for (j=0; j<=Net->InputLayer->Units; j++) {
      Sum += Net->OutputLayer->Weight[i][j] * Net->InputLayer->Output[j];
    }
    Net->OutputLayer->Activation[i] = Sum;
    if (Sum >= 0)
      Net->OutputLayer->Output[i] = HI;
    else
      Net->OutputLayer->Output[i] = LO;
  }
}


/******************************************************************************
                       A D J U S T I N G   W E I G H T S
 ******************************************************************************/


void ComputeOutputError(NET* Net, int* Target)
{
  int  i;
  double Err;
   
  Net->Error = 0;
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    Err = Target[i-1] - Net->OutputLayer->Activation[i];
    Net->OutputLayer->Error[i] = Err;
    Net->Error += 0.5 * sqr(Err);
  }
}


void AdjustWeights(NET* Net)
{
  int  i,j;
  int  Out;
  double Err;
   
  for (i=1; i<=Net->OutputLayer->Units; i++) {
    for (j=0; j<=Net->InputLayer->Units; j++) {
      Out = Net->InputLayer->Output[j];
      Err = Net->OutputLayer->Error[i];
      Net->OutputLayer->Weight[i][j] += Net->Eta * Err * Out;
    }
  }
}


/******************************************************************************
                      S I M U L A T I N G   T H E   N E T
 ******************************************************************************/


void SimulateNet(NET* Net, int* Input, int* Target, int Training, int Protocoling)
{
  int Output[M];
   
  SetInput(Net, Input, Protocoling);
  PropagateNet(Net);
  GetOutput(Net, Output, Protocoling);
   
  ComputeOutputError(Net, Target);
  if (Training)
    AdjustWeights(Net);
}


/******************************************************************************
                                    M A I N
 ******************************************************************************/


int main()
{
  NET  Net;
  double Error;
  int Stop;
  int  n,m;

  Initializerandoms();
  GenerateNetwork(&Net);
  randomWeights(&Net);
  InitializeApplication(&Net);
   
  do {
    Error = 0;
    Stop = 1;
    for (n=0; n<NUM_DATA; n++) {
      SimulateNet(&Net, Input[n], Output[n], 0, 0);
      Error = MAX(Error, Net.Error);
      Stop = Stop && (Net.Error < Net.Epsilon);
    }
    Error = MAX(Error, Net.Epsilon);
    printf("Training %0.0f%% completed ...\n", (Net.Epsilon / Error) * 100);
    if (! Stop) {
      for (m=0; m<10*NUM_DATA; m++) {
        n = RandomEqualint(0, NUM_DATA-1);      
        SimulateNet(&Net, Input[n], Output[n], 1, 0);
      }
    }
  } while (! Stop);
   
  for (n=0; n<NUM_DATA; n++) {
    SimulateNet(&Net, Input[n], Output[n], 0, 1);
  }
  texte = fopen("out/finalText.txt", "w");
  out = fopen("out/listchar.txt", "r");
  int c = fgetc(out);
  int c1 = fgetc(out);
	do
  {
	int b = 0;  
	char  mat[16][16];
	if((c == '0' || c == '1') && (c1 == '0' || c1 == '1'))
		b = 1;
		for(int i = 0; i < 16; i++)
		{
			for(int j = 0; j < 16; j++)
			{
				mat[i][j] = c;
				c = c1;
				c1 = fgetc(out);
			}
		}
		int *p = 0;
		if(b)
		{
			for(int i = 0; i < 55; i++)
			{
				if(cmpWait(Pattern[i], mat, p))
					fprintf(texte, "%c", (char)(i+65));
			}
		}
	c = c1;
	c1 = fgetc(out);
  }while(c1 != EOF);
  FinalizeApplication(&Net);
  return 0;
}
