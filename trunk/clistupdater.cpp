#include "clistupdater.h"
#include "crunner.h"
#include <QDebug>
#include <QtXml/QXmlSimpleReader>
#include "ciof3xmlcontenthandler.h"
#include "cconfiguration.h"
#include "calert.h"
#include <QThreadPool>
#include "cplaysound.h"

CListUpdater::CListUpdater(QQmlContext *a_Context,  CConfiguration* a_Config, CAlert* a_Alert, QObject *parent) :
    QObject(parent), m_Timer(this), m_Context(a_Context), m_Config(a_Config), m_Alert(a_Alert)
{
    m_Context->setContextProperty("myConfig", m_Config);
    m_Context->setContextProperty("myAlert", m_Alert);
    m_Context->setContextProperty("myUpdater", this);
    Reload();
    connect(m_Config, SIGNAL(fileChanged()), this, SLOT(Reload()));
    connect(m_Config, SIGNAL(lookAheadChanged()), this, SLOT(Update()));
    connect(m_Config, SIGNAL(staleChanged()), this, SLOT(Update()));
    connect(m_Config, SIGNAL(maxDisplayChanged()), this, SLOT(Update()));
}

CListUpdater::~CListUpdater()
{
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i!= m_AllRunners.end(); i++)
       delete i.value();

    foreach (QObject* runner, m_OldRunners)
        delete runner;
}

void CListUpdater::FakeStartList()
{
    QStringList fnames = QStringList() << "Andy" << "Cathy" << "Bill" << "Juergen" << "Eoin" << "Iain" << "Gill" << "Michael" << "Bert" << "Rob" << "Moose" << "Linda" << "Aldo" << "Stix";
    QStringList mnames = QStringList() << "Faker" << "'the dud'" << "'Princess'" << "Ace" << "'leech'" << "de" << "King" << "Red" << "Hustler";
    QStringList snames = QStringList() << "Smith" << "Jones" << "Black" << "White" << "Murphy" << "Kirk" << "Tippett" << "Goodes" << "O'Loughlin" << "Barry" << "Rampe" << "Bolton" << "O'Keefe" << "Kennedy";
    QStringList oclasses = QStringList() << "M10" << "W10" << "W65A" << "M55AS" << "M65 Short but tricky" << "W53-54";
    QDateTime base = QDateTime::currentDateTime();
    for (int i = 0; i < 100; i++)
    {
        base = base.addSecs(qrand() % 60);
        base = base.addMSecs(i);
        QString fname = fnames.at(qrand() % fnames.size());
        QString mname = qrand() % 5 > 3 ? " " + mnames.at(qrand() % mnames.size()) : "";
        QString sname = snames.at(qrand() % snames.size());
        QString oclass = oclasses.at(qrand() % oclasses.size());
        CRunner* runner = new CRunner(fname + mname + " " + sname, base, oclass);
        m_AllRunners[base] = runner;
    }
}

bool CListUpdater::LoadRunners(QString& a_FileName)
{
    if (a_FileName.isEmpty())
        return false;
    if (a_FileName.compare("Fake", Qt::CaseInsensitive) == 0)
    {
        FakeStartList();
        return true;
    }
    else
    {
        QXmlSimpleReader parser;
        CIof3XmlContentHandler* handler = new CIof3XmlContentHandler(m_AllRunners, m_Alert);
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
    m_PlayedStartSounds.clear();
    for (QMap<QDateTime, CRunner*>::Iterator i = m_AllRunners.begin(); i!= m_AllRunners.end(); i++)
        m_OldRunners.push_back(i.value());

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
        if (i.value()->timeleft().toInt() == 0 && m_Config->playStartSound() && !m_PlayedStartSounds.contains(i.value()))
        {
            m_PlayedStartSounds.push_back(i.value());
            PlaySound();
        }
    }
}

void CListUpdater::UpdateDisplayList()
{
    m_Context->setContextProperty("myModel", QVariant::fromValue(m_DisplayList));
}

void CListUpdater::PlaySound()
{
    QThreadPool::globalInstance()->start(new CPlaySound(m_Config->startSoundFile()));
}
