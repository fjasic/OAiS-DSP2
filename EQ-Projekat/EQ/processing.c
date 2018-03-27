#include "processing.h"
#include "iir.h"
#define SCALE 32768
void calculateShelvingCoeff(float c_alpha, Int16* output)
{
		/* Your code here */
		output[0] = c_alpha*SCALE;
		output[1] = -SCALE;
		output[2] = SCALE-1;
		output[3] = -c_alpha*SCALE;

}
void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
		/* Your code here */
		output[0]=c_alpha;
		output[1]= (-c_beta*(1+c_alpha))*(SCALE/2);
		output[2]= SCALE-1;
		output[3]= SCALE-1;
		output[4]= (-c_beta*(1+c_alpha))*(SCALE/2);
		output[5]= c_alpha*SCALE;
}
Int16 shelvingLP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered = first_order_IIR(input, coeff, x_history, y_history);

		Int32 Output=(((Int32)filtered+input)>>1)+_smpy((input-filtered),k);
			if (Output>=SCALE-1)
				return SCALE-1;
			else if (Output<=-SCALE)
				return -SCALE;
			else
				return (Int16)Output;
}
Int16 shelvingHP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered = first_order_IIR(input, coeff, x_history, y_history);
		/**RAZLIKA IZMEDJU INPUTA I FILTERED PONISTAVAJU SE SREDNJE I VISOKE FREKVENCIJE,DOK KOEFICIJENT K ODREDJUJE NIVO NISKIH FREKVENCIJA U KRAJNJEM SIGNALU,AKO JE MANJE OD 1 DOLAZI DO SLABLJENJA NISKIH FREKVENCIJA,OBRNUTO DOLAZI DO POJACANJA NISKIH FREKVENCIJA
		SABIRANJEM FILTERED I INPUTA DOLAZI DO PONISTAVANJA NISKIH FREKVENCIJA DOK CE SREDNJE I VISOKE BITI NEPROMENJENE**/
		Int32 Output=((Int32)filtered+input)*k+((input-filtered)>>1);
			if (Output>=SCALE-1)
				return SCALE-1;
			else if (Output<=-SCALE)
				return -SCALE;
			else
				return (Int16)Output;
}
Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered = second_order_IIR(input, coeff, x_history, y_history);
		/** Za razliku od NP i VP shelving filtera kojima je moguce regulisati frekvencije nize (NP) ili
		vise od granicne frekvencije (VP) od granicne frekvencije, kod peeking filtera moguce‡e je regulisati opseg
		frekvencija oko centralne frekvencije filtera. Zbog viseg red filtera moguce je kontrolisati dva parametra
		: propusni opseg i centralnu frekvenciju**/
		Int32 Output=(((Int32)filtered+input)>>1)+(Int32)_smpy((input-filtered),k);
			if (Output>=SCALE-1)
				return SCALE-1;
			else if (Output<=-SCALE)
				return -SCALE;
			else
				return (Int16)Output;
}




