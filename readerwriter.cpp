#include "readerwriter.h"

CReaderWriter::CReaderWriter() :
	m_nVersion(1), m_sSignature("SAED/VirtualMachine/ExeFile"), m_headerSize(128)
{
}
void CReaderWriter::addSection(QByteArray section, ESectionType sectionType)
{

	if (section.size() == 0)
	{
		return;
	}
	else
	{
		m_MSectionSizes[sectionType] = section;
	}
}
int CReaderWriter::getOffset(int i)
{
	int offset = m_headerSize;
	while (i)
	{
		offset += m_SHeader.recordTable[i - 1].offset;
		--i;
	}
	return offset;
}
void CReaderWriter::write(QString filename)
{
	createHeader();
	testHeader();

	/////////////////Filling the exe file

	QFile file(filename);
	file.resize(0);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QDataStream out(&file);

	uint8 zero = 0;
	/////fill header
	QChar ch;
	for (int i = 0; i < m_SHeader.sSignature.size(); ++i)
	{
		ch = static_cast<QChar>(m_SHeader.sSignature[i]);
		out << ch;
	}
	out << m_SHeader.nVersion;
	out << m_SHeader.recordCount;

	for (int i = 0; i < m_SHeader.recordCount; ++i)
	{
		out << static_cast<int>(m_SHeader.recordTable[i].eSectionType);
		out << m_SHeader.recordTable[i].offset;
		out << m_SHeader.recordTable[i].size;
	}

	int currPos = file.pos();
	while (currPos < m_headerSize)
	{
		out << zero;
		currPos++;
	}

	////////header is filled
	///////filling .exe file sections
	int secIndex = 2;						///secIndex = 1 for header (usage:	find exact number of zeros to be filled between .ex  file's sections)
	for (auto e : m_MSectionSizes.keys())
	{
		for (int j = 0; j < m_MSectionSizes.value(e).size(); ++j)
		{
			out << static_cast<uint8>(m_MSectionSizes.value(e)[j]);
		}

		currPos = file.pos();
		while (currPos < m_headerSize * secIndex)
		{
			out << zero;
			currPos++;
		}
		++secIndex;
	}
	file.close();

	testTheExeFile(filename);
	SHeader h = readHeader(filename);

	readDataSection(h, filename);

}
SHeader CReaderWriter::readHeader(QString filename)
{
	qDebug() << "\n\nreadHeader Start\n\n";
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray output = file.readAll();
	QDataStream in(&file);

//	SHeader m_readSHeader;

	char ch;
	int i = 1;
	for (; i < m_sSignature.size() * 2; i += 2)		//as the sizeof qbytearray[i] = 16 bytes
	{
		ch = static_cast<char>(output[i]);
		m_readSHeader.sSignature += ch;
	}
	std::cout << "readHeader.sSignature = " << m_readSHeader.sSignature << std::endl;
	std::cout << "readHeader.sSignature.size = " << m_readSHeader.sSignature.size() << std::endl;

	file.seek(54);
	in >> m_readSHeader.nVersion;
	qDebug() << "readHeader.nVersion = " << m_readSHeader.nVersion;

	file.seek(58);
	in >> m_readSHeader.recordCount;
	qDebug() << "readHeader.recordCount = " << m_readSHeader.recordCount;

	for (int k = 0; k < m_readSHeader.recordCount; k++)
	{
		SHeaderTable hdt;
		int sectype;
		in >> sectype;
		hdt.eSectionType = static_cast<ESectionType>(sectype);
		in >> hdt.offset;
		in >> hdt.size;
		m_readSHeader.recordTable.push_back(hdt);
		qDebug() << "hdt = " << static_cast<int>(hdt.eSectionType) << " " << hdt.offset << " " << hdt.size;
	}

	qDebug() << "\n\nend of testTheExeFile";
	return m_readSHeader;
}		//OK		
void CReaderWriter::testTheExeFile(QString filename)
{
	qDebug() << "\n\ntestTheExeFile Start\n\n";

	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray output = file.readAll();
	QDataStream in(&file);


	for (int pos = 0; pos < output.size(); ++pos)
	{
		qDebug() << pos << static_cast<int>(output[pos]);
	}
	qDebug() << "\n\nend of testTheExeFile";

	file.close();
}
void CReaderWriter::createHeader()
{
	m_SHeader.sSignature = m_sSignature;
	m_SHeader.nVersion = m_nVersion;
	m_SHeader.recordCount = m_MSectionSizes.size();

	int currPos = m_headerSize;

	for (auto e : m_MSectionSizes.keys())
	{
		SHeaderTable section;
		section.eSectionType = e;
		section.size = m_MSectionSizes.value(e).size();
		section.offset = currPos;
		m_SHeader.recordTable.push_back(section);
		currPos += m_headerSize;
	}
}
void CReaderWriter::testHeader()
{
	qDebug() << "RW::testHeader Start\n\n";

	//	qDebug() << "signature = " << m_SHeader.sSignature;
	std::cout << "m_SHeader.sSignature = " << m_SHeader.sSignature;
	qDebug() << "version = " << m_SHeader.nVersion;
	qDebug() << "recordCount = " << m_SHeader.recordCount;
	for (int i = 0; i < m_SHeader.recordTable.size(); ++i)
	{
		qDebug() << "sectionType = " << static_cast<int>(m_SHeader.recordTable[i].eSectionType);
		qDebug() << "offset  = " << m_SHeader.recordTable[i].offset;
		qDebug() << "size  = " << m_SHeader.recordTable[i].size;
	}

	qDebug() << "\n\nend of RW::testHeader";
}
QByteArray CReaderWriter::readDataTableSection(SHeader h, QString filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray output = file.readAll();
	int secSize;
	secSize = h.recordTable[static_cast<int>(ESectionType::DataTable)].size;
	QByteArray out;
	int size = m_headerSize;
	file.seek(h.recordTable[static_cast<int>(ESectionType::DataTable)].offset);
	int start = file.pos();
	int j = 0;
	for (int i = start; i < start + size; ++i)
	{
		out[j] = output[i];
		++j;
	}
	return out;
}
QByteArray CReaderWriter::readDataSection(SHeader h, QString filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray output = file.readAll();
	QByteArray out;

	int size = m_headerSize;
	file.seek(h.recordTable[static_cast<int>(ESectionType::Data)].offset);
	int start = file.pos();

	int j = 0;
	for (int i = start; i < start + size; ++i)
	{
		out[j] = output[i];
		++j;
	}
	return out;
	file.close();
}
QByteArray CReaderWriter::readCodeSection(SHeader h, QString filename)
{
	qDebug() << "\n\nreadCodeSection Start\n\n";

	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray output = file.readAll();
	QDataStream in(&file);

	file.seek(h.recordTable[1].offset);
	int secSize;
	secSize = h.recordTable[static_cast<int>(ESectionType::Code)].size;

	QByteArray out;
	int size = m_headerSize;
	file.seek(h.recordTable[static_cast<int>(ESectionType::Code)].offset);
	int start = file.pos();

	int j = 0;
	for (int i = start; i < start + size; ++i)
	{
		out[j] = output[i];
		++j;
	}

	//qDebug() << "out.size() = " << out.size();
	//for (int i = 0; i < out.size(); ++i)
	//{
	//	qDebug() << "out[" << i << "] = " << static_cast<int>(out[i]);
	//}

	return out;


	//QVector <SCodeToken> VCT;
	//int currPos = file.pos();
	//for (int pos = currPos; pos < currPos + secSize;)
	//{
	//	SCodeToken sct;
	//	uint16 opcode;
	//	in >> opcode;
	//	sct.opcode.opcode = opcode;
	//	//	VCT[i].opcode.opcode = opcode;
	//	qDebug() << "opcode = " << opcode;

	//	int count = HInstCount[opcode];
	//	qDebug() << "count = " << count;
	//	for (int i = 0; i < count; ++i)
	//	{
	//		uint32 arg;
	//		in >> arg;
	//		sct.argValue.append(arg);
	//		//		VCT[i].argValue.append(arg);
	//		qDebug() << "arg = " << arg;
	//	}

	//	//	uint8 l;
	//	//	in >> l;
	//	//	sct.line = l;
	//	////	VCT[i].line = l;
	//	//	qDebug() << "VCT[i].line = " << sct.line;
	//	VCT.push_back(sct);
	//	pos = file.pos();
	//}
	file.close();
	qDebug() << "\n\nend of readCodeSection\n\n";
}
QByteArray CReaderWriter::read(QString sFileExe)
{
	QFile file(sFileExe);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Cannot open";
	}
	QDataStream in(&file);
	QByteArray output;
	in >> output;
	return output;
}


