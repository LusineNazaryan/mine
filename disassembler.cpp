#include "disassembler.h"


CDisAssembler::CDisAssembler()
{

}


void CDisAssembler::work(QString exename, QString txtname)
{

	m_disp = new CDisParser;
	m_rw = new CReaderWriter;
	m_codegen = new CCodeGenerator;
	m_disp->work(exename, m_rw);
	QVector<SDataToken> v_dt = m_disp->getDataTokens();
	QVector<SCodeToken> v_ct = m_disp->getCodeTokens();

	m_codegen->work(v_dt, v_ct, txtname);
}

