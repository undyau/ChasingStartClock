#include "cplaysound.h"
#include <QSound>

CPlaySound::CPlaySound(QString a_SoundFile) :
    QRunnable(), m_SoundFile(a_SoundFile)
{
}

void CPlaySound::run()
{
    QSound::play(m_SoundFile);
}
