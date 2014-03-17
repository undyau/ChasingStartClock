#include "cplaysound.h"
#include <QSound>
#include <QDebug>

CPlaySound::CPlaySound(QString a_SoundFile) :
    QRunnable(), m_Sound(a_SoundFile), m_SoundFile(a_SoundFile)
{
}

void CPlaySound::run()
{
    qDebug() << "Playing sound" << m_SoundFile;
    m_Sound.play();
}
