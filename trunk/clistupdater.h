#ifndef CLISTUPDATER_H
#define CLISTUPDATER_H

#include <QObject>
#include <QTimer>
#include <QQmlContext>
#include "crunner.h"

class CConfiguration;

class CListUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CListUpdater(QQmlContext *a_Context, CConfiguration* a_Config, QObject *parent = 0);

private:
    QList<QObject*> m_DisplayList;
    QMap<QDateTime, CRunner*> m_AllRunners;
    QTimer m_Timer;
    CConfiguration* m_Config;
    QQmlContext *m_Context;
    void UpdateDisplayList();
    bool LoadRunners(QString &a_FileName);
    void GetDisplayList(QList<QObject *> &a_List);

signals:
    
public slots:
    void Update();
    void Reload();
};

#endif // CLISTUPDATER_H
