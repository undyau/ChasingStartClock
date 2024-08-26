#include "ciof3xmlcontenthandler.h"
#include "crunner.h"
#include <QTime>
#include <QDebug>
#include "calert.h"

CIof3XmlContentHandler::CIof3XmlContentHandler(QMap<QDateTime, CRunner*>& a_AllRunners, CAlert* a_Alerter, bool a_TimesAreUtc):
    QXmlDefaultHandler(),m_AllRunners(a_AllRunners), m_Alerter(a_Alerter), m_TimesAreUtc(a_TimesAreUtc)
{
}


CIof3XmlContentHandler::~CIof3XmlContentHandler()
{

}

bool CIof3XmlContentHandler::endElement(const QString& , const QString &localName, const QString &)
{
    if (localName != m_Elements.last() && m_Elements.size() > 0)
        qDebug() << "ending element" << localName << "expected" << m_Elements.last();
    else
        m_Elements.pop_back();

    if (localName == "PersonStart")
        AddPerson();
   // if (localName == "Class")
   //     m_Class.clear();
    return true;
}

bool CIof3XmlContentHandler::characters(const QString &ch)
{
    if (m_Elements.size() > 1 && m_Elements.last() == "Name" && m_Elements[m_Elements.size()-2] == "Class")
        m_Class = ch;
    if (m_Elements.size() > 2 && m_Elements.last() == "Family" && m_Elements[m_Elements.size()-2] == "Name" && m_Elements[m_Elements.size()-3] == "Person")
        m_SName = ch;
    if (m_Elements.size() > 2 && m_Elements.last() == "Given" && m_Elements[m_Elements.size()-2] == "Name" && m_Elements[m_Elements.size()-3] == "Person")
        m_FName = ch;
    if (m_Elements.size() > 2 && m_Elements.last() == "StartTime" && m_Elements[m_Elements.size()-2] == "Start" && m_Elements[m_Elements.size()-3] == "PersonStart")
        m_Start = ch;
    if (m_Elements.size() > 2 && m_Elements.last() == "Date" && m_Elements[m_Elements.size()-2] == "StartTime" && m_Elements[m_Elements.size()-3] == "Event")
        ValidateDate(ch);


    return true;
}

bool CIof3XmlContentHandler::startElement(const QString & , const QString & localName,
                const QString &, const QXmlAttributes &  )
{
    m_Elements.append(localName);
    return true;
}

void CIof3XmlContentHandler::AddPerson()
{
    if (m_Start.size() < 19)
        return; // unknown time format

    int tpos, pos(-1),hours(0),mins(0);
    QDateTime date;
    tpos = m_Start.indexOf('T');
    if (( pos = m_Start.indexOf('+')) > 0 && m_Start.size() >= pos + 6)
        {
        hours = -m_Start.mid(pos+1,2).toInt();
        mins = -m_Start.mid(pos+3,2).toInt();
        }
    else if (( pos = m_Start.indexOf('-', tpos)) > 0 && m_Start.size() >= pos + 6)
        {
        hours = m_Start.mid(pos+1,2).toInt();
        mins = m_Start.mid(pos+3,2).toInt();
        }

    date = QDateTime::fromString(m_Start.left(19), "yyyy-MM-ddTHH:mm:ss");
    date = date.addSecs(((hours*60)+mins)*60);
    QDateTime local;
    if (m_TimesAreUtc)
        {
        date.setTimeSpec(Qt::UTC);
        local = date.toLocalTime();
        }
    else
        {
        date.setTimeSpec(Qt::LocalTime);
        local = date;
        }

    if (local.isValid() && (m_FName.size() > 0 || m_SName.size() > 0))
    {
        CRunner* runner = new CRunner(m_FName + " " + m_SName, local, m_Class);
        while (m_AllRunners.find(local) != m_AllRunners.end())  // disambiguate common start times
            local = local.addMSecs(1);
        m_AllRunners[local] = runner;
    }
    m_FName.clear();
    m_SName.clear();
    m_Start.clear();
}

void CIof3XmlContentHandler::ValidateDate(QString a)
{
  QDate eventDate = QDate::fromString(a, "yyyy-MM-dd");
  QDate today = QDate::currentDate();
  if (eventDate != today)
      m_Alerter->setMessage("Start list is for another day - " + a);
    else
      m_Alerter->clearMessage();
}
