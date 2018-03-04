#ifndef READERWRITER_H
#define READERWRITER_H

#include "lexer.h"
#include "instructions.h"
#include "header.h"

#include <QTextStream>
#include <QVector>
#include <qdebug.h>
#include <QDataStream>
#include <QFileInfo>
#include <QString>
#include <QtEndian>

#include <string>
#include <iostream>

class CReaderWriter
{
public:
	CReaderWriter();

	void addSection(QByteArray buf, ESectionType sectionType);
	void write(QString filename);

	QByteArray read(QString sFileExe);


public:
	QByteArray getDataSection();
public:
	SHeader readHeader(QString filename);
	QByteArray readDataTableSection(SHeader h, QString filename);
	QByteArray readDataSection(SHeader h, QString filename);
	QByteArray readCodeSection(SHeader h, QString filename);
private:
	void createHeader();
	//	QByteArray writeHeader(QString signature, int32 version);
	QByteArray writeHeader(std::string signature, int32 version);
	int getOffset(int i);

	void printMSectionSizes();
	void testHeader();
	void testTheExeFile(QString filename);

private:
	QMap<ESectionType, QByteArray> m_MSectionSizes;

private:
	int32 m_headerSize;
	int32 m_nVersion;
	//	QString m_sSignature;
	std::string m_sSignature;
	SHeader m_SHeader;
	QByteArray m_buffer;

private:
	SHeader m_readSHeader;
};




#endif
