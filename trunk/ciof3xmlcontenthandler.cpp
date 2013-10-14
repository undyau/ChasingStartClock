#include "ciof3xmlcontenthandler.h"
#include "crunner.h"
#include <QTime>
#include <QDebug>
#include "calert.h"

CIof3XmlContentHandler::CIof3XmlContentHandler(QMap<QDateTime, CRunner*>& a_AllRunners, CAlert* a_Alerter):
    QXmlDefaultHandler(),m_AllRunners(a_AllRunners), m_Alerter(a_Alerter)
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
    else if (m_Start.size() > 19)  // throw away tz info - QT can't handle it ?
        m_Start = m_Start.left(19);

    QDateTime date = QDateTime::fromString(m_Start, "yyyy-MM-ddTHH:mm:ss");

    date.setTimeSpec(Qt::UTC);
    QDateTime local = date.toLocalTime();

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
  qDebug() << eventDate << today << a;
  if (eventDate != today)
      m_Alerter->setMessage("Start list is for another day - " + a);
}
