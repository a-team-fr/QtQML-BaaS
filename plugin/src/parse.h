#ifndef PARSE_H
#define PARSE_H
#include <QObject>
#include "baasbase.h"


class Parse : public BaasBase
{
    Q_OBJECT

public:
    Parse();

    //Users related
    Q_INVOKABLE void registerUser( QString username, QString password );
    Q_INVOKABLE void deleteUser( QString objectId);
    Q_INVOKABLE void signIn( QString username, QString password );
    Q_INVOKABLE void signOut( );
    Q_INVOKABLE void verifyEmail( QString email);
    Q_INVOKABLE void passwordReset(QString email);
    Q_INVOKABLE void updateCurrentUser(QJsonDocument me);
    Q_INVOKABLE void currentUser();


    //Objects related
    Q_INVOKABLE void createObject(QString doc);
    Q_INVOKABLE void getObject( QString include = "");
    Q_INVOKABLE void updateObject(QString doc);
    Q_INVOKABLE void deleteObject(QString doc = "");

    //Files related
    Q_INVOKABLE void uploadFile(QUrl url, QString name = QString());
    Q_INVOKABLE void deleteFile(QString fileName);


protected:
    void initHeaders( bool sendSessionToken = false ) override;
    bool readErrorFromResponse( int& error, QString& message ) override;
private:
    bool registerInstallationOnLogin = false;

};

#endif // PARSE_H
