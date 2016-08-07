#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QDebug>
#include "card.h"

class Deck : public QObject
{
    Q_OBJECT
public:
    Deck(QObject *parent = 0);
    Deck(int players);
    bool isEmpty();
    Card* popRandomCard();

signals:

public slots:

private:
    int randInt(int low, int high){
        return(qrand() % ((high + 1) - low) + low);
    }

    QList<Card*> m_deck;
};

#endif // DECK_H
