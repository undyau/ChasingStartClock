#include "calert.h"
#include <QDebug>

CAlert::CAlert(QObject *parent) :
    QObject(parent)
{
}

void CAlert::setMessage(const QString &a)
{
    if (m_Message != a)
    {
        m_Message = a;
        emit messageChanged();
//        qDebug() << "Sent alert";
    }
}

void CAlert::clearMessage()
{
    setMessage("");
}
