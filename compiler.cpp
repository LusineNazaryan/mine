#include "compiler.h"

CCompiler::CCompiler()
{
	m_cLexer = new CLexer;
	m_cParser = new CParser;
	m_cWriter = new CReaderWriter;
}

void CCompiler::work(QString txtFileName, QString exeFileName)
{
	QFile fin(txtFileName);
	QFileInfo f_info(fin);
	if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Cannot open" << f_info.absoluteFilePath() <<
			"file doesn't exist";
	}
	QTextStream input(&fin);
	try
	{
		CLexer lexer;
		lexer.work(input);
		CParser parser;
		CReaderWriter rw;

		parser.work(&rw, lexer.getData(), lexer.getCode(), lexer.getDataTable(), exeFileName);
	}
	catch (CError& e)
	{
		qDebug() << e.ShowError();
	}
}

