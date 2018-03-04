#include "codegenerator.h"

CCodeGenerator::CCodeGenerator()
{
}

class SIn
{
public:
	SIn() {}
	SIn(EInstruction i, int32 a, EType t) :
		instrCode(i), argumentCount(a), argumentType(t)
	{
	}
	EInstruction instrCode;
	int32 argumentCount;
	EType argumentType;
	friend bool operator==(const SIn& other, const SIn& other1)
	{
		return (other.argumentCount == other1.argumentCount) && (static_cast<int>(other.argumentType) == static_cast<int>(other1.argumentType))
			&& (static_cast<int>(other.instrCode) == static_cast<int>(other1.instrCode));
	}
};
void CCodeGenerator::work(QVector<SDataToken> v_dt, QVector<SCodeToken> v_ct, QString filename)
{
	QFile file(filename);
	file.resize(0);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream out(&file);

	for (int i = 0; i < v_ct.size(); ++i)
	{
		QString instruction;
		int count = v_ct[i].argValue.size();
		EType t = static_cast<EType>(v_ct[i].opcode.argSize);
		qDebug() << "static_cast<EType>(v_ct[i].opcode.argSize) = " << v_ct[i].opcode.argSize;
		
		qDebug() << "" << "v_ct[i].opcode.instr = " << v_ct[i].opcode.instr;
		
		EInstruction ins = static_cast<EInstruction>(v_ct[i].opcode.instr);
		SInstructionParameters sParam (ins, count, t);
		qDebug() << "instruction = " << inst.key(sParam);
	}
}











