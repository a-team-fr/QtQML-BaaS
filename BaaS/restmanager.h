#ifndef RESTMANAGER_H
#define RESTMANAGER_H
#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QBuffer>
#include <QNetworkConfigurationManager>

class RestManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool online READ online NOTIFY onlineChanged)
    Q_PROPERTY( QString hostURI READ hostURI  WRITE setHostURI NOTIFY hostChanged)
    Q_PROPERTY( QString extraHostURI READ extraHostURI  WRITE setExtraHostURI NOTIFY hostChanged)
    Q_PROPERTY( QString endPoint READ endPoint WRITE setEndPoint NOTIFY endPointChanged)

    Q_PROPERTY( qreal percComplete READ percComplete NOTIFY percCompleteChanged)
    Q_PROPERTY( QString namErrorText READ errorText NOTIFY finished)
    Q_PROPERTY( QJsonDocument httpResponse READ httpResponse NOTIFY finished)
    Q_PROPERTY( QString httpResponseText READ httpResponseText NOTIFY finished)
    Q_PROPERTY( int httpCode READ httpCode NOTIFY finished)
    Q_PROPERTY( uint namError READ namError NOTIFY finished)
    Q_PROPERTY( bool ready READ isReady NOTIFY readyChanged)

signals: // operation notifications
    void hostChanged();
    void onlineChanged(bool);
    void endPointChanged();
    void readyChanged();
    void percCompleteChanged();
    void beforeFinished();                  //emited when a request gets replied
    void finished();                       //emited when a request gets replied and fully processed

public:
    RestManager( QObject* _parent = nullptr );
    // property access
    void setHostURI(const QString& res);
    void setExtraHostURI(const QString& res);
    void setEndPoint(const QString& res);
    bool online() const{
        //return m_NCM.isOnline();
        QList<QNetworkConfiguration> activeConfigs = m_NCM.allConfigurations(QNetworkConfiguration::Active);
         if (activeConfigs.count() > 0)
         {
             QNetworkConfiguration cfg = activeConfigs.first();
             qDebug() << QString("Family %1, Type %2, Name %3").arg(cfg.bearerTypeFamily()).arg(cfg.name()).arg(cfg.bearerTypeName());
             Q_ASSERT(m_NCM.isOnline());
             return true;
         }
         else
             return false;

    }
    QString errorText() const{return m_NAMerrorText;}
    qreal percComplete() const{return m_percComplete;}
    int httpCode() const{return m_httpCode;}
    uint namError() const{return m_NAMerror;}
    virtual bool isReady() const;
    QString hostURI() const{return m_hostURI;}
    QString extraHostURI() const{return m_extraHostURI;}
    QString endPoint() const{ return m_endPoint;}
    QJsonDocument httpResponse(){return m_httpResponse;}
    QString httpResponseText(){return m_httpResponse.toJson();}

    Q_INVOKABLE bool ensureEndPointHasPrefix(QString prefix);


private slots: // common network operations
    void replyFinished();
    void replyProgress(qint64 bytesSent, qint64 bytesTotal);


protected:

    uint request( QNetworkAccessManager::Operation operation, const QByteArray& data = QByteArray());
    void setPercComplete(qreal res);


    //Use this to manage headers
    void setRawHeader(QByteArray name, QByteArray value){ m_rawHeaders[name] = value;}
    void removeRawHeader(QByteArray name){ m_rawHeaders.remove(name);}
    void resetRawHeaders(){ m_rawHeaders.clear();}
    void setHeader(QNetworkRequest::KnownHeaders name, QByteArray value){ m_headers[name] = value;}
    void removeHeader(QNetworkRequest::KnownHeaders name){ m_headers.remove(name);}
    void resetHeaders(){ m_headers.clear();}
    virtual void initHeaders( bool sendSessionToken = false ){ Q_UNUSED(sendSessionToken)}
    void applyAllHeaders( QNetworkRequest&) const;

    QMetaObject::Connection  conn ; //TODO find a better workaround

private:
    QNetworkAccessManager m_NAM;
    QNetworkConfigurationManager m_NCM;
    QMap<QByteArray, QByteArray> m_rawHeaders;
    QMap<QNetworkRequest::KnownHeaders, QByteArray> m_headers;
    QString m_hostURI {};
    QString m_extraHostURI {};
    QString m_endPoint{};
    qreal m_percComplete{100};

    QString m_NAMerrorText{};
    uint m_NAMerror{0};
    int m_httpCode{-1};
    QJsonDocument m_httpResponse{};

    uint m_requestNumber{0};


};

#endif // RESTMANAGER_H

