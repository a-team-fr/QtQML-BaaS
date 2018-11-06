#ifndef BAASBASE_H
#define BAASBASE_H

#include <QObject>
#include "restmanager.h"

class BaasBase : public RestManager
{
    Q_OBJECT
    Q_PROPERTY( QString applicationId READ applicationId  WRITE setApplicationId NOTIFY applicationIdChanged)
    Q_PROPERTY( QString masterKey READ masterKey  WRITE setMasterKey NOTIFY masterKeyChanged)
    Q_PROPERTY( QString restKey READ restKey  WRITE setRestKey NOTIFY restKeyChanged)
    Q_PROPERTY( bool useMaster READ useMaster WRITE SetMasterMode NOTIFY masterModeChanged)
    Q_PROPERTY( bool signedIn READ signedIn NOTIFY loginChanged)
    Q_PROPERTY( QString userName MEMBER m_userName NOTIFY loginChanged)

    Q_PROPERTY(int errorCode READ errorCode NOTIFY error)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY error)
    Q_PROPERTY(BaasOperation successCode READ successCode NOTIFY success)
    Q_PROPERTY(QString successMessage READ successMessage NOTIFY success)

    Q_ENUMS( BaasOperation)
public:
    BaasBase();
    enum BaasOperation{ Unknown = 0,userRegister,userDelete, userSignIn,userSignOut,userUpdate, userMe,
                        verifyEmail = 100,passwordReset,
                        objectCreate = 200, objectGet, objectUpdate, objectDelete,
                        fileUpload = 300, fileDelete };

    Q_INVOKABLE void query(QString m_endPoint, QUrlQuery extraParams, QString prefix = "classes");

    //Property accessors
    QString errorMessage() {return m_errorMessage;}
    int errorCode() {return m_errorCode;}
    QString successMessage() {return m_successMessage;}
    BaasOperation successCode() {return m_successCode;}
    QString applicationId() const{ return m_applicationId;}
    void setApplicationId(const QString& res);
    QString masterKey() const{ return m_masterKey;}
    void setMasterKey(const QString& res);
    QString restKey() const{ return m_restKey;}
    void setRestKey(const QString& res);
    bool signedIn(){ return !m_sessionId.isEmpty();}
    virtual bool isReady() const override{ return ( (!m_applicationId.isEmpty()) && RestManager::isReady() );}
    bool useMaster() const{ return m_useMaster;}
    void SetMasterMode(bool _useMaster);





signals:
    //property change notifies
    void querySucceeded(const QHash<int, QByteArray>&, const QVector<QVariantMap>&);
    void error();
    void operationSuccess(BaasOperation, QVariant extraContext = "");
    void success();
    void applicationIdChanged();
    void masterKeyChanged();
    void restKeyChanged();
    void loginChanged();
    void masterModeChanged();
    void objectDeleted( QString deletedObjectId );
    void currentObjectChanged( QJsonObject currentObject);

    void fileUploaded( QJsonObject currentObject);


protected:
    virtual bool readErrorFromResponse( int& error, QString& message ) = 0;
    virtual void processError() final;
    virtual void processSuccess(BaasOperation, QVariant extraContext = "") final;

    QString m_applicationId ="";
    QString m_sessionId ="";
    QString m_userId ="";
    QString m_userName = "";
    QString m_masterKey = "";
    QString m_restKey = "";
    bool m_useMaster = false;

private:
    void resetStateProperties();           //reset things before processing the request (error code for instance)
    int m_errorCode{-1};
    QString m_errorMessage{};
    BaasOperation m_successCode{Unknown};
    QString m_successMessage{};


};

#endif // BAASBASE_H
