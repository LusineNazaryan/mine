#include "parser.h"

void CParser::work(CReaderWriter* writer, QVector<SDataToken> dataTokens, QVector<SCodeToken> codeTokens,
	QVector<QString> dataTable, QString filename)
{
	m_dataTokens = dataTokens;
	m_codeTokens = codeTokens;
	m_dataTable = dataTable;

	QByteArray dataSection = writeDataSection();						//OK
	QByteArray codeSection = writeCodeSection();						//OK
	QByteArray dataTableSection = writeDataTableSection();				//OK

																		//	testDataSection(dataSection);										//OK
																		//	testCodeSection(codeSection);										//OK
																		//	testDataTableSection(dataTableSection);								//OK

	writer->addSection(dataSection, ESectionType::Data);
	writer->addSection(codeSection, ESectionType::Code);
	writer->addSection(dataTableSection, ESectionType::DataTable);
	writer->write(filename);

}

QByteArray CParser::writeCodeSection()
{
	QByteArray out;
	int offset = 0;			//size of codeSection

	QHash<QString, int>::Iterator i;
	for (int i = 0; i < m_codeTokens.size(); i++)
	{
		for (int k = 0; k < sizeof m_codeTokens[i].opcode.opcode - 1; ++k)
		{
			out.append(char(0));
		}
		out.append(m_codeTokens[i].opcode.opcode);
		offset += sizeof m_codeTokens[i].opcode.opcode;

		for (int j = 0; j < m_codeTokens[i].argValue.size(); j++)
		{
			if ((j == 0 && m_codeTokens[i].opcode.arg1Type == static_cast<uint16>(EArgumentType::Data)) ||
				(j == 1 && m_codeTokens[i].opcode.arg2Type == static_cast<uint16>(EArgumentType::Data)) ||
				(j == 2 && m_codeTokens[i].opcode.arg3Type == static_cast<uint16>(EArgumentType::Data)))
			{
				int indexInDataTable = m_codeTokens[i].argValue[j];
				QString identifierName = m_dataTable[indexInDataTable];
				int indexInSymbolTable = m_nameGrammar[identifierName];
				for (int k = 0; k < sizeof indexInSymbolTable - 1; ++k)
				{
					out.append(char(0));
				}
				out.append(indexInSymbolTable);
				offset += sizeof indexInSymbolTable;
			}
			else
			{
				for (int k = 0; k < sizeof m_codeTokens[i].argValue[j] - 1; ++k)
				{
					out.append(char(0));
				}
				out.append(m_codeTokens[i].argValue[j]);
				offset += sizeof m_codeTokens[i].argValue[j];
			}
		}
	}
	return out;
}
void CParser::testCodeSection(QByteArray& codeSection)
{
	qDebug() << "testCodeSection Start\n\n";
	QByteArray in = codeSection;

	for (int pos = 0; pos < in.size(); ++pos)
	{
		qDebug() << pos << static_cast<int>(in[pos]);
	}
	qDebug() << "\n\nend of testCodeSection";
}
QByteArray CParser::writeDataSection()
{
	QByteArray out;
	int offset = 0;		//size of dataSection
	uint16 zero = 0;
	int remainder = 0;
	int typeToSize = 0;
	int currOffset = 0;

	for (int i = 0; i < m_dataTokens.size(); ++i)
	{
		int dataType = static_cast<int>(m_dataTokens[i].type);
		typeToSize = static_cast<int> (pow(2, dataType));

		if (currOffset % typeToSize != 0)
		{
			int remainder = typeToSize - (currOffset % typeToSize);
			while (remainder != 0)
			{
				out.append(zero);
				remainder--;
				++currOffset;
			}
		}
		m_nameGrammar[m_dataTokens[i].identifierName] = currOffset;
		out.append(m_dataTokens[i].value);
		currOffset += typeToSize;
	}
	return out;
}
void CParser::testDataSection(QByteArray& dataSection)
{
	qDebug() << "testDataSection Start\n\n";
	QByteArray in = dataSection;

	for (int pos = 0; pos < in.size(); ++pos)
	{
		qDebug() << pos << static_cast<int>(in[pos]);
	}

	qDebug() << "\n\nend of testDataSection";
}
QByteArray CParser::writeDataTableSection()
{
	QByteArray out;
	int offset = 0;
	for (int i = 0; i < m_dataTokens.size(); i++)
	{
		int dataType = static_cast<int>(m_dataTokens[i].type);
		for (int k = 0; k < sizeof dataType - 1; ++k)
		{
			out.append(char(0));
		}
		out.append(dataType);

		QByteArray sizeofidname = m_dataTokens[i].identifierName.toUtf8();

		for (int k = 0; k < sizeof sizeofidname.size() - 1; ++k)
		{
			out.append(char(0));
		}
		out.append(sizeofidname.size());
		out.append(sizeofidname);

		for (int k = 0; k < sizeof m_dataTokens[i].line - 1; ++k)
		{
			out.append(char(0));
		}
		out.append(m_dataTokens[i].line);
	}
	return out;
}
void CParser::testDataTableSection(QByteArray& dataTableSection)
{
	qDebug() << "testDataTableSection Start\n\n";

	QByteArray in = dataTableSection;

	for (int pos = 0; pos < in.size(); )
	{
		int nDataType = static_cast<int>(in[pos]);
		qDebug() << "nDataType = " << nDataType;
		++pos;
		int nIdNameSize = static_cast<int>(in[pos]);
		qDebug() << "IdentifierNameSize = " << nIdNameSize;
		++pos;
		QString strIdName = 0;
		for (int j = 0; j < nIdNameSize; ++j)
		{
			strIdName += static_cast<QChar>(in[pos]);
			++pos;
		}
		qDebug() << "IdentifierName = " << strIdName;
		int nLine = static_cast<int>(in[pos]);
		qDebug() << "line = " << nLine;
		++pos;
	}

	qDebug() << "\n\nend of testDataTableSection";
}
QByteArray CParser::writeCodeTableSection()
{
	QByteArray out;
	int offset = 0;
	return out;
}

