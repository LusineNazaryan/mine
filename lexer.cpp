#include "lexer.h"

void CLexer::work(QTextStream & in)
{
	EState state = EState::Init;
	EState n_state = EState::Init;
	EState p_state = EState::Init;
	QString buffer;
	m_line = 1;
	while (!in.atEnd())
	{
		QChar ch;
		in >> ch;
		if (ch == '\n')
		{
			++m_line;
		}

		if (ch == ';')
		{
			n_state = state;
			state = EState::Comment;
		}

		if (checkDelimiter(ch, state))
		{
			if (buffer == ".code")
			{
				state = EState::CodeCommand;
				n_state = EState::CodeArgument;
				buffer.clear();
			}
			if (buffer.isEmpty())
			{
				continue;
			}

			p_state = state;

			switch (state)
			{
			case EState::Init:
				if (buffer == ".data")
				{
					state = EState::DataType;
					buffer.clear();
				}
				break;
			case EState::CodeCommand:
				checkCommand(buffer);
				if (m_argCount)
				{
					state = EState::CodeArgument;
				}
				break;
			case EState::CodeArgument:
				checkArgument(buffer);
				if (m_argCount)
				{
					state = EState::CodeArgument;
				}
				else
				{
					state = EState::CodeCommand;
				}
				break;
			case EState::DataType:
				checkType(buffer);
				p_state = state;
				state = EState::DataIdentifier;
				break;
			case EState::DataIdentifier:
				checkIdentifier(buffer);
				state = EState::DataValue;
				break;
			case EState::DataValue:
				checkValue(buffer);
				state = EState::DataType;
				break;
			case EState::Comment:
				if (ch == '\n')
				{
					state = n_state;
				}
				break;
			}
			buffer.clear();
		}
		else
		{
			buffer += ch;
		}
	}
}

QVector<SDataToken> CLexer::getData() const
{
	return m_dataTokens;
}

QVector<SCodeToken> CLexer::getCode() const
{
	return m_codeTokens;
}

QVector<QString> CLexer::getDataTable() const
{
	return m_data;
}

bool CLexer::isDigit(const QString & token)
{
	for (int i = 0; i < token.size(); i++)
	{
		if (!token[i].isDigit())
		{
			return false;
		}
	}
	return true;
}

void CLexer::checkCommand(const QString & command)
{
	if (inst.contains(command))
	{
		m_codeTokens.push_back(SCodeToken(m_line));
		m_argCount = inst[command].argumentCount;
		m_codeTokens.last().opcode.instr = static_cast<uint16>(inst[command].instrCode);
		m_codeTokens.last().opcode.argSize = static_cast<uint16>(inst[command].argumentType);
	}
	else
	{
		throw CError(command + " on line " + QString::number(m_line) + " can't be used as command");
	}
}

void CLexer::checkArgument(const QString & argument)
{
	m_argCount--;
	if (isRegister(argument))
	{
		m_codeTokens.last().argValue.push_back(getRegNumber(argument));
		setArgType(EArgumentType::Register);
	}
	else if (m_data.contains(argument))
	{
		m_codeTokens.last().argValue.push_back(m_data.indexOf(argument));
		setArgType(EArgumentType::Data);
	}
	else if (isDigit(argument))
	{
		m_codeTokens.last().argValue.push_back(argument.toInt());
		setArgType(EArgumentType::Constant);
	}
	else
	{
		throw CError(argument + " on line " + QString::number(m_line) + " can't be used as argument");
	}
}

void CLexer::checkIdentifier(const QString & identifier)
{
	if (isRegister(identifier) || inst.contains(identifier) || HType.contains(identifier) || m_data.contains(identifier))
	{
		throw CError(identifier + " on line " + QString::number(m_line) + " can't be used as identifier");
	}

	if (identifier.contains('[') && identifier.contains(']'))
	{
		QString tmp = identifier.split('[')[1].split(']')[0];
		if (!tmp.isEmpty())
		{
			m_dataTokens.last().quantity = tmp.toInt();
		}
		else
		{
			m_dataTokens.last().quantity = -2;
		}
	}
	else if (IsAlNum(identifier))
	{
		m_dataTokens.last().quantity = -1;
	}
	else
	{

	}
	m_dataTokens.last().identifierName = identifier;
	m_data.push_back(identifier);
}

void CLexer::checkValue(const QString & value)
{
	const int32 count = m_dataTokens.last().quantity;
	if (count == -1)
	{
		if (isDigit(value))
		{
			QDataStream tmp(&m_dataTokens.last().value, QIODevice::WriteOnly);

			switch (m_dataTokens.last().type)
			{
			case EType::Byte:
			{
				uint8 val = value.toUShort();
				tmp << val;
				break;
			}
			case EType::Word:
				tmp << value.toShort();
				break;
			case EType::Dword:
				tmp << value.toInt();
				break;
			case EType::Qword:
				tmp << value.toLongLong();
				break;
			default:;
			}
		}
		else
		{
			throw CError("not a number");
		}
	}
	else if (count == -2)
	{
		/*QStringList arrElements = value.split(',');
		QVarLengthArray<uint32> arr;
		for (const QString& a : arrElements)
		{
		arr.push_back(a.toInt());
		}
		m_dataTokens.last().count = arr.size();
		m_dataTokens.last().value = arr;*/
	}
}

bool CLexer::isRegister(const QString & token)
{
	if (!isDigit(token.right(token.size() - 1)))
	{
		return false;
	}

	if (token.size() <= 1)
	{
		return false;
	}

	int reg_num = token.right(token.size() - 1).toInt();

	if ((token[0] == 'r') && (reg_num >= 0 && reg_num <= 1023))
	{
		return true;
	}
	else if ((token[0] == 'a') && (reg_num >= 0 && reg_num <= 3))
	{
		return true;
	}

	return false;
}

void CLexer::checkType(const QString& type_string)
{
	if (HType.contains(type_string))
	{
		m_dataTokens.push_back(SDataToken(HType[type_string], m_line));
	}
	else
	{
		throw CError(type_string + " on line " + QString::number(m_line) + "can't be used as type");
	}
}

void CLexer::setArgType(EArgumentType argType)
{
	switch (m_codeTokens.last().argValue.size())
	{
	case 1:
		m_codeTokens.last().opcode.arg1Type = static_cast<uint16>(argType);
		break;
	case 2:
		m_codeTokens.last().opcode.arg2Type = static_cast<uint16>(argType);
		break;
	case 3:
		m_codeTokens.last().opcode.arg3Type = static_cast<uint16>(argType);
		break;
	}
}

uint32 CLexer::getRegNumber(const QString & token)
{
	int reg_num = token.right(token.size() - 1).toInt();

	if ((token[0] == 'r') && (reg_num >= 0 && reg_num <= 1023))
	{
		return reg_num + 9;
	}
	else if ((token[0] == 'a') && (reg_num >= 0 && reg_num <= 3))
	{
		return reg_num;
	}
	else if (token == "IP")
	{
		return 4;
	}
	else if (token == "TR")
	{
		return 5;
	}
	else if (token == "SP")
	{
		return 6;
	}
	else if (token == "SF")
	{
		return 7;
	}
	else if (token == "FLAGS")
	{
		return 8;
	}
}

bool CLexer::checkDelimiter(QChar ch, EState state)
{
	switch (state)
	{
	case EState::Init: break;
	case EState::CodeCommand: break;
	case EState::CodeArgument: break;
	case EState::DataType:
		if (ch == ' ' || ch == '\t')
		{
			return true;
		}
		else
			break;
	case EState::DataIdentifier: break;
	case EState::DataValue: break;
	case EState::Comment: break;
	default:;
	}
	if (ch == ' ' || ch == '\t' || ch == '\n' || ch == ',' || ch == '=' || ch == ';')
	{
		return true;
	}
	return false;
}

bool CLexer::IsAlNum(QString ch)
{
	for (const QChar& a : ch)
	{
		if (!a.isLetterOrNumber())
		{
			return false;
		}
	}
	return true;
}
