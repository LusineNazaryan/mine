#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QHash>

enum class EInstruction;
enum class EArgumentType;

using int8 = char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;

class CError : public std::exception
{
public:
	CError() : m_line(0)
	{
	}

	explicit CError(QString token) : m_error(token), m_line(0)
	{
	}

	explicit CError(QString token, uint32 l) : m_error(token), m_line(l)
	{
	}

	virtual QString ShowError() const
	{
		return m_error + QString::number(m_line);
	}
private:
	QString m_error;
	uint32 m_line;
};

union UOpcode
{
	UOpcode & operator=(const uint16 & val)
	{
		opcode = val;
		return *this;
	}

	//UOpcode(EInstruction op, EArgumentType at1, EArgumentType at2, EArgumentType at3, uint16 as) : opcode(op), arg1Type(at1), arg2Type(at2), arg3Type(at3), argSize(as)
	//{
	//}

	uint16 opcode;
	struct
	{
		uint16 instr : 8;
		union
		{
			uint16 argType : 6;
			struct
			{
				uint16 arg1Type : 2;
				uint16 arg2Type : 2;
				uint16 arg3Type : 2;
			};
		};
		uint16 argSize : 2;					//word,dword....
	};
};

enum class EArgumentType { None, Register, Data, Constant };

enum class EType { Byte, Word, Dword, Qword };
//enum class EType { Byte = 1, Word = 2, Dword = 4, Qword = 8 };

const QHash<QString, EType> HType =
{
	{ "byte", EType::Byte },{ "word", EType::Word },
{ "dword", EType::Dword },{ "qword", EType::Qword }
};

enum class EInstruction
{
	Invalid,
	Nop = 1,
	Break,
	Int,
	Jump,
	//Conditional instructions
	Je = 16,
	Jz = Je,
	Jne,
	Jnz = Jne,
	Ja,
	Jnbe = Ja,
	Jae,
	Jnb = Jae,
	Jb,
	Jnae = Jb,
	Jbe,
	Jna = Jbe,
	Jg,
	Jnle = Jg,
	Jge,
	Jnl = Jge,
	Jl,
	Jnge = Jl,
	Jle,
	Jng = Jle,
	Jo,
	Jno,
	Js,
	Jns,
	Jp,
	Jnp,

	// !Conditional instructions
	Call,
	Ret,
	Iret,
	Sti,
	Cli,
	Stc,
	Clc,
	Assign,
	Move,
	Swap,
	Add,
	Adc,
	Sub,
	Sbb,
	Mul,
	Imul,
	Div,
	Idiv,
	Inc,
	Dec,
	Neg,
	Cmp,
	And,
	Or,
	Xor,
	Nand,
	Nor,
	Not,
	Shr,
	Sar,
	Shl,
	Sal,
	Ror,
	Rcr,
	Rol,
	Rcl,
	Test,
	Load,
	Store,
	Push,
	Pop,
	Pushf,
	Popf,
	Pushsf,
	Popsf,
	In,
	Out,
};

struct SInstructionParameters
{
	SInstructionParameters() {}
	SInstructionParameters(EInstruction i, int32 a, EType t) :
		instrCode(i), argumentCount(a), argumentType(t)
	{
	}
	EInstruction instrCode;
	int32 argumentCount;
	EType argumentType;
	friend bool operator==(const SInstructionParameters& other, const SInstructionParameters& other1)
	{
		return (other.argumentCount == other1.argumentCount) && (other.argumentType == other1.argumentType) && (other.instrCode == other1.instrCode);
	}
};

const QHash<uint16, int> HInstCount =
{
	{ static_cast<uint16>(EInstruction::Add), 3 },
{ static_cast<uint16>(EInstruction::Assign), 2 },
{ static_cast<uint16>(EInstruction::Sub), 3 },
{ static_cast<uint16>(EInstruction::Mul), 3 }
};

