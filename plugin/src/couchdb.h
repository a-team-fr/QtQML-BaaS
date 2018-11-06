#ifndef COUCHDB_H
#define COUCHDB_H
#include "restmanager.h"
#include <QObject>

class CouchDB : public RestManager
{
    Q_OBJECT
    Q_PROPERTY( QString applicationId READ getApplicationId  WRITE setApplicationId NOTIFY applicationIdChanged)
    Q_PROPERTY( QString masterKey READ getMasterKey  WRITE setMasterKey NOTIFY masterKeyChanged)
    Q_PROPERTY( bool useMaster READ getUseMaster WRITE SetMasterMode NOTIFY masterModeChanged)
    Q_PROPERTY( bool loggedIn READ isLoggedIn NOTIFY loginChanged)
    Q_PROPERTY( QString userName MEMBER userName NOTIFY loginChanged)

public:
    CouchDB();

    //Users related
    Q_INVOKABLE void signup( QString username, QString password );
    Q_INVOKABLE void deleteUser( QString objectId);
    Q_INVOKABLE void login( QString username, QString password );
    Q_INVOKABLE void logout( );
    Q_INVOKABLE void passwordReset(QString email);

    //Objects related
    Q_INVOKABLE void create(QString doc);
    Q_INVOKABLE QNetworkReply *get( QString include = "");
    Q_INVOKABLE void update(QString doc);
    Q_INVOKABLE void deleteObject(QString doc = "");

    Q_INVOKABLE bool ensureEndPointHasPrefix(QString prefix);

    //Files related
    Q_INVOKABLE QNetworkReply *uploadFile(QUrl url, QString name = QString());
    Q_INVOKABLE QNetworkReply *deleteFile(QString fileName);

    //Query - this is used by the BaasModel
    virtual QNetworkReply *query(QString m_endPoint, QUrlQuery extraParams)  Q_DECL_OVERRIDE;


public: // property access
    QString getApplicationId() const;
    void setApplicationId(const QString& res);
    QString getMasterKey() const;
    void setMasterKey(const QString& res);
    bool isLoggedIn(){ return !sessionId.isEmpty();}
    bool isReady() const Q_DECL_OVERRIDE;
    bool getUseMaster() const{ return useMaster;}
    void SetMasterMode(bool _useMaster){
        if (masterKey.isEmpty()) return;

        useMaster = _useMaster;

        if (useMaster){
            setRawHeader("X-Parse-Master-Key", masterKey.toUtf8());
        }
        else removeRawHeader("X-Parse-Master-Key");
        emit masterModeChanged();
    }

signals:
    void applicationIdChanged();
    void masterKeyChanged();
    void loginChanged();
    void masterModeChanged();
    void objectDeleted( QString deletedObjectId );
    void currentObjectChanged( QJsonObject currentObject);
    void fileUploaded( QJsonObject currentObject);


private:
    void initHeaders( );

    QString applicationId ="";
    QString sessionId ="";
    QString userId ="";
    QString userName = "";
    QJsonObject currentObject = QJsonObject();
    QString masterKey = "";

    QMetaObject::Connection m_conn;             //temporary use for terminating connection with lambda
    bool useMaster = false;

    QList<QString> m_createQueue;
    QList<QString> m_updateQueue;
    QList<QString> m_deleteQueue;
};

#endif // COUCHDB_H
