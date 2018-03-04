#include "disparser.h"


CDisParser::CDisParser()
{
}

void CDisParser::work(QString filename, CReaderWriter* rw)
{
	rw = new CReaderWriter;
	m_header = rw->readHeader(filename);

	readDataTableSection(rw->readDataTableSection(m_header, filename));
	createSymbolTable();
	readDataSection(rw->readDataSection(m_header, filename));

	readCodeSection(rw->readCodeSection(m_header, filename));

}


void CDisParser::createSymbolTable()
{
	int offset = 0;		
	uint16 zero = 0;
	int remainder = 0;
	int typeToSize = 0;
	int currOffset = 0;

	for (int i = 0; i < m_sDataTokens.size(); ++i)
	{
		int dataType = static_cast<int>(m_sDataTokens[i].type);
		typeToSize = static_cast<int> (pow(2, dataType));

		if (currOffset % typeToSize != 0)
		{
			int remainder = typeToSize - (currOffset % typeToSize);
			while (remainder != 0)
			{
				remainder--;
				++currOffset;
			}
		}
		m_symbolTable.push_back(currOffset);
		currOffset += typeToSize;
	}
}

QVector<SDataToken> CDisParser::getDataTokens()
{
	return m_sDataTokens;
}
QVector<SCodeToken> CDisParser::getCodeTokens()
{
	return m_sCodeTokens;
}

void CDisParser::readDataSection(QByteArray buf)
{
	qDebug() << "disparser readDataSection\n\n";

	for (int i = 0; i < m_symbolTable.size(); ++i)
	{
		int pos = m_symbolTable[i];
		int dataType = static_cast<int>(m_sDataTokens[i].type);
		int typeToSize = static_cast<int> (pow(2, dataType));

		for (int j = 0; j < typeToSize; ++j)
		{ 
			m_sDataTokens[i].value[j] = buf[pos];
			++pos;
		}
	}
	//for (int i = 0; i < m_sDataTokens.size(); ++i)
	//{
	//	for (int j = 0; j < m_sDataTokens[i].value.size(); ++j)
	//	{
	//		qDebug() << "static_cast<int>(m_sDataTokens[i].value[" << j << "]) = " << static_cast<int>(m_sDataTokens[i].value[j]);
	//	}
	//}
	qDebug() << "\n\nend of disparser readDataSection\n\n";
}
void CDisParser::readCodeSection(QByteArray buf)
{
	QDataStream in(buf);
	int secSize = m_header.recordTable[static_cast<int>(ESectionType::Code)].size;

	QByteArray out;
	int start = 0;
	for (int pos = 0; pos < secSize;)
	{
		SCodeToken sct;
		uint16 opcode;
		in >> opcode;
		pos += 2;
		sct.opcode.opcode = opcode;
		qDebug() << "opcode = " << opcode;

		int count = HInstCount[opcode];
		for (int i = 0; i < count; ++i)
		{
			uint32 arg;
			in >> arg;
			pos += 4;
			sct.argValue.append(arg);
			qDebug() << "arg = " << arg;
		}

		//	uint8 l;
		//	in >> l;
		//	sct.line = l;
		////	VCT[i].line = l;
		//	qDebug() << "VCT[i].line = " << sct.line;
		m_sCodeTokens.push_back(sct);
	//	pos = file.pos();
	}
}
void CDisParser::readDataTableSection(QByteArray buf)
{
	qDebug() << "disparser readDataTableSection\n\n";

	QDataStream in(buf);

	int secSize = m_header.recordTable[static_cast<int>(ESectionType::DataTable)].size;
	int currPos = 0;
	for (int p = currPos; p < secSize;)
	{
		SDataToken dt;
		int dataType;
		in >> dataType;
		dt.type = static_cast<EType>(dataType);
		qDebug() << "dataType = " << dataType;
		p += 4;

		int idNameSize;
		in >> idNameSize;
		p += 4;
		std::string idName;
		uint8 ch;
		for (int j = 0; j < idNameSize; ++j)
		{
			in >> ch;
			++p;
			idName += ch;
		}
		dt.identifierName = QString::fromStdString(idName);
		qDebug() << "idName = " << dt.identifierName;
		int l;
		in >> l;
		p += 4;
		dt.line = l;
		qDebug() << "line = " << dt.line;
		m_sDataTokens.push_back(dt);
	}
	qDebug() << "\n\nend of readDataTableSection";
}
void CDisParser::readCodeTableSection(QByteArray buf)
{

}


