#ifndef PARSER_H
#define PARSER_H

#include "readerwriter.h"
#include "lexer.h"
#include "header.h"
//#include "buffer.h"
#include <QTextStream>
#include <QVector>
#include <qdebug.h>
#include "instructions.h"
#include <QDataStream>
#include <QFileInfo>
#include <cmath>

using uint16 = unsigned short;

class CParser
{
public:
	CParser::CParser()
	{
	}
	void work(CReaderWriter* writer, QVector<SDataToken> dataTokens, QVector<SCodeToken> codeTokens,
		QVector<QString> dataTable, QString filename);
public:
	QByteArray writeDataSection();
	QByteArray writeCodeSection();
	QByteArray writeDataTableSection();
	QByteArray writeCodeTableSection();
	void testDataTableSection(QByteArray& dataTableSection);
	void testDataSection(QByteArray& dataSection);
	void testCodeSection(QByteArray& codeSectoion);
private:
	QVector<SDataToken> m_dataTokens;
	QVector<SCodeToken> m_codeTokens;
	QVector<QString> m_dataTable;
	QHash<QString, int> m_nameGrammar;
};


#endif
