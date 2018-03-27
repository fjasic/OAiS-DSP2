#include "processing.h"
#include "iir.h"
#define SCALE 32768
void calculateShelvingCoeff(float c_alpha, Int16* output)
{
		/* Your code here */
		Int16 a =c_alpha;

		output[0] = a*SCALE;
		output[1] = -SCALE;
		output[2] = SCALE-1;
		output[3] = -a*SCALE;

}
void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
		/* Your code here */
		Int16 a=  c_alpha;
		Int16 b=  c_beta;

		output[0]= a;
		output[1]= (-b*(1+a))*(SCALE/2);
		output[2]= SCALE-1;
		output[3]= SCALE-1;
		output[4]= (-b*(1+a))*(SCALE/2);
		output[5]= a*SCALE;
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
		/**RAZLIKA IZMEDJU INPUTA I FILTERED PONISTAVAJU SE SREDNJE I VISOKE FREKVENCIJE,DOL KOEFICIJENT K ODREDJUJE NIVO NISKIH FREKVENCIJA U KRAJNJEM SIGNALU,AKO JE MANJE OD 1 DOLAZI DO SLABLJENJA NISKIH FREKVENCIJA,OBRNUTO DOLAZI DO POJACANJA NISKIH FREKVENCIJA
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
		/** Za razliku od NP i VP shelving filtera kojima je moguće regulisati frekvencije niže (NP) ili
		više od granične frekvencije (VP) od granične frekvencije, kod peeking filtera moguće je regulisati opseg
		frekvencija oko centralne frekvencije filtera. Zbog višeg red filtera moguće je kontrolisati dva parametra
		– propusni opseg i centralnu frekvenciju**/
		Int32 Output=(((Int32)filtered+input)>>1)+(Int32)_smpy((input-filtered),k);
			if (Output>=SCALE-1)
				return SCALE-1;
			else if (Output<=-SCALE)
				return -SCALE;
			else
				return (Int16)Output;
}




