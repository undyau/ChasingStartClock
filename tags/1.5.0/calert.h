#ifndef CALERT_H
#define CALERT_H

#include <QObject>

class CAlert : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
public:
    explicit CAlert(QObject *parent = 0);
    void setMessage(const QString &a);
    QString message() const { return m_Message; }
    
signals:
    void messageChanged();
    
public slots:

private:
    QString m_Message;
    
};

#endif // CALERT_H
