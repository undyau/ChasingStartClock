#ifndef CIOF3XMLCONTENTHANDLER_H
#define CIOF3XMLCONTENTHANDLER_H

#include <QtXml/QXmlDefaultHandler>
#include <QVector>

class CRunner;

class CIof3XmlContentHandler : public QXmlDefaultHandler
{
public:
    CIof3XmlContentHandler(QMap<QDateTime, CRunner*>& a_AllRunners);
    ~CIof3XmlContentHandler();
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool characters ( const QString & ch );
    bool startElement(const QString & namespaceURI, const QString & localName,
                    const QString & qName, const QXmlAttributes & atts );

private:
    QVector<QString> m_Elements;
    QMap<QDateTime, CRunner*>& m_AllRunners;
    void AddPerson();
    QString m_FName;
    QString m_SName;
    QString m_Class;
    QString m_Start;
};

#endif // CIOF3XMLCONTENTHANDLER_H
