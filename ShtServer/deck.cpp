#include "deck.h"
#include <QTime>

Deck::Deck(QObject *parent) : QObject(parent)
{
    qsrand(QTime::currentTime().msec());
    for(int i=0; i<4; i++){
        for(int j=1; j<=10; j++){
            Card *c = new Card(this, i, j);
            //qWarning() << "generated card " << c->toString();
            m_deck.append(c);
        }
    }
}

Deck::Deck(int players)
{
    qsrand(QTime::currentTime().msec());
    int limit = players < 4 ? players : 4;
    for(int i=0; i<limit; i++){
        for(int j=1; j<=players; j++){
            Card *c = new Card(this, i, j);
            m_deck.append(c);
        }
    }
}

bool Deck::isEmpty()
{
    return m_deck.isEmpty();
}

Card *Deck::popRandomCard()
{
    if(m_deck.length()>0){
        int at = randInt(0, m_deck.length()-1);
        Card *c = m_deck.at(at);
        m_deck.removeOne(c);
        return c;
    }
    return nullptr;
}
