#include "cconfiguration.h"
#include <QUrl>
#include <QSettings>
#include <QDebug>

CConfiguration::CConfiguration(const QString &a_File, int a_LookAhead, int a_Stale,
                               int a_MaxDisplay, QObject *parent):
    m_File(a_File), QObject(parent), m_Stale(a_Stale), m_MaxDisplay(a_MaxDisplay),
    m_LookAhead(a_LookAhead)
{
    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "ChasingStartClock");
    settings.beginGroup("Configuration");
    m_File = settings.value("StartFile", a_File).toString();
    if (m_File.isEmpty())
        m_File = a_File;
    m_Stale = settings.value("Stale", a_Stale).toInt();
    m_MaxDisplay = settings.value("MaxDisplay", a_MaxDisplay).toInt();
    m_LookAhead = settings.value("LookAhead", a_LookAhead).toInt();
    settings.endGroup();
}

CConfiguration::~CConfiguration()
{
    //qDebug() << "In destructor";
    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "ChasingStartClock");
    settings.beginGroup("Configuration");
    settings.setValue("StartFile", m_File);
    settings.setValue("Stale", m_Stale);
    settings.setValue("MaxDisplay", m_MaxDisplay);
    settings.setValue("LookAhead", m_LookAhead);
    settings.endGroup();
   // qDebug() << "Ended destructor";

}

void CConfiguration::setFile(const QString &a)
{
    if (a != m_File) {
        m_File = a;
        emit fileChanged();
    }
}

void CConfiguration::setFileUrl(const QString &a)
{
    if (a != m_FileUrl) {
        m_FileUrl = a;
        emit fileUrlChanged();
    }
    setFile(QUrl(a).toLocalFile());
}

void CConfiguration::setStartSoundFile(const QString &a)
{
    if (a != m_StartSoundFileName) {
        m_StartSoundFileName = a;
        emit startSoundFileChanged();
    }
}

void CConfiguration::setLookAhead(int a)
{
    if (a != m_LookAhead) {
        m_LookAhead = a;
        emit lookAheadChanged();
    }
}

void CConfiguration::setStale(int a)
{
    if (a != m_Stale) {
        m_Stale = a;
        emit staleChanged();
    }
}

void CConfiguration::setMaxDisplay(int a)
{
    if (a != m_MaxDisplay) {
        m_MaxDisplay = a;
        emit maxDisplayChanged();
    }
}



