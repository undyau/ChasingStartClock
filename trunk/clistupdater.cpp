#include "clistupdater.h"
#include "crunner.h"
#include <QDebug>
#include <QtXml/QXmlSimpleReader>
#include "ciof3xmlcontenthandler.h"
#include "cconfiguration.h"

CListUpdater::CListUpdater(QQmlContext *a_Context,  CConfiguration* a_Config, QObject *parent) :
    QObject(parent), m_Timer(this), m_Context(a_Context), m_Config(a_Config)
{
    m_Context->setContextProperty("myConfig", m_Config);
    m_Context->setContextProperty("myUpdater", this);
    Reload();
    connect(m_Config, SIGNAL(fileChanged()), this, SLOT(Reload()));
    connect(m_Config, SIGNAL(lookAheadChanged()), this, SLOT(Update()));
    connect(m_Config, SIGNAL(staleChanged()), this, SLOT(Update()));
    connect(m_Config, SIGNAL(maxDisplayChanged()), this, SLOT(Update()));
}

/*void CListUpdater::FakeStartList()
{
    for (int i = 0; i < 100; i++)
    {
    QDateTime date = QDateTime::fromString(m_Start, "yyyy-MM-ddTHH:mm:ss");

    date.setTimeSpec(Qt::UTC);
    QDateTime local = date.toLocalTime();

    if (local.isValid() && (m_FName.size() > 0 || m_SName.size() > 0))
    {
        CRunner* runner = new CRunner(m_FName + " " + m_SName, local, m_Class);
}*/

bool CListUpdater::LoadRunners(QString& a_FileName)
{
    if (a_FileName.isEmpty())
        return false;
    if (a_FileName.compare("Fake", Qt::CaseInsensitive) == 0)
    {
        //FakeStartList();
    }
    else
    {
        QXmlSimpleReader parser;
        CIof3XmlContentHandler* handler = new CIof3XmlContentHandler(m_AllRunners);
        parser.setContentHandler(handler);

        if(parser.parse(new QXmlInputSource(new QFile(a_FileName))))
            return true;
        else
            return false;
    }
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

void CListUpdater::Reload()
{
    m_Timer.stop();

    m_DisplayList.clear();
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i!= m_AllRunners.end(); i++)
        delete i.value();
    m_AllRunners.clear();

    if (LoadRunners(m_Config->file()))
    {
        GetDisplayList(m_DisplayList);
        for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i != m_AllRunners.end(); i++)
        {
            CRunner* runner = qobject_cast<CRunner*>(i.value());
            if (runner)
                connect(&m_Timer, SIGNAL(timeout()), runner, SLOT(updateTimeleft()));
        }
    }

    UpdateDisplayList();

    connect(&m_Timer, SIGNAL(timeout()),this, SLOT(Update()));
    m_Timer.start(100);
}

void CListUpdater::GetDisplayList(QList<QObject*>& a_List)
{
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i!= m_AllRunners.end(); i++)
    {
        if (i.value()->timeleft().toInt() > m_Config->stale() &&
                i.value()->timeleft().toInt() <= m_Config->lookAhead() *60 &&
                a_List.size() < m_Config->maxDisplay())
            a_List.append(i.value());
    }
}

void CListUpdater::UpdateDisplayList()
{
    qDebug() << "At 1, m_DisplayList has" << m_DisplayList.size();
    m_Context->setContextProperty("myModel", QVariant::fromValue(m_DisplayList));
    qDebug() << "At 2";
}
