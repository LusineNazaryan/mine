#ifndef TOKENS_H
#define TOKENS_H
#include "instructions.h"
#include <QVarLengthArray>

struct SDataToken
{
	SDataToken() : type(), quantity(0), line(0)
	{
	}

	SDataToken(EType t, uint32 l) : type(t), quantity(0), line(l)
	{
	}

	EType type;
	QString identifierName;
	QByteArray value;
	uint32 quantity;
	uint32 line;
};

struct SCodeToken
{
	SCodeToken() : line(1)
	{
	}

	explicit SCodeToken(uint32 l) : line(l)
	{
	}

	//QString instructionName;

	QVarLengthArray<uint32, 3> argValue;

	UOpcode opcode;
	uint32 line;
};

//DataTable returned by QHash<QString, uint32> CLexer::getDataTable() function

#endif // !TOKENS_H