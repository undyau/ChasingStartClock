#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <QObject>

class CConfiguration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString startSoundFile READ startSoundFile WRITE setStartSoundFile NOTIFY startSoundFileChanged)
    Q_PROPERTY(QString startSoundFileUrl READ startSoundFileUrl WRITE setStartSoundFileUrl NOTIFY startSoundFileUrlChanged)
    Q_PROPERTY(int lookAhead READ lookAhead WRITE setLookAhead NOTIFY lookAheadChanged)
    Q_PROPERTY(int stale READ stale WRITE setStale NOTIFY staleChanged)
    Q_PROPERTY(int maxDisplay READ maxDisplay WRITE setMaxDisplay NOTIFY maxDisplayChanged)
    Q_PROPERTY(bool timesAreUtc READ timesAreUtc WRITE setTimesAreUtc NOTIFY timesAreUtcChanged)
public:
    explicit CConfiguration(const QString& a_File = "", int a_LookAhead = 10,
                            int a_Stale = -3, int a_MaxDisplay = 10, QObject *parent = 0);
    ~CConfiguration();
    void setFile(const QString &a);
    void setFileUrl(const QString &a);
    void setStartSoundFile(const QString& a);
    void setStartSoundFileUrl(const QString& a);
    void setLookAhead(int a);
    void setStale(int a);
    void setMaxDisplay(int a);
    QString file() const { return m_File; }
    QString fileUrl() const { return m_FileUrl; }
    QString startSoundFile() const { return m_StartSoundFileName;}
    QString startSoundFileUrl() const { return m_StartSoundFileUrl;}
    int lookAhead() const { return m_LookAhead; }
    int stale() const { return m_Stale; }
    int maxDisplay() const { return m_MaxDisplay; }
    bool playStartSound() const { return !!m_StartSoundFileName.size(); }
    bool timesAreUtc() const;
    void setTimesAreUtc(bool a_TimesAreUtc);

signals:
    void fileChanged();
    void fileUrlChanged();
    void lookAheadChanged();
    void staleChanged();
    void maxDisplayChanged();
    void startSoundFileChanged();
    void startSoundFileUrlChanged();
    void timesAreUtcChanged();

public slots:

private:
    QString m_File;
    QString m_FileUrl;
    QString m_IniFileName;
    QString m_StartSoundFileName;
    QString m_StartSoundFileUrl;
    int m_Stale;
    int m_MaxDisplay;
    int m_LookAhead;
    bool m_TimesAreUtc;
};

#endif // CCONFIGURATION_H
