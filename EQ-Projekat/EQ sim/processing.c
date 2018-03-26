#include "processing.h"
#include "iir.h"

void calculateShelvingCoeff(float c_alpha, Int16* output)
{
		/* Your code here */
		Int16 a =c_alpha*32768;

		output[0] = a;
		output[1] = -32768;
		output[2] = 32767;
		output[3] = -a;


}
void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
		/* Your code here */
		Int16 a=  c_alpha*32768;
		Int16 b=  c_beta*32768;

		output[0]= (Int16)a;
		output[1]= (Int16)(-b*(1+a));
		output[2]= 32767;
		output[3]= 32767;
		output[4]= (Int16)(-b*(1+a));
		output[5]= a;
}
Int16 shelvingLP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered;
		Int32 low;
		Int32 high;
		Int16 res;
		Int32 resRet;
		Int16 ret;

		filtered = first_order_IIR(input, coeff, x_history, y_history);

		high = (input+filtered) >> 1;

		low = input - filtered;

		if(low > 32767)
			low = 32767;
		else if(low < -32768)
			low = -32768;

		res = _smpy(k, (Int16)low);

		resRet = res + high;

		if(resRet > 32767)
			ret = 32767;
		else if (resRet < -32768)
			ret = -32768;
		else
			ret = (Int16) resRet;

	return ret;
}
Int16 shelvingHP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered;
		Int32 low;
		Int32 high;
		Int16 res;
		Int32 resRet;
		Int16 ret;

		filtered = first_order_IIR(input, coeff, x_history, y_history);

		high = input+filtered;

		if(high > 32767)
			high = 32767;
		else if(high < -32768)
			high = -32768;

		res = _smpy(k, (Int16)high);

		low = (input - filtered) >> 1;

		resRet = res + low;

		if(resRet > 32767)
			ret = 32767;
		else if (resRet < -32768)
			ret = -32768;
		else
			ret = (Int16) resRet;

	return ret;
}
Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
		/* Your code here */
		Int16 filtered;
		Int32 low;
		Int32 high;
		Int16 res;
		Int32 resRet;
		Int16 ret;

		filtered = second_order_IIR(input, coeff, x_history, y_history);

		high = (input+filtered) >> 1;

		low = input - filtered;

		if(low > 32767)
			low = 32767;
		else if(low < -32768)
			low = -32768;

		res = _smpy(k, (Int16)low);

		resRet = res + high;

		if(resRet > 32767)
			ret = 32767;
		else if (resRet < -32768)
			ret = -32768;
		else
			ret = (Int16) resRet;

	return ret;
}




