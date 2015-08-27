#ifndef CRUNNER_H
#define CRUNNER_H

#include <QObject>
#include <QDateTime>

class CRunner : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString start READ start CONSTANT)
    Q_PROPERTY(QString oclass READ oclass CONSTANT)
    Q_PROPERTY(QString timeleft READ timeleft WRITE setTimeleft NOTIFY timeleftChanged)

public:
    CRunner(QObject *parent=0);
    CRunner(const QString &name,  const QDateTime &start,const QString &oclass,QObject *parent=0);

    QString name() const;
    QString color() const;
    void setColor(const QString &color);
    QString start() const;
    QString oclass() const;
    QString timeleft() const;
    void setTimeleft(const QString &timeleft);
    
signals:
   void colorChanged();
   void timeleftChanged();

public slots:
   void updateTimeleft();

private:
    QString m_name;
    QString m_color;
    QDateTime m_start;
    QString m_oclass;
    QString m_timeleft;

};
#endif

