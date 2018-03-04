#ifndef LEXER_H
#define LEXER_H

#include <QVarLengthArray>
#include <QDebug>
#include <QByteArray>
#include "instructions.h"
#include "tokens.h"
#include <QDataStream>

class CLexer
{
public:

	void work(QTextStream& in);

	QVector<SDataToken> getData() const;
	QVector<SCodeToken> getCode() const;
	QVector<QString> getDataTable() const;

	void p()
	{
		for (auto a : m_data)
		{
			qDebug() << a;
		}
	}

private:
	enum class EState
	{
		Init, CodeCommand, CodeArgument, DataType, DataIdentifier,
		DataValue, Comment
	};

	bool isDigit(const QString &);
	bool isRegister(const QString &);

	void checkCommand(const QString& command);
	void checkArgument(const QString& argument);
	void checkIdentifier(const QString& identifier);
	void checkValue(const QString& value);
	bool checkDelimiter(QChar ch, EState);
	bool IsAlNum(QString ch);
	void checkType(const QString& type_string);
	void setArgType(EArgumentType argType);
	uint32 getRegNumber(const QString & argument);

	uint32 m_line = 1;
	int32 m_argCount = 0;
	QVector<SDataToken> m_dataTokens;
	QVector<SCodeToken> m_codeTokens;
	QVector<QString> m_data;
	uint32 m_dataOffset = 0;
	QVector<QString> ErrorLog;
};



class CLexerError : public CError
{
public:
	explicit CLexerError(const QString& t, uint32 l)
		: m_token(t), m_errorLine(l)
	{
	}
	virtual QString ShowError() const override
	{
		return m_token + " in line " + QString::number(m_errorLine) + " can't be used as " + m_errorType;
	}
protected:
	QString m_token;
	uint32 m_errorLine;
	QString m_errorType;
};

class CommandError : public CLexerError
{
	explicit CommandError(const QString& t, uint32 l) : CLexerError(t, l)
	{
		m_errorType = "command";
	}
};

class TypeError : public CLexerError
{
	explicit TypeError(const QString& t, uint32 l) : CLexerError(t, l)
	{
		m_errorType = "type";
	}
};

#endif // !LEXER_H
