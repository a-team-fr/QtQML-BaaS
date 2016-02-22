#ifndef PARSE_H
#define PARSE_H
#include "baas.h"
#include <QObject>

class Parse : public BaaS
{
    Q_OBJECT
    Q_PROPERTY( QString applicationId READ getApplicationId  WRITE setApplicationId NOTIFY applicationIdChanged)
    Q_PROPERTY( QString masterKey READ getMasterKey  WRITE setMasterKey NOTIFY masterKeyChanged)
    Q_PROPERTY( bool loggedIn READ isLoggedIn NOTIFY loginChanged)
    Q_PROPERTY( QString userName MEMBER userName NOTIFY loginChanged)

public:
    Parse();

    //Users related
    Q_INVOKABLE void signup( QString username, QString password );
    Q_INVOKABLE void deleteUser( QString objectId);
    Q_INVOKABLE void login( QString username, QString password );
    Q_INVOKABLE void logout( );
    Q_INVOKABLE void passwordReset(QString email);

    //Files related
    /*Q_INVOKABLE void uploadFile(QString name, QString contentType , QString data);
    Q_INVOKABLE void uploadBinaryFile(QString name, QString contentType , QString data);
    Q_INVOKABLE void associateFile(QString objectName, QString remoteFilePath);
    Q_INVOKABLE void deleteFile(QString remoteFilePath);*/

    //Query
    virtual void query(QString endPoint, QUrlQuery extraParams)  Q_DECL_OVERRIDE;


public: // property access
    QString getApplicationId() const;
    void setApplicationId(const QString& res);
    QString getMasterKey() const;
    void setMasterKey(const QString& res);
    bool isLoggedIn(){ return !sessionId.isEmpty();}
    bool isReady() const Q_DECL_OVERRIDE;

signals:
    void applicationIdChanged();
    void masterKeyChanged();
    void loginChanged();


private:
    QString applicationId ="";
    QString sessionId ="";
    QString userId ="";
    QString userName = "";
    QString masterKey = "";

    QMetaObject::Connection m_conn;             //temporary use for terminating connection with lambda
    bool registerInstallationOnLogin = false;
};

#endif // PARSE_H
