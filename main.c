
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLEMATRICE 4 //aes 128bit = 4*4*8bit
#define NBROUND 10 //aes 128bit

/* DRESCH Stéphane */


static const unsigned char SubBytesMatrice[16][16] = {
    {0x63,0xca,0xb7,0x4,0x09,0x53,0xd0,0x51,0xcd,0x60,0xe0,0xe7,0xba,0x70,0xe1,0x8c},
    {0x7c,0x82,0xfd,0xc7,0x83,0xd1,0xef,0xa3,0x0c,0x81,0x32,0xc8,0x78,0x3e,0xf8,0xa1},
    {0x77,0xc9,0x93,0x23,0x2c,0x0,0xaa,0x40,0x13,0x4f,0x3a,0x37,0x25,0xb5,0x98,0x89},
    {0x7b,0x7d,0x26,0xc3,0x1a,0xed,0xfb,0x8f,0xec,0xdc,0x0a,0x6d,0x2e,0x66,0x11,0x0d},
    {0xf2,0xfa,0x36,0x18,0x1b,0x20,0x43,0x92,0x5f,0x22,0x49,0x8d,0x1c,0x48,0x69,0xbf},
    {0x6b,0x59,0x3f,0x96,0x6e,0xfc,0x4d,0x9d,0x97,0x2a,0x6,0xd5,0xa6,0x3,0xd9,0xe6},
    {0x6f,0x47,0xf7,0x5,0x5a,0xb1,0x33,0x38,0x44,0x90,0x24,0x4e,0xb4,0xf6,0x8e,0x42},
    {0xc5,0xf0,0xcc,0x9a,0xa0,0x5b,0x85,0xf5,0x17,0x88,0x5c,0xa9,0xc6,0x0e,0x94,0x68},
    {0x30,0xad,0x34,0x7,0x52,0x6a,0x45,0xbc,0xc4,0x46,0xc2,0x6c,0xe8,0x61,0x9b,0x41},
    {0x1,0xd4,0xa5,0x12,0x3b,0xcb,0xf9,0xb6,0xa7,0xee,0xd3,0x56,0xdd,0x35,0x1e,0x99},
    {0x67,0xa2,0xe5,0x80,0xd6,0xbe,0x2,0xda,0x7e,0xb8,0xac,0xf4,0x74,0x57,0x87,0x2d},
    {0x2b,0xaf,0xf1,0xe2,0xb3,0x39,0x7f,0x21,0x3d,0x14,0x62,0xea,0x1f,0xb9,0xe9,0x0f},
    {0xfe,0x9c,0x71,0xeb,0x29,0x4a,0x50,0x10,0x64,0xde,0x91,0x65,0x4b,0x86,0xce,0xb0},
    {0xd7,0xa4,0xd8,0x27,0xe3,0x4c,0x3c,0xff,0x5d,0x5e,0x95,0x7a,0xbd,0xc1,0x55,0x54},
    {0xab,0x72,0x31,0xb2,0x2f,0x58,0x9f,0xf3,0x19,0x0b,0xe4,0xae,0x8b,0x1d,0x28,0xbb},
    {0x76,0xc0,0x15,0x75,0x84,0xcf,0xa8,0xd2,0x73,0xdb,0x79,0x8,0x8a,0x9e,0xdf,0x16}
};


static const unsigned char InvSubBytesMatrice[16][16] = {
    {0x52,0x7c,0x54,0x8,0x72,0x6c,0x90,0xd0,0x3a,0x96,0x47,0xfc,0x1f,0x60,0xa0,0x17},
    {0x9,0xe3,0x7b,0x2e,0xf8,0x70,0xd8,0x2c,0x91,0xac,0xf1,0x56,0xdd,0x51,0xe0,0x2b},
    {0x6a,0x39,0x94,0xa1,0xf6,0x48,0xab,0x1e,0x11,0x74,0x1a,0x3e,0xa8,0x7f,0x3b,0x4},
    {0xd5,0x82,0x32,0x66,0x64,0x50,0x0,0x8f,0x41,0x22,0x71,0x4b,0x33,0xa9,0x4d,0x7e},
    {0x30,0x9b,0xa6,0x28,0x86,0xfd,0x8c,0xca,0x4f,0xe7,0x1d,0xc6,0x88,0x19,0xae,0xba},
    {0x36,0x2f,0xc2,0xd9,0x68,0xed,0xbc,0x3f,0x67,0xad,0x29,0xd2,0x7,0xb5,0x2a,0x77},
    {0xa5,0xff,0x23,0x24,0x98,0xb9,0xd3,0x0f,0xdc,0x35,0xc5,0x79,0xc7,0x4a,0xf5,0xd6},
    {0x38,0x87,0x3d,0xb2,0x16,0xda,0x0a,0x2,0xea,0x85,0x89,0x20,0x31,0x0d,0xb0,0x26},
    {0xbf,0x34,0xee,0x76,0xd4,0x5e,0xf7,0xc1,0x97,0xe2,0x6f,0x9a,0xb1,0x2d,0xc8,0xe1},
    {0x40,0x8e,0x4c,0x5b,0xa4,0x15,0xe4,0xaf,0xf2,0xf9,0xb7,0xdb,0x12,0xe5,0xeb,0x69},
    {0xa3,0x43,0x95,0xa2,0x5c,0x46,0x58,0xbd,0xcf,0x37,0x62,0xc0,0x10,0x7a,0xbb,0x14},
    {0x9e,0x44,0x0b,0x49,0xcc,0x57,0x5,0x3,0xce,0xe8,0x0e,0xfe,0x59,0x9f,0x3c,0x63},
    {0x81,0xc4,0x42,0x6d,0x5d,0xa7,0xb8,0x1,0xf0,0x1c,0xaa,0x78,0x27,0x93,0x83,0x55},
    {0xf3,0xde,0xfa,0x8b,0x65,0x8d,0xb3,0x13,0xb4,0x75,0x18,0xcd,0x80,0xc9,0x53,0x21},
    {0xd7,0xe9,0xc3,0xd1,0xb6,0x9d,0x45,0x8a,0xe6,0xdf,0xbe,0x5a,0xec,0x9c,0x99,0x0c},
    {0xfb,0xcb,0x4e,0x25,0x92,0x84,0x6,0x6b,0x73,0x6e,0x1b,0xf4,0x5f,0xef,0x61,0x7d}
};

static const unsigned char MixColumnsMatrice[4][4] = {
    {0x02,0x01,0x01,0x03},
    {0x03,0x02,0x01,0x01},
    {0x01,0x03,0x02,0x01},
    {0x01,0x01,0x03,0x02}
};


static const unsigned char InvMixColumnsMatrice[4][4] = {
    {0x0E,0x09,0x0D,0x0B},
    {0x0B,0x0E,0x09,0x0D},
    {0x0D,0x0B,0x0E,0x09},
    {0x09,0x0D,0x0B,0x0E}
};

static const unsigned char Rcon[15][4] = {
    {0x01,0x00,0x00,0x00},
    {0x02,0x00,0x00,0x00},
    {0x04,0x00,0x00,0x00},
    {0x08,0x00,0x00,0x00},
    {0x10,0x00,0x00,0x00},
    {0x20,0x00,0x00,0x00},
    {0x40,0x00,0x00,0x00},
    {0x80,0x00,0x00,0x00},
    {0x1b,0x00,0x00,0x00},
    {0x36,0x00,0x00,0x00},
    {0x6c,0x00,0x00,0x00},
    {0xd8,0x00,0x00,0x00},
    {0xab,0x00,0x00,0x00},
    {0x4d,0x00,0x00,0x00},
    {0x9a,0x00,0x00,0x00}
};



void SubBytes( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne, colonne;

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

            matrice[colonne][ligne]=SubBytesMatrice[matrice[colonne][ligne]&0x0f][(matrice[colonne][ligne]>>4)&0x0f];
        }
    }
}

void InvSubBytes( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne, colonne;

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

            matrice[colonne][ligne]=InvSubBytesMatrice[matrice[colonne][ligne]&0x0f][(matrice[colonne][ligne]>>4)&0x0f];

        }
    }
}




void ExpansionCle(char cle[], char unsigned cleEtendue[TAILLEMATRICE*(NBROUND+1)][TAILLEMATRICE]){

    int caractere, round, ligne;

    unsigned char wMoinsUn[TAILLEMATRICE][TAILLEMATRICE]={0};
    unsigned char wMoinsQuatre[TAILLEMATRICE];

    memset(cleEtendue, 0, TAILLEMATRICE*NBROUND);

    for (caractere=0; caractere<strlen(cle); caractere++){

        cleEtendue[(caractere/TAILLEMATRICE)%TAILLEMATRICE][caractere%TAILLEMATRICE]=cle[caractere];

    }

    for(round=1; round<=NBROUND*TAILLEMATRICE; round++){

        for(ligne=0; ligne<TAILLEMATRICE; ligne++){

            wMoinsQuatre[ligne]=cleEtendue[(round+TAILLEMATRICE-1)-4][ligne];
            wMoinsUn[1][ligne]=cleEtendue[(round+TAILLEMATRICE-1)-1][ligne];

        }


        if((round-1)%TAILLEMATRICE==0){

                RotWord(wMoinsUn);
                SubBytes(wMoinsUn);
         }

        for(ligne=0; ligne<TAILLEMATRICE; ligne++){


            cleEtendue[TAILLEMATRICE+round-1][ligne]=wMoinsQuatre[ligne]^wMoinsUn[1][ligne];

            if((round-1)%TAILLEMATRICE==0){ (cleEtendue[TAILLEMATRICE+round-1][ligne])^=(Rcon[((round-1)/4)][ligne]);}

        }
    }
}




void AddRoundKey(unsigned char cleEtendue[TAILLEMATRICE*(NBROUND+1)][TAILLEMATRICE], int numRound ,unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne, colonne;

    for (colonne=numRound*TAILLEMATRICE; colonne<(numRound+1)*TAILLEMATRICE; colonne++){

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

          // printf("[0x%.2X\t]^[0x%.2X\t] ",matrice[colonne-(numRound*TAILLEMATRICE)][ligne],cleEtendue[colonne][ligne]);
           //matrice[colonne][ligne]= matrice[colonne][ligne]^cleEtendue[colonne-(numRound*TAILLEMATRICE)][ligne];

           matrice[colonne-(numRound*TAILLEMATRICE)][ligne]^=cleEtendue[colonne][ligne];

        }

    }
}

void RotWord( unsigned char mot[TAILLEMATRICE][TAILLEMATRICE]){

 int ligne,nouvelIndice;

    unsigned char tmp[TAILLEMATRICE]={0};

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

       nouvelIndice= (TAILLEMATRICE+ligne-1)%TAILLEMATRICE;

       tmp[nouvelIndice]=mot[1][ligne];
    }

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

       mot[1][ligne]=tmp[ligne];
    }
}




void ShiftRow( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne,colonne,nouvelIndice;

    unsigned char tmp[TAILLEMATRICE][TAILLEMATRICE]={0};

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           nouvelIndice= (TAILLEMATRICE+colonne-ligne)%TAILLEMATRICE;

           tmp[nouvelIndice][ligne]=matrice[colonne][ligne];
        }
    }

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           matrice[colonne][ligne]=tmp[colonne][ligne];
        }
    }
}



void InvShiftRow( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne,colonne,nouvelIndice;

    unsigned char tmp[TAILLEMATRICE][TAILLEMATRICE]={0};

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           nouvelIndice= (TAILLEMATRICE+colonne+ligne)%TAILLEMATRICE;

           tmp[nouvelIndice][ligne]=matrice[colonne][ligne];
        }
    }

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           matrice[colonne][ligne]=tmp[colonne][ligne];
        }
    }
}

unsigned char multiplicationParDeux(unsigned char nombre){

    if( nombre <= 127){
         return (nombre<<1)&0xff;
    }
    else{
        return ((nombre<<1)&0xff)^0x1b;
   }
}

unsigned char multiplicationParTrois(unsigned char nombre){

    if(nombre <= 127){
        return ((nombre<<1)&0xff)^nombre;
    }
    else{
        return (((nombre<<1)&0xff)^0x1b)^nombre;
   }
}


void MixColumns( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne,colonne,i;

    unsigned char tmp[TAILLEMATRICE][TAILLEMATRICE];

    unsigned char multiplication=0;

    for (colonne=0; colonne<TAILLEMATRICE; colonne++){

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            tmp[colonne][ligne]=0;

            for (i=0; i<TAILLEMATRICE; i++){

                // <<1 = *2

                if(MixColumnsMatrice[i][ligne]==1){

                    multiplication=matrice[colonne][i];
                }
                else if(MixColumnsMatrice[i][ligne]==2){

                    multiplication=multiplicationParDeux(matrice[colonne][i]);

                }
                else if(MixColumnsMatrice[i][ligne]==3){

                    multiplication=multiplicationParTrois(matrice[colonne][i]);

                }
                tmp[colonne][ligne]^=multiplication;
            }
        }
    }


    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           matrice[colonne][ligne]=tmp[colonne][ligne];
        }
    }

}





void InvMixColumns( unsigned char matrice[TAILLEMATRICE][TAILLEMATRICE]){

    int ligne,colonne,i;

    unsigned char tmp[TAILLEMATRICE][TAILLEMATRICE];

    unsigned char multiplication=0;

    for (colonne=0; colonne<TAILLEMATRICE; colonne++){

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            tmp[colonne][ligne]=0;

            for (i=0; i<TAILLEMATRICE; i++){

                // <<1 = *2

                if(InvMixColumnsMatrice[i][ligne]==9){

                   tmp[colonne][ligne]^=multiplicationParDeux(multiplicationParDeux(multiplicationParDeux(matrice[colonne][i])))^matrice[colonne][i];
                }
                else if(InvMixColumnsMatrice[i][ligne]==11){

                   tmp[colonne][ligne]^=multiplicationParDeux(multiplicationParDeux(multiplicationParDeux(matrice[colonne][i]))^matrice[colonne][i])^matrice[colonne][i];

                }
                else if(InvMixColumnsMatrice[i][ligne]==13){

                   tmp[colonne][ligne]^=multiplicationParDeux(multiplicationParDeux(multiplicationParDeux(matrice[colonne][i])^(matrice[colonne][i])))^matrice[colonne][i];

                }

                else if(InvMixColumnsMatrice[i][ligne]==14){

                   tmp[colonne][ligne]^=multiplicationParDeux(multiplicationParDeux(multiplicationParDeux(matrice[colonne][i])^matrice[colonne][i])^matrice[colonne][i]);
                }

                //printf("%d \n", tmp[colonne][ligne]);

                //tmp[colonne][ligne]^=multiplication;
            }
        }
    }


    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

           matrice[colonne][ligne]=tmp[colonne][ligne];
        }
    }

}




void  chiffrerBloc(unsigned char mat[TAILLEMATRICE][TAILLEMATRICE], unsigned char cleEtendue[TAILLEMATRICE*(NBROUND+1)][TAILLEMATRICE]){

int ligne, colonne, round;
        // Traitement du bloc

        for(round=0; round<NBROUND; round++){

        /*//DEBUG BLOC EN CLAIR ------------------------------------------

            printf("\nBLOC EN CLAIR \n");

            for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                   printf("0x%.2X\t"  ,mat[colonne][ligne]);
                }

                 printf("\n");
            }

             printf("\n---------------------------------\n");
            // FIN DEBUG BLOC EN CLAIR---------------------------------------*/

            AddRoundKey(cleEtendue,round,mat);

            /*//DEBUG ADDROUNDKEY----------------------------------------------

            printf("\nADDROUNDKEY [%d] \n",round);

            for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                   printf("0x%.2X\t"  ,mat[colonne][ligne]);
                }

                 printf("\n");
            }

            printf("\n---------------------------------\n");
            // FIN DEBUG BLOC EN CLAIR---------------------------------------*/

            SubBytes(mat);

            /*//DEBUG SUBBYTES------------------------------------------

            printf("DEBUG SUBBYTES[%d] \n",round);

            for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                   printf("0x%.2X\t"  ,mat[colonne][ligne]);
                }

                 printf("\n");
            }

             printf("\n---------------------------------\n");
            // FIN DEBUG SUBBYTES---------------------------------------*/

            ShiftRow(mat);

            /*//DEBUG SHIFTROW------------------------------------------

            printf("DEBUG SHIFTROW[%d] \n",round);

            for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                   printf("0x%.2X\t"  ,mat[colonne][ligne]);
                }

                 printf("\n");
            }

             printf("\n---------------------------------\n");
         // FIN DEBUG SHIFTROW---------------------------------------*/

            if(round!=(NBROUND-1))
            {
                MixColumns(mat);

               /* //DEBUG MIXCOLUMNS------------------------------------------

                printf("DEBUG MIXCOLUMNS [%d] \n",round);

                for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                    for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                       printf("0x%.2X\t"  ,mat[colonne][ligne]);
                    }

                     printf("\n");
                }

                 printf("\n---------------------------------\n");
                // FIN DEBUG MIXCOLUMNS---------------------------------------*/
            }
        }

        AddRoundKey(cleEtendue,round,mat);



       //DEBUG  MESSAGE CRYPTE EN HEXA------------------------------------------

        printf("BLOC CHIFFRE EN HEXA   \n",round);

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

              printf("0x%.2X\t"  ,mat[colonne][ligne]);
            }

             printf("\n");
        }

        // FIN DEBUG MESSAGE CRYPTE EN HEXA---------------------------------------

}




void  dechiffrerBloc(unsigned char mat[TAILLEMATRICE][TAILLEMATRICE], unsigned char cleEtendue[TAILLEMATRICE*(NBROUND+1)][TAILLEMATRICE]){

    int ligne, colonne, round;
    // Traitement du bloc


   //BLOC CHIFFRE------------------------------------------
   printf("\n---------------------------------\n");
   printf("\nBLOC CHIFFRE \n",round);

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

          printf("0x%.2X\t"  ,mat[colonne][ligne]);
        }

         printf("\n");
    }
    printf("\n");

     //FIN BLOC CHIFFRE-----------------------------------



    AddRoundKey(cleEtendue,NBROUND,mat);

   /* //ADD ROUND KEY------------------------------------------
    printf("ADD ROUND KEY [%d]  \n",round);

    for (ligne=0; ligne<TAILLEMATRICE; ligne++){

        for (colonne=0; colonne<TAILLEMATRICE; colonne++){

          printf("0x%.2X\t"  ,mat[colonne][ligne]);
        }

        printf("\n");
    }
    printf("\n---------------------------------\n");

    //FIN ADD ROUND KEY-----------------------------------*/

    for(round=NBROUND-1; round>=0; round--){


        InvShiftRow(mat);


       /* //INV SHIFT ROW------------------------------------------
        printf("SHIFT ROW [%d]  \n",round);

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

              printf("0x%.2X\t"  ,mat[colonne][ligne]);
            }

             printf("\n");
        }

        printf("\n---------------------------------\n");
        //INV SHIFT ROW-----------------------------------*/


        InvSubBytes(mat);

       /*//INV SUB BYTES------------------------------------------
        printf("INV SUBBYTES[%d]  \n",round);

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

              printf("0x%.2X\t"  ,mat[colonne][ligne]);
            }

             printf("\n");
        }
        printf("\n---------------------------------\n");
        //INV SUBBYTES------------------------------------------*/

        AddRoundKey(cleEtendue,round,mat);

        /* //ADD ROUND KEY------------------------------------------
        printf("ADD ROUND KEY [%d]  \n",round);

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

              printf("0x%.2X\t"  ,mat[colonne][ligne]);
            }

             printf("\n");
        }

        printf("\n---------------------------------\n");
        //FIN ADD ROUND KEY-----------------------------------*/



        if(round!=(0))
        {
            InvMixColumns(mat);

            /*//DEBUG INV MIXCOLUMNS------------------------------------------

            printf("DEBUG INV MIXCOLUMNS [%d] \n",round);

            for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                   printf("0x%.2X\t"  ,mat[colonne][ligne]);
                }

                 printf("\n");
            }

             printf("\n---------------------------------\n");
            // FIN DEBUG INV MIXCOLUMNS---------------------------------------*/
        }
    }



       //DEBUG  MESSAGE CRYPTE EN HEXA------------------------------------------

        printf("BLOC DECHIFRRE EN HEXA   \n",round);

        for (ligne=0; ligne<TAILLEMATRICE; ligne++){

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

              printf("0x%.2X\t"  ,mat[colonne][ligne]);
            }

             printf("\n");
        }

}



int main(int argc,char* argv[])
{


    int ligne, colonne, lectureOctect, caractere, bloc;

    unsigned char mat[TAILLEMATRICE][TAILLEMATRICE]={0};
    unsigned char cleEtendue[TAILLEMATRICE*(NBROUND+1)][TAILLEMATRICE]={0};


    unsigned char cleDefaut[]= {0x2b ,0x7e , 0x15, 0x16, 0x28 ,0xae , 0xd2, 0xa6,0xab ,0xf7 , 0x15, 0x88,0x09 ,0xcf , 0x4f, 0x3c,'\0'};
    unsigned char messageDefaut[]= {0x32 ,0x43 , 0xf6, 0xa8, 0x88 ,0x5a , 0x30, 0x8d,0x31 ,0x31 , 0x98, 0xa2,0xe0 ,0x37 , 0x07, 0x34,'\0'};

    char* cle;
    char* message;


    if(argc && argc>=3){

        cle=argv[1];
        message= argv[2];
    }
    else{

         cle = cleDefaut;
         message= messageDefaut;
    }



    printf("Cle K : %s <-> 0x%.2X \n\n",cle);
    printf("Message : %s \n",message);
    printf("Longeur : %d \n\n",strlen(message));

    //Expansion de la cle pour chaque round

    ExpansionCle(cle,cleEtendue);




    if(argc && argc==4 && (!strcmp(argv[3],"-C") || !strcmp(argv[3],"-c"))){


        size_t tailleMessageChiffre= ((((strlen(message)-1)/(TAILLEMATRICE*TAILLEMATRICE))*TAILLEMATRICE*TAILLEMATRICE)+TAILLEMATRICE*TAILLEMATRICE+1)*sizeof(char);
        char* messageChiffre = malloc (tailleMessageChiffre);

        messageChiffre[tailleMessageChiffre]='\0';
        lectureOctect=0;
        caractere=0;


        for(bloc=1; bloc<=(tailleMessageChiffre-1)/(TAILLEMATRICE*TAILLEMATRICE); bloc++)
        {
            //Copie du texte clair dans un bloc (matrice) pour traitement

            memset(mat, 0, TAILLEMATRICE*TAILLEMATRICE);

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                    if(lectureOctect<strlen(message)){
                        mat[colonne][ligne]=message[lectureOctect];
                        lectureOctect++;
                    }
                }

            }

            //Chiffrement du bloc

            chiffrerBloc(mat,cleEtendue);

            // Convertion du bloc en chaine de caractère pour l'affichage du message crypté



            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                   messageChiffre[caractere]=mat[colonne][ligne];

                   caractere++;

                }
            }
        }


        // affichage du message crypté
        printf("\n---------------------------------\nMessage chiffre\n\n");

        printf("Hexa : ");

        for (caractere=0; caractere<tailleMessageChiffre-1; caractere++){

            printf("%.2X",messageChiffre[caractere]&0xFF);

        }
         printf("\n\n");

         printf("Ascii : %s",messageChiffre);

         printf("\n\n");


        free(messageChiffre);

    }
    else if(argc && argc==4 && (!strcmp(argv[3],"-D") ||!strcmp(argv[3],"-d"))){


        size_t tailleMessageDechiffre= ((((strlen(message)/2-1)/(TAILLEMATRICE*TAILLEMATRICE))*TAILLEMATRICE*TAILLEMATRICE)+TAILLEMATRICE*TAILLEMATRICE+1)*sizeof(char);
        char* messageDechiffre = malloc (tailleMessageDechiffre);

        messageDechiffre[tailleMessageDechiffre]='\0';

        caractere=0;



        //Convertion message hexa(saisie en ascii) en hexa
        char conv;

        for(lectureOctect=0;lectureOctect<strlen(message); lectureOctect=lectureOctect+2)
        {

            if(message[lectureOctect]>=65 && message[lectureOctect]<=90){
                conv=((message[lectureOctect]-55)<<4);
            }
            else if(message[lectureOctect]>=48 && message[lectureOctect]<=57){
                conv=((message[lectureOctect]-48)<<4);
            }
            else if(message[lectureOctect]>=97 && message[lectureOctect]<=122){
                conv=((message[lectureOctect]-87)<<4);
            }


            if(message[lectureOctect+1]>=65 && message[lectureOctect+1]<=90){
                conv^=((message[lectureOctect+1]-55));
            }
            else if(message[lectureOctect+1]>=48 && message[lectureOctect+1]<=57){
                conv^=((message[lectureOctect+1]-48));
            }
            else if(message[lectureOctect+1]>=97 && message[lectureOctect+1]<=122){
                conv^=((message[lectureOctect+1]-87));
            }

            message[lectureOctect/2]=conv;
        }


        lectureOctect=0;

        for(bloc=1; bloc<=(tailleMessageDechiffre-1)/(TAILLEMATRICE*TAILLEMATRICE); bloc++)
        {
            //Copie du texte chiffré dans un bloc (matrice) pour traitement

            memset(mat, 0, TAILLEMATRICE*TAILLEMATRICE);


            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                    if(lectureOctect<strlen(message)){
                        mat[colonne][ligne]=message[lectureOctect];
                        lectureOctect++;
                    }
                }

            }

            //Dechiffrement du bloc

            dechiffrerBloc(mat,cleEtendue);

            // Convertion du bloc en chaine de caractère pour l'affichage du message déchiffré

            for (colonne=0; colonne<TAILLEMATRICE; colonne++){

                for (ligne=0; ligne<TAILLEMATRICE; ligne++){

                   messageDechiffre[caractere]=mat[colonne][ligne];

                   caractere++;

                }
            }
        }


        // affichage du message dehiffré
        printf("\n---------------------------------\nMessage dechiffre : %s \n" , messageDechiffre);
        free(messageDechiffre);



    }
    else{
         printf("\nAIDE \n");

         printf("arguments : cleSecrete message -C (chiffer texte en ascii) ou -D (dechiffrer texte en hexa) \n\n");
    }









    return 0;
}
