#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card : public QObject
{
    Q_OBJECT
public:
    Card(QObject *parent = 0);
    Card(QObject *parent = 0, int m_type=0, int m_value=0);
    QString toString();

    bool operator ==(const Card &c);

    enum types{DENARI=0, BASTONI=1, COPPE=2, SPADE=3};
    enum values{ASSO=1, DUE=2, TRE=3, QUATTRO=4, CINQUE=5, SEI=6, SETTE=7, FANTE=8, CAVALLO=9, RE=10};

    int type() const;
    void setType(int type);

    int value() const;
    void setValue(int value);

    QString name() const;

signals:

public slots:

private:
    int m_type;
    int m_value;
    QString m_name;

};

#endif // CARD_H
