#ifndef COMPILER_H
#define COMPILER_H

#include "readerwriter.h"
#include "lexer.h"
#include "parser.h"


class CCompiler
{
public:
	CCompiler();
	void work(QString txtFileName, QString exeFileName);
private:
	CReaderWriter * m_cWriter;
	CParser* m_cParser;
	CLexer* m_cLexer;
};



#endif
