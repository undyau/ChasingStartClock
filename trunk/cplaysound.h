#ifndef CPLAYSOUND_H
#define CPLAYSOUND_H

#include <QRunnable>
#include <QString>

class CPlaySound : public QRunnable
{
public:
    explicit CPlaySound(QString a_SoundFile);
    virtual void run();
    
private:
    QString m_SoundFile;
    
};

#endif // CPLAYSOUND_H
