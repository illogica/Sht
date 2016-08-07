#include "card.h"

Card::Card(QObject *parent) : QObject(parent)
{}

Card::Card(QObject *parent, int type, int value)
    : QObject(parent), m_type(type), m_value(value)
{
    this->m_name = toString();
}

QString Card::toString()
{
    QString result;
    switch(m_type){
    case DENARI:
        result.append("D");
        break;
    case BASTONI:
        result.append("B");
        break;
    case COPPE:
        result.append("C");
        break;
    case SPADE:
        result.append("S");
        break;
    }

    result.append(QString::number(m_value));
    return result;
}

bool Card::operator ==(const Card &c)
{
    return c.m_type == this->m_type && c.m_value==this->m_value;
}

int Card::type() const
{
    return m_type;
}

void Card::setType(int type)
{
    m_type = type;
    m_name = toString();
}

int Card::value() const
{
    return m_value;
}

void Card::setValue(int value)
{
    m_value = value;
    m_name = toString();
}

QString Card::name() const
{
    return m_name;
}
