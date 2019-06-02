#include "utility/random/Random.hpp"

#include <QtMath>

namespace utility
{

namespace random
{


float frand()
{
	float v=qrand();
	v/=RAND_MAX;
	return v;
}

double dfrand()
{
	double v=qrand();
	v/=RAND_MAX;
	return v;
}



static int ipow(int base, int exp)
{
	int result = 1;
	while (exp) {
		if (exp & 1) {
			result *= base;
		}
		exp >>= 1;
		base *= base;
	}

	return result;
}
QByteArray randomByteArray(int size)
{
	QByteArray out;
	int bpc=(ipow(2,sizeof(char)*8));
	for(int i=0; i<size; ++i) {
		const char ch=static_cast<char>(qrand() % bpc);
		out.append(ch);
	}
	return out;
}


}

}