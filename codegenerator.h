#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "tokens.h"
#include "readerwriter.h"

class CCodeGenerator
{
public:
	CCodeGenerator();
	void work(QVector<SDataToken> dataTokens, QVector<SCodeToken> codeTokens, QString filename);

private:

};






#endif