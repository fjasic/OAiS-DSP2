//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2017
// *
// * Zadatak: Ekvalizacija audio signala
// * Autor:
// *                                                                          
// *                                                                          
/////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "math.h"

#include "iir.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];

Int16 z_yPEEK1[3];
Int16 z_xPEEK1[3];
Int16 z_yPEEK2[3];
Int16 z_xPEEK2[3];
Int16 z_xL[2];
Int16 z_yL[2];
Int16 z_xH[2];
Int16 z_yH[2];
Int16 shelvingCoeffLP[4];
Int16 shelvingCoeffHP[4];
Int16 peekingCoeff1[6];
Int16 peekingCoeff2[6];
void main( void )
{
	/* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );
	
	/* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Ekvalizacija audio signala \n");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();

    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();
	
	aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

	int i;
	for(i = 0; i < 2; i++) {
		z_xL[i] = 0;
		z_yL[i] = 0;
		z_xH[i] = 0;
		z_yH[i] = 0;
	}

	for(i = 0; i < 3; i++) {
			z_xPEEK1[i] = 0;
			z_yPEEK1[i] = 0;
			z_xPEEK2[i] = 0;
			z_yPEEK2[i] = 0;
	}
	calculateShelvingCoeff(-0.3322, shelvingCoeffLP);
	calculateShelvingCoeff(0.0777, shelvingCoeffHP);
	calculatePeekCoeff(1,-0.4337,peekingCoeff1);
	calculatePeekCoeff(-0.87559,0.0332,peekingCoeff2);

    while(1)
    {
    	aic3204_read_block(sampleBufferL, sampleBufferR);

    	/* Your code here */

    		sampleBufferR[0]=10000;
			for(i=1;i<AUDIO_IO_SIZE;i++)
			{
				sampleBufferR[i]=0;
			}

    		// koristim EQ
    	for(i = 0; i < AUDIO_IO_SIZE; i++) {
			sampleBufferR[i] = shelvingLP(sampleBufferR[i], shelvingCoeffLP, z_xL, z_yL, 24576);

			sampleBufferR[i] = shelvingHP(sampleBufferR[i], shelvingCoeffHP, z_xH, z_yH, 24576);

			sampleBufferR[i] = shelvingPeek(sampleBufferR[i], peekingCoeff1, z_xPEEK1, z_yPEEK1, 24576);

			sampleBufferR[i] = shelvingPeek(sampleBufferR[i], peekingCoeff2, z_xPEEK2, z_yPEEK2, 24576);
			}

		aic3204_write_block(sampleBufferR, sampleBufferR);
	}

    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}

