#include "clistupdater.h"
#include "crunner.h"
#include <QDebug>
#include <QtXml/QXmlSimpleReader>
#include "ciof3xmlcontenthandler.h"

CListUpdater::CListUpdater(QQmlContext *a_Context, QString a_FileName, int a_LookAhead,
                           int a_Stale, int a_MaxDisplay, QObject *parent) :
    QObject(parent), m_Timer(this), m_Context(a_Context), m_Stale(a_Stale), m_Lookahead(a_LookAhead),
    m_MaxDisplay(a_MaxDisplay)
{
    LoadRunners(a_FileName);
    GetDisplayList(m_DisplayList);
    UpdateDisplayList();
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i != m_AllRunners.end(); i++)
    {
        CRunner* runner = qobject_cast<CRunner*>(i.value());
        if (runner)
            connect(&m_Timer, SIGNAL(timeout()), runner, SLOT(updateTimeleft()));
    }
    connect(&m_Timer, SIGNAL(timeout()),this, SLOT(Update()));
    m_Timer.start(100);
}


bool CListUpdater::LoadRunners(QString& a_FileName)
{
    QXmlSimpleReader parser;
    CIof3XmlContentHandler* handler = new CIof3XmlContentHandler(m_AllRunners);
    parser.setContentHandler(handler);

    if(parser.parse(new QXmlInputSource(new QFile(a_FileName))))
        return true;
    else
        return false;
}



void CListUpdater::Update()
{
    QList<QObject*> nowList;
    GetDisplayList(nowList);
    if (nowList != m_DisplayList)
    {
        m_DisplayList = nowList;
        UpdateDisplayList();
    }
}

void CListUpdater::GetDisplayList(QList<QObject*>& a_List)
{
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i!= m_AllRunners.end(); i++)
    {
        if (i.value()->timeleft().toInt() > m_Stale &&
                i.value()->timeleft().toInt() <= m_Lookahead &&
                a_List.size() < m_MaxDisplay)
            a_List.append(i.value());
    }
}

void CListUpdater::UpdateDisplayList()
{
    m_Context->setContextProperty("myModel", QVariant::fromValue(m_DisplayList));
}
