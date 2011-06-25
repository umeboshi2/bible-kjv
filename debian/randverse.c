/* 
	Produce random bible verses
					*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

typedef struct
{
  char* t;
  int n;
  int *p;
} book ;

int main(int argc, char **argv)
{
  int bkno, chno, vno, i;
  int tv;
  char cmd[32], opt[32] = "";
  struct timeval t;
  long int ii;

  int genesis[50] = {31, 25, 24, 26, 32, 22, 24, 22, 29, 32, 32, 20, 18, 24, 21, 16, 27, 33, 38, 18, 34, 24, 20, 67, 34, 35, 46, 22, 35, 43, 55, 32, 20, 31, 29, 43, 36, 30, 23, 23, 57, 38, 34, 34, 28, 34, 31, 22, 33, 26};
  
  int exodus[40] = {22, 25, 22, 31, 23, 30, 25, 32, 35, 29, 10, 51, 22, 31, 27, 36, 16, 27, 25, 26, 36, 31, 33, 18, 40, 37, 21, 43, 46, 38, 18, 35, 23, 35, 35, 38, 29, 31, 43, 38};
  
  int leviticus[27] = {17, 16, 17, 35, 19, 30, 38, 36, 24, 20, 47, 8, 59, 57, 33, 34, 16, 30, 37, 27, 24, 33, 44, 23, 55, 46, 34};
  
  int numbers[36] = {54, 34, 51, 49, 31, 27, 89, 26, 23, 36, 35, 16, 33, 45, 41, 50, 13, 32, 22, 29, 35, 41, 30, 25, 18, 65, 23, 31, 40, 16, 54, 42, 56, 29, 34, 13};
  
  int deuteronomy[34] = {46, 37, 29, 49, 33, 25, 26, 20, 29, 22, 32, 32, 18, 29, 23, 22, 20, 22, 21, 20, 23, 30, 25, 22, 19, 19, 26, 68, 29, 20, 30, 52, 29, 12};
  
  int joshua[24] = {18, 24, 17, 24, 15, 27, 26, 35, 27, 43, 23, 24, 33, 15, 63, 10, 18, 28, 51, 9, 45, 34, 16, 33};
  
  int judges[21] = {36, 23, 31, 24, 31, 40, 25, 35, 57, 18, 40, 15, 25, 20, 20, 31, 13, 31, 30, 48, 25};
  
  int ruth[4] = {22, 23, 18, 22};
  
  int samuel1[31] = {28, 36, 21, 22, 12, 21, 17, 22, 27, 27, 15, 25, 23, 52, 35, 23, 58, 30, 24, 42, 15, 23, 29, 22, 44, 25, 12, 25, 11, 31, 13};
  
  int samuel2[24] = {27, 32, 39, 12, 25, 23, 29, 18, 13, 19, 27, 31, 39, 33, 37, 23, 29, 33, 43, 26, 22, 51, 39, 25};
  
  int kings1[22] = {53, 46, 28, 34, 18, 38, 51, 66, 28, 29, 43, 33, 34, 31, 34, 34, 24, 46, 21, 43, 29, 53};
  
  int kings2[25] = {18, 25, 27, 44, 27, 33, 20, 29, 37, 36, 21, 21, 25, 29, 38, 20, 41, 37, 37, 21, 26, 20, 37, 20, 30};
  
  int chronicles1[29] = {54, 55, 24, 43, 26, 81, 40, 40, 44, 14, 47, 40, 14, 17, 29, 43, 27, 17, 19, 8, 30, 19, 32, 31, 31, 32, 34, 21, 30};
  
  int chronicles2[36] = {17, 18, 17, 22, 14, 42, 22, 18, 31, 19, 23, 16, 22, 15, 19, 14, 19, 34, 11, 37, 20, 12, 21, 27, 28, 23, 9, 27, 36, 27, 21, 33, 25, 33, 27, 23};
  
  int ezra[10] = {11, 70, 13, 24, 17, 22, 28, 36, 15, 44};
  
  int nehemiah[13] = {11, 20, 32, 23, 19, 19, 73, 18, 38, 39, 36, 47, 31};
  
  int esther[10] = {22, 23, 15, 17, 14, 14, 10, 17, 32, 3};
  
  int job[42] = {22, 13, 26, 21, 27, 30, 21, 22, 35, 22, 20, 25, 28, 22, 35, 22, 16, 21, 29, 29, 34, 30, 17, 25, 6, 14, 23, 28, 25, 31, 40, 22, 33, 37, 16, 33, 24, 41, 30, 24, 34, 17};
  
  int psalms[150] = {6, 12, 8, 8, 12, 10, 17, 9, 20, 18, 7, 8, 6, 7, 5, 11, 15, 50, 14, 9, 13, 31, 6, 10, 22, 12, 14, 9, 11, 12, 24, 11, 22, 22, 28, 12, 40, 22, 13, 17, 13, 11, 5, 26, 17, 11, 9, 14, 20, 23, 19, 9, 6, 7, 23, 13, 11, 11, 17, 12, 8, 12, 11, 10, 13, 20, 7, 35, 36, 5, 24, 20, 28, 23, 10, 12, 20, 72, 13, 19, 16, 8, 18, 12, 13, 17, 7, 18, 52, 17, 16, 15, 5, 23, 11, 13, 12, 9, 9, 5, 8, 28, 22, 35, 45, 48, 43, 13, 31, 7, 10, 10, 9, 8, 18, 19, 2, 29, 176, 7, 8, 9, 4, 8, 5, 6, 5, 6, 8, 8, 3, 18, 3, 3, 21, 26, 9, 8, 24, 13, 10, 7, 12, 15, 21, 10, 20, 14, 9};
  
  int proverbs[31] = {33, 22, 35, 27, 23, 35, 27, 36, 18, 32, 31, 28, 25, 35, 33, 33, 28, 24, 29, 30, 31, 29, 35, 34, 28, 28, 27, 28, 27, 33, 31};
  
  int ecclesiastes[12] = {18, 26, 22, 16, 20, 12, 29, 17, 18, 20, 10, 14};
  
  int solomon[8] = {17, 17, 11, 16, 16, 13, 13, 14};
  
  int isaiah[66] = {31, 22, 26, 6, 30, 13, 25, 22, 21, 34, 16, 6, 22, 32, 9, 14, 14, 7, 25, 6, 17, 25, 18, 23, 12, 21, 13, 29, 24, 33, 9, 20, 24, 17, 10, 22, 38, 22, 8, 31, 29, 25, 28, 28, 25, 13, 15, 22, 26, 11, 23, 15, 12, 17, 13, 12, 21, 14, 21, 22, 11, 12, 19, 12, 25, 24};
  
  int jeremiah[52] = {19, 37, 25, 31, 31, 30, 34, 22, 26, 25, 23, 17, 27, 22, 21, 21, 27, 23, 15, 18, 14, 30, 40, 10, 38, 24, 22, 17, 32, 24, 40, 44, 26, 22, 19, 32, 21, 28, 18, 16, 18, 22, 13, 30, 5, 28, 7, 47, 39, 46, 64, 34};
  
  int lamentations[5] = {22, 22, 66, 22, 22};
  
  int ezekiel[48] = {28, 10, 27, 17, 17, 14, 27, 18, 11, 22, 25, 28, 23, 23, 8, 63, 24, 32, 14, 49, 32, 31, 49, 27, 17, 21, 36, 26, 21, 26, 18, 32, 33, 31, 15, 38, 28, 23, 29, 49, 26, 20, 27, 31, 25, 24, 23, 35};
  
  int daniel[12] = {21, 49, 30, 37, 31, 28, 28, 27, 27, 21, 45, 13};
  
  int hosea[14] = {11, 23, 5, 19, 15, 11, 16, 14, 17, 15, 12, 14, 16, 9};
  
  int joel[3] = {20, 32, 21};
  
  int amos[9] = {15, 16, 15, 13, 27, 14, 17, 14, 15};
  
  int obadiah[1] = {21};
  
  int jonah[4] = {17, 10, 10, 11};
  
  int micah[7] = {16, 13, 12, 13, 15, 16, 20};
  
  int nahum[3] = {15, 13, 19};
  
  int habakkuk[3] = {17, 20, 19};
  
  int zephaniah[3] = {18, 15, 20};
  
  int haggai[2] = {15, 23};
  
  int zechariah[14] = {21, 13, 10, 14, 11, 15, 14, 23, 17, 12, 17, 14, 9, 21};
  
  int malachi[4] = {14, 17, 18, 6};
  
  int matthew[28] = {25, 23, 17, 25, 48, 34, 29, 34, 38, 42, 30, 50, 58, 36, 39, 28, 27, 35, 30, 34, 46, 46, 39, 51, 46, 75, 66, 20};
  
  int mark[16] = {45, 28, 35, 41, 43, 56, 37, 38, 50, 52, 33, 44, 37, 72, 47, 20};
  
  int luke[24] = {80, 52, 38, 44, 39, 49, 50, 56, 62, 42, 54, 59, 35, 35, 32, 31, 37, 43, 48, 47, 38, 71, 56, 53};
  
  int john[21] = {51, 25, 36, 54, 47, 71, 53, 59, 41, 42, 57, 50, 38, 31, 27, 33, 26, 40, 42, 31, 25};
  
  int acts[28] = {26, 47, 26, 37, 42, 15, 60, 40, 43, 48, 30, 25, 52, 28, 41, 40, 34, 28, 41, 38, 40, 30, 35, 27, 27, 32, 44, 31};
  
  int romans[16] = {32, 29, 31, 25, 21, 23, 25, 39, 33, 21, 36, 21, 14, 23, 33, 27};
  
  int corinthians1[16] = {31, 16, 23, 21, 13, 20, 40, 13, 27, 33, 34, 31, 13, 40, 58, 24};
  
  int corinthians2[13] = {24, 17, 18, 18, 21, 18, 16, 24, 15, 18, 33, 21, 14};
  
  int galatians[6] = {24, 21, 29, 31, 26, 18};
  
  int ephesians[6] = {23, 22, 21, 32, 33, 24};
  
  int philippians[4] = {30, 30, 21, 23};
  
  int colossians[4] = {29, 23, 25, 18};
  
  int thessalonians1[5] = {10, 20, 13, 18, 28};
  
  int thessalonians2[3] = {12, 17, 18};
  
  int timothy1[6] = {20, 15, 16, 16, 25, 21};
  
  int timothy2[4] = {18, 26, 17, 22};
  
  int titus[3] = {16, 15, 15};
  
  int philemon[1] = {25};
  
  int hebrews[13] = {14, 18, 19, 16, 14, 20, 28, 13, 28, 39, 40, 29, 25};
  
  int james[5] = {27, 26, 18, 17, 20};
  
  int peter1[5] = {25, 25, 22, 19, 14};
  
  int peter2[3] = {21, 22, 18};
  
  int john1[5] = {10, 29, 24, 21, 21};
  
  int john2[1] = {13};
  
  int john3[1] = {14};
  
  int jude[1] = {25};
  
  int revelation[22] = {20, 29, 22, 11, 14, 17, 17, 13, 21, 11, 19, 17, 18, 20, 8, 21, 18, 24, 21, 15, 27, 21};
  
  book books[66] = {
    {"Genesis", 50, genesis},
    {"Exodus", 40, exodus},
    {"Leviticus", 27, leviticus},
    {"Numbers", 36, numbers},
    {"Deuteronomy", 34, deuteronomy},
    {"Joshua", 24, joshua},
    {"Judges", 21, judges},
    {"Ruth", 4, ruth},
    {"1Samuel", 31, samuel1},
    {"2Samuel", 24, samuel2},
    {"1Kings", 22, kings1},
    {"2Kings", 25, kings2},
    {"1Chronicles", 29, chronicles1},
    {"2Chronicles", 36, chronicles2},
    {"Ezra", 10, ezra},
    {"Nehemiah", 13, nehemiah},
    {"Esther", 10, esther},
    {"Job", 42, job},
    {"Psalms", 150, psalms},
    {"Proverbs", 31, proverbs},
    {"Ecclesiastes", 12, ecclesiastes},
    {"Song", 8, solomon},
    {"Isaiah", 66, isaiah},
    {"Jeremiah", 52, jeremiah},
    {"Lamentations", 5, lamentations},
    {"Ezekiel", 48, ezekiel},
    {"Daniel", 12, daniel},
    {"Hosea", 14, hosea},
    {"Joel", 3, joel},
    {"Amos", 9, amos},
    {"Obadiah", 1, obadiah},
    {"Jonah", 4, jonah},
    {"Micah", 7, micah},
    {"Nahum", 3, nahum},
    {"Habakkuk", 3, habakkuk},
    {"Zephaniah", 3, zephaniah},
    {"Haggai", 2, haggai},
    {"Zechariah", 14, zechariah},
    {"Malachi", 4, malachi},
    {"Matthew", 28, matthew},
    {"Mark", 16, mark},
    {"Luke", 24, luke},
    {"John", 21, john},
    {"Acts", 28, acts},
    {"Romans", 16, romans},
    {"1Corinthians", 16, corinthians1},
    {"2Corinthians", 13, corinthians2},
    {"Galatians", 6, galatians},
    {"Ephesians", 6, ephesians},
    {"Philippians", 4, philippians},
    {"Colossians", 4, colossians},
    {"1Thessalonians", 5, thessalonians1},
    {"2Thessalonians", 3, thessalonians2},
    {"1Timothy", 6, timothy1},
    {"2Timothy", 4, timothy2},
    {"Titus", 3, titus},
    {"Philemon", 1, philemon},
    {"Hebrews", 13, hebrews},
    {"James", 5, james},
    {"1Peter", 5, peter1},
    {"2Peter", 3, peter2},
    {"1John", 5, john1},
    {"2John", 1, john2},
    {"3John", 1, john3},
    {"Jude", 1, jude},
    {"Revelation", 22, revelation}
  };
  
  for (i=1; i<argc; i++)
    {
      strcat(opt, argv[i]);
    }
  
  gettimeofday(&t,NULL);
  ii = t.tv_sec+t.tv_usec;
  srand(ii);

  ii = rand() % 31096;      /* Total verses in bible */
  
  for (bkno=0, tv = 0; bkno<66 && tv <= ii; bkno++)
    {
      for (chno=0; chno<books[bkno].n && tv <= ii; chno++)
	{
	  tv += books[bkno].p[chno];
	}
      if (tv>ii)
	bkno--;
    }
  if (tv>ii)
    {
      if (bkno <66)
	{
	  chno--;
	  if (chno <0 )
	    {
	      bkno--;
	      if (bkno<0)
		{
		  bkno = 0; chno = 0;
		}
	    }
	  tv -= books[bkno].p[chno];
	}  
      else
	{
	  bkno--;
	  chno = books[bkno].n - 1;
	  tv -= books[bkno].p[chno];
	}
      vno = ii-tv;
      if (vno<1)
	{
	  vno = 0;
	}
      vno++;
      chno ++;
    }
  /*   printf ("tv = %d; bkno = %d, chno = %d, vno = %d\n", tv, bkno
       chno, vno); */
  
  sprintf(cmd, "%s%d:%d", books[bkno].t, chno, vno);
  /* vno is now redundant; so we now use it as a status code holder */
  
  vno = execlp("bible", opt, cmd, NULL);
  return (vno);
}