const QHash<QString, SInstructionParameters> inst =
{
	//{ "invalid", SInstructionParameters(EInstruction::Invalid, 0, EType::Dword) },
	//{ "nop", UOpcode(EInstruction::Nop,EArgumentType::None,EArgumentType::None,EArgumentType::None,EType::Dword) },
	{ "nop", SInstructionParameters(EInstruction::Nop, 0, EType::Dword) },
{ "break", SInstructionParameters(EInstruction::Break, 1, EType::Dword) },
{ "int", SInstructionParameters(EInstruction::Int, 1, EType::Dword) },
{ "jump", SInstructionParameters(EInstruction::Jump, 1, EType::Dword) },
{ "call", SInstructionParameters(EInstruction::Call, 1, EType::Dword) },
{ "ret", SInstructionParameters(EInstruction::Ret, 0, EType::Dword) },
{ "iret", SInstructionParameters(EInstruction::Iret, 0, EType::Dword) },
{ "sti", SInstructionParameters(EInstruction::Sti, 1, EType::Dword) },
{ "cli", SInstructionParameters(EInstruction::Cli, 1, EType::Dword) },
{ "stc", SInstructionParameters(EInstruction::Stc, 1, EType::Dword) },
{ "clc", SInstructionParameters(EInstruction::Clc, 1, EType::Dword) },
{ "assignb", SInstructionParameters(EInstruction::Assign, 2, EType::Byte) },
{ "assignw", SInstructionParameters(EInstruction::Assign, 2, EType::Word) },
{ "assign", SInstructionParameters(EInstruction::Assign, 2, EType::Dword) },
{ "assignq", SInstructionParameters(EInstruction::Assign, 2, EType::Qword) },
{ "moveb", SInstructionParameters(EInstruction::Move, 2, EType::Byte) },
{ "movew", SInstructionParameters(EInstruction::Move, 2, EType::Word) },
{ "move", SInstructionParameters(EInstruction::Move, 2, EType::Dword) },
{ "moveq", SInstructionParameters(EInstruction::Move, 2, EType::Qword) },
{ "swapb", SInstructionParameters(EInstruction::Swap, 2, EType::Byte) },
{ "swapw", SInstructionParameters(EInstruction::Swap, 2, EType::Word) },
{ "swap", SInstructionParameters(EInstruction::Swap, 2, EType::Dword) },
{ "swapq", SInstructionParameters(EInstruction::Swap, 2, EType::Qword) },
{ "addb", SInstructionParameters(EInstruction::Add, 3, EType::Byte) },
{ "addw", SInstructionParameters(EInstruction::Add, 3, EType::Word) },
{ "add", SInstructionParameters(EInstruction::Add, 3, EType::Dword) },
{ "addq", SInstructionParameters(EInstruction::Add, 3, EType::Qword) },
{ "adcb", SInstructionParameters(EInstruction::Adc, 3, EType::Byte) },
{ "adcw", SInstructionParameters(EInstruction::Adc, 3, EType::Word) },
{ "adc", SInstructionParameters(EInstruction::Adc, 3, EType::Dword) },
{ "adcq", SInstructionParameters(EInstruction::Adc, 3, EType::Qword) },
{ "subb", SInstructionParameters(EInstruction::Sub, 3, EType::Byte) },
{ "subw", SInstructionParameters(EInstruction::Sub, 3, EType::Word) },
{ "sub", SInstructionParameters(EInstruction::Sub, 3, EType::Dword) },
{ "subq", SInstructionParameters(EInstruction::Sub, 3, EType::Qword) },
{ "sbbb", SInstructionParameters(EInstruction::Sbb, 3, EType::Byte) },
{ "sbbw", SInstructionParameters(EInstruction::Sbb, 3, EType::Word) },
{ "sbb", SInstructionParameters(EInstruction::Sbb, 3, EType::Dword) },
{ "sbbq", SInstructionParameters(EInstruction::Sbb, 3, EType::Qword) },
{ "mulb", SInstructionParameters(EInstruction::Mul, 3, EType::Byte) },
{ "mulw", SInstructionParameters(EInstruction::Mul, 3, EType::Word) },
{ "mul", SInstructionParameters(EInstruction::Mul, 3, EType::Dword) },
{ "mulq", SInstructionParameters(EInstruction::Mul, 3, EType::Qword) },
{ "imulb", SInstructionParameters(EInstruction::Imul, 3, EType::Byte) },
{ "imulw", SInstructionParameters(EInstruction::Imul, 3, EType::Word) },
{ "imul", SInstructionParameters(EInstruction::Imul, 3, EType::Dword) },
{ "imulq", SInstructionParameters(EInstruction::Imul, 3, EType::Qword) },
{ "divb", SInstructionParameters(EInstruction::Div, 3, EType::Byte) },
{ "divw", SInstructionParameters(EInstruction::Div, 3, EType::Word) },
{ "div", SInstructionParameters(EInstruction::Div, 3, EType::Dword) },
{ "divq", SInstructionParameters(EInstruction::Div, 3, EType::Qword) },
{ "idivb", SInstructionParameters(EInstruction::Idiv, 3, EType::Byte) },
{ "idivw", SInstructionParameters(EInstruction::Idiv, 3, EType::Word) },
{ "idiv", SInstructionParameters(EInstruction::Idiv, 3, EType::Dword) },
{ "idivq", SInstructionParameters(EInstruction::Idiv, 3, EType::Qword) },
{ "incb", SInstructionParameters(EInstruction::Inc, 1, EType::Byte) },
{ "incw", SInstructionParameters(EInstruction::Inc, 1, EType::Word) },
{ "inc", SInstructionParameters(EInstruction::Inc, 1, EType::Dword) },
{ "incq", SInstructionParameters(EInstruction::Inc, 1, EType::Qword) },
{ "decb", SInstructionParameters(EInstruction::Dec, 1, EType::Byte) },
{ "decw", SInstructionParameters(EInstruction::Dec, 1, EType::Word) },
{ "dec", SInstructionParameters(EInstruction::Dec, 1, EType::Dword) },
{ "decq", SInstructionParameters(EInstruction::Dec, 1, EType::Qword) },
{ "negb", SInstructionParameters(EInstruction::Neg, 1, EType::Byte) },
{ "negw", SInstructionParameters(EInstruction::Neg, 1, EType::Word) },
{ "neg", SInstructionParameters(EInstruction::Neg, 1, EType::Dword) },
{ "negq", SInstructionParameters(EInstruction::Neg, 1, EType::Qword) },
{ "cmpb", SInstructionParameters(EInstruction::Cmp, 2, EType::Byte) },
{ "cmpw", SInstructionParameters(EInstruction::Cmp, 2, EType::Word) },
{ "cmp", SInstructionParameters(EInstruction::Cmp, 2, EType::Dword) },
{ "cmpq", SInstructionParameters(EInstruction::Cmp, 2, EType::Qword) },
{ "andb", SInstructionParameters(EInstruction::And, 3, EType::Byte) },
{ "andw", SInstructionParameters(EInstruction::And, 3, EType::Word) },
{ "and", SInstructionParameters(EInstruction::And, 3, EType::Dword) },
{ "andq", SInstructionParameters(EInstruction::And, 3, EType::Qword) },
{ "orb", SInstructionParameters(EInstruction::Or, 3, EType::Dword) },
{ "orw", SInstructionParameters(EInstruction::Or, 3, EType::Word) },
{ "or", SInstructionParameters(EInstruction::Or, 3, EType::Dword) },
{ "orq", SInstructionParameters(EInstruction::Or, 3, EType::Qword) },
{ "xorb", SInstructionParameters(EInstruction::Xor, 3, EType::Byte) },
{ "xorw", SInstructionParameters(EInstruction::Xor, 3, EType::Word) },
{ "xor", SInstructionParameters(EInstruction::Xor, 3, EType::Dword) },
{ "xorq", SInstructionParameters(EInstruction::Xor, 3, EType::Qword) },
{ "nandb", SInstructionParameters(EInstruction::Nand, 3, EType::Byte) },
{ "nandw", SInstructionParameters(EInstruction::Nand, 3, EType::Word) },
{ "nand", SInstructionParameters(EInstruction::Nand, 3, EType::Dword) },
{ "nandq", SInstructionParameters(EInstruction::Nand, 3, EType::Qword) },
{ "norb", SInstructionParameters(EInstruction::Nor, 3, EType::Byte) },
{ "norw", SInstructionParameters(EInstruction::Nor, 3, EType::Word) },
{ "nor", SInstructionParameters(EInstruction::Nor, 3, EType::Dword) },
{ "norq", SInstructionParameters(EInstruction::Nor, 3, EType::Qword) },
{ "notb", SInstructionParameters(EInstruction::Not, 1, EType::Byte) },
{ "notw", SInstructionParameters(EInstruction::Not, 1, EType::Word) },
{ "not", SInstructionParameters(EInstruction::Not, 1, EType::Dword) },
{ "notq", SInstructionParameters(EInstruction::Not, 1, EType::Qword) },
{ "shrb", SInstructionParameters(EInstruction::Shr, 2, EType::Byte) },
{ "shrw", SInstructionParameters(EInstruction::Shr, 2, EType::Word) },
{ "shr", SInstructionParameters(EInstruction::Shr, 2, EType::Dword) },
{ "shrq", SInstructionParameters(EInstruction::Shr, 2, EType::Qword) },
{ "sarb", SInstructionParameters(EInstruction::Sar, 2, EType::Byte) },
{ "sarw", SInstructionParameters(EInstruction::Sar, 2, EType::Word) },
{ "sar", SInstructionParameters(EInstruction::Sar, 2, EType::Dword) },
{ "sarq", SInstructionParameters(EInstruction::Sar, 2, EType::Qword) },
{ "shlb", SInstructionParameters(EInstruction::Shl, 2, EType::Byte) },
{ "shlw", SInstructionParameters(EInstruction::Shl, 2, EType::Word) },
{ "shl", SInstructionParameters(EInstruction::Shl, 2, EType::Dword) },
{ "shlq", SInstructionParameters(EInstruction::Shl, 2, EType::Qword) },
{ "salb", SInstructionParameters(EInstruction::Sal, 2, EType::Byte) },
{ "salw", SInstructionParameters(EInstruction::Sal, 2, EType::Word) },
{ "sal", SInstructionParameters(EInstruction::Sal, 2, EType::Dword) },
{ "salq", SInstructionParameters(EInstruction::Sal, 2, EType::Qword) },
{ "rorb", SInstructionParameters(EInstruction::Ror, 2, EType::Byte) },
{ "rorw", SInstructionParameters(EInstruction::Ror, 2, EType::Word) },
{ "ror", SInstructionParameters(EInstruction::Ror, 2, EType::Dword) },
{ "rorq", SInstructionParameters(EInstruction::Ror, 2, EType::Qword) },
{ "rcrb", SInstructionParameters(EInstruction::Rcr, 2, EType::Byte) },
{ "rcrw", SInstructionParameters(EInstruction::Rcr, 2, EType::Word) },
{ "rcr", SInstructionParameters(EInstruction::Rcr, 2, EType::Dword) },
{ "rcrq", SInstructionParameters(EInstruction::Rcr, 2, EType::Qword) },
{ "rolb", SInstructionParameters(EInstruction::Rol, 2, EType::Byte) },
{ "rolw", SInstructionParameters(EInstruction::Rol, 2, EType::Word) },
{ "rol", SInstructionParameters(EInstruction::Rol, 2, EType::Dword) },
{ "rolq", SInstructionParameters(EInstruction::Rol, 2, EType::Qword) },
{ "rclb", SInstructionParameters(EInstruction::Rcl, 2, EType::Byte) },
{ "rclw", SInstructionParameters(EInstruction::Rcl, 2, EType::Word) },
{ "rcl", SInstructionParameters(EInstruction::Rcl, 2, EType::Dword) },
{ "rclq", SInstructionParameters(EInstruction::Rcl, 2, EType::Qword) },
{ "testb", SInstructionParameters(EInstruction::Test, 2, EType::Byte) },
{ "testw", SInstructionParameters(EInstruction::Test, 2, EType::Word) },
{ "test", SInstructionParameters(EInstruction::Test, 2, EType::Dword) },
{ "testq", SInstructionParameters(EInstruction::Test, 2, EType::Qword) },
{ "loadb", SInstructionParameters(EInstruction::Load, 2, EType::Byte) },
{ "loadw", SInstructionParameters(EInstruction::Load, 2, EType::Word) },
{ "load", SInstructionParameters(EInstruction::Load, 2, EType::Dword) },
{ "loadq", SInstructionParameters(EInstruction::Load, 2, EType::Qword) },
{ "storeb", SInstructionParameters(EInstruction::Store, 2, EType::Byte) },
{ "storew", SInstructionParameters(EInstruction::Store, 2, EType::Word) },
{ "store", SInstructionParameters(EInstruction::Store, 2, EType::Dword) },
{ "storeq", SInstructionParameters(EInstruction::Store, 2, EType::Qword) },
{ "pushb", SInstructionParameters(EInstruction::Push, 1, EType::Byte) },
{ "pushw", SInstructionParameters(EInstruction::Push, 1, EType::Word) },
{ "push", SInstructionParameters(EInstruction::Push, 1, EType::Dword) },
{ "pushq", SInstructionParameters(EInstruction::Push, 1, EType::Qword) },
{ "popb", SInstructionParameters(EInstruction::Pop, 1, EType::Byte) },
{ "popw", SInstructionParameters(EInstruction::Pop, 1, EType::Word) },
{ "pop", SInstructionParameters(EInstruction::Pop, 1, EType::Dword) },
{ "popq", SInstructionParameters(EInstruction::Pop, 1, EType::Qword) },
{ "pushf", SInstructionParameters(EInstruction::Pushf, 0, EType::Dword) },
{ "popf", SInstructionParameters(EInstruction::Popf, 0, EType::Dword) },
{ "pushsf", SInstructionParameters(EInstruction::Pushsf, 0, EType::Dword) },
{ "popsf", SInstructionParameters(EInstruction::Popsf, 0, EType::Dword) },
{ "inb", SInstructionParameters(EInstruction::In, 2, EType::Dword) },
{ "inw", SInstructionParameters(EInstruction::In, 2, EType::Word) },
{ "in", SInstructionParameters(EInstruction::In, 2, EType::Dword) },
{ "inq", SInstructionParameters(EInstruction::In, 2, EType::Qword) },
{ "outb", SInstructionParameters(EInstruction::Out, 2, EType::Byte) },
{ "outw", SInstructionParameters(EInstruction::Out, 2, EType::Word) },
{ "out", SInstructionParameters(EInstruction::Out, 2, EType::Dword) },
{ "outq", SInstructionParameters(EInstruction::Out, 2, EType::Qword) },
};

#endif // !INSTRUCTIONS_H

