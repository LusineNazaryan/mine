#ifndef HEADER_H
#define HEADER_H

#include <QVector>
#include <qdebug.h>
#include "instructions.h"
#include <QDataStream>
#include <QFileInfo>

enum class ESectionType
{
	Data, Code, DataTable, CodeTable
};

const QMap<ESectionType, QString> MSections
{
	{ ESectionType::Data, "DataSection", },
{ ESectionType::Code, "CodeSection", },
{ ESectionType::DataTable, "DataTableSection", },
{ ESectionType::CodeTable, "CodeTableSection", }
};


struct SHeaderTable {
	SHeaderTable()
	{
	}
	//SHeaderTable(ESectionType tt, int32 oo, int32 ss) : eSectionType(tt), offset(oo), size(ss)
	//{
	//}
	ESectionType eSectionType;
	int32 offset;
	int32 size;
};

struct SHeader {
	SHeader()
	{
	}
	//SHeader(QString ss, int32 vv) :sSignature(ss), nVersion(vv)
	SHeader(std::string ss, int32 vv) :sSignature(ss), nVersion(vv)
	{
	}
	//	QString sSignature;
	std::string sSignature;
	int32 nVersion;
	int32 recordCount;
	QVector<SHeaderTable> recordTable;
};
//int32 SHeader::recordCount = 0;


#endif






