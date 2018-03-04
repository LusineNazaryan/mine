#ifndef DISPARSER_H
#define DISPARSER_H

#include "tokens.h"
#include "readerwriter.h"


class CDisParser
{
public:
	CDisParser();
	void work(QString exeFilename, CReaderWriter* rw);
	QVector<SDataToken> getDataTokens();
	QVector<SCodeToken> getCodeTokens();
private:
	void readDataSection(QByteArray in);
	void readCodeSection(QByteArray in);
	void readDataTableSection(QByteArray in);
	void readCodeTableSection(QByteArray in);
private:
	void createSymbolTable();
private:
	QVector<SDataToken> m_sDataTokens;
	QVector<SCodeToken> m_sCodeTokens;
	QVector<int> m_symbolTable;
	SHeader m_header;
};






#endif