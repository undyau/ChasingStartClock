#include "crunner.h"
#include <QDebug>
#include <QDateTime>

CRunner::CRunner(QObject *parent) :
    QObject(parent)
{
}

CRunner::CRunner(const QString &name,  const QDateTime &start, const QString &oclass,QObject *parent)
    : QObject(parent), m_name(name), m_oclass(oclass), m_start(start)
{
    updateTimeleft();
}

QString CRunner::name() const
{
    return m_name;
}

QString CRunner::color() const
{
    return m_color;
}

QString CRunner::timeleft() const
{
    return m_timeleft;
}

void CRunner::setTimeleft(const QString &timeleft)
{
    if (timeleft != m_timeleft) {
        m_timeleft = timeleft;
        emit timeleftChanged();
    }
    if (timeleft.toInt() > 10)
        setColor("white");
    else if (timeleft.toInt() > 0)
        setColor("yellow");
    else
        setColor("red");
}

void CRunner::setColor(const QString &color)
{
    if (color != m_color) {
        m_color = color;
        emit colorChanged();
    }
}

void CRunner::updateTimeleft()
{
    int secs = QDateTime::currentDateTime().secsTo(m_start);
    QString secsStr = QString("%1").arg(secs);
    setTimeleft(secsStr);
}

QString CRunner::oclass() const
{
    return m_oclass;
}

QString CRunner::start() const
{
    return m_start.toString("HH:mm:ss");
}
