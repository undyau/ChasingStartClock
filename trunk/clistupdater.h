#ifndef CLISTUPDATER_H
#define CLISTUPDATER_H

#include <QObject>
#include <QTimer>
#include <QQmlContext>
#include "crunner.h"

class CConfiguration;
class CAlert;

class CListUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CListUpdater(QQmlContext *a_Context, CConfiguration* a_Config, CAlert* a_Alert, QObject *parent = 0);
    ~CListUpdater();


private:
    QList<QObject*> m_DisplayList;
    QMap<QDateTime, CRunner*> m_AllRunners;
    QList<QObject*> m_OldRunners;
    QList<QObject*> m_PlayedStartSounds;
    QTimer m_Timer;
    CConfiguration* m_Config;
    CAlert* m_Alert;
    QQmlContext *m_Context;
    void UpdateDisplayList();
    bool LoadRunners(QString &a_FileName);
    void GetDisplayList(QList<QObject *> &a_List);
    void FakeStartList();
    void PlaySound();

signals:
    
public slots:
    void Update();
    void Reload();
};

#endif // CLISTUPDATER_H
