#ifndef REGISTERS_H
#define REGISTERS_H

#include <QByteArray>
#include <QVector>

struct Register
{
	QByteArray R;	//0 - 3 special registers, 4-7 address registers, 7-1031 data registers 
					//	QVector<int> A;	//0 - 7

	Register()
	{
		R.resize(1031);
		//		A.resize(8);
	}
};



#endif