#ifndef CLISTUPDATER_H
#define CLISTUPDATER_H

#include <QObject>
#include <QTimer>
#include <QQmlContext>
#include "crunner.h"

class CListUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CListUpdater(QQmlContext *a_Context, QString a_FileName, int a_LookAhead = 100000,
                          int a_Stale = -3, int a_MaxDisplay = 10, QObject *parent = 0);

private:
    QList<QObject*> m_DisplayList;
    QMap<QDateTime, CRunner*> m_AllRunners;
    QTimer m_Timer;
    int m_Stale;
    int m_MaxDisplay;
    int m_Lookahead;
    QQmlContext *m_Context;
    void UpdateDisplayList();
    bool LoadRunners(QString &a_FileName);
    void GetDisplayList(QList<QObject *> &a_List);

signals:
    
public slots:
    void Update();
};

#endif // CLISTUPDATER_H
