#include "cconfiguration.h"
#include <QUrl>

CConfiguration::CConfiguration(const QString &a_File, int a_LookAhead, int a_Stale,
                               int a_MaxDisplay, QObject *parent):
    m_File(a_File), QObject(parent), m_Stale(a_Stale), m_MaxDisplay(a_MaxDisplay),
    m_LookAhead(a_LookAhead)
{
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




