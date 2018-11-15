#include "parse.h"
#include <QFile>
#include <QMimeDatabase>



Parse::Parse(): BaasBase ()
{
    //Defines default headers
    //setExtraHostURI("parse");
}

void Parse::initHeaders( bool sendSessionToken )
{

    //apply
    setRawHeader("X-Parse-Application-Id",m_applicationId.toUtf8());
    setRawHeader("Accept","application/json");
    setRawHeader("X-Parse-REST-API-Key",m_restKey.toUtf8());
    if (sendSessionToken && !m_sessionId.isEmpty())
        setRawHeader("X-Parse-Session-Token", m_sessionId.toUtf8());
    if (m_useMaster && !m_masterKey.isEmpty()){
        setRawHeader("X-Parse-Master-Key", m_masterKey.toUtf8());
    }
    else removeRawHeader("X-Parse-Master-Key");

    setHeader(QNetworkRequest::ContentTypeHeader, "application/Json");
}

bool Parse::readErrorFromResponse(int &error, QString &message)
{

    if ( !httpResponse().isObject() ) return false;

    QJsonObject obj = httpResponse().object();

    QJsonValue val = obj.value("code");
    if (val.isUndefined() ) return false;
    error = val.toInt();

    val = obj.value("error");
    if (val.isUndefined() ) return false;
    message = val.toString();

    return true;
}



void Parse::registerUser( QString username, QString password )
{
    if (!isReady()) return;

    setEndPoint("users");

    QJsonObject obj{
      {"username", username},
      {"password", password}
    };


    conn = connect(this, &RestManager::finished, [&, username](){
        disconnect(conn);
        if ( httpCode() == 201 ){
            QJsonObject obj = httpResponse().object();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << m_sessionId;
            emit operationSuccess( BaasOperation::userRegister, username);
        }
        else processError() ;

    } );

    initHeaders();
    setRawHeader("X-Parse-Revocable-Session","1");
    request( QNetworkAccessManager::PostOperation, QJsonDocument(obj).toJson());
}

void Parse::deleteUser( QString objectId)
{
    if (!isReady() || m_sessionId.isEmpty()) return;


    conn = connect(this, &RestManager::finished, [&, objectId ](){
        disconnect(conn);
        if ( namError() ) processError() ;
        else emit operationSuccess( BaasOperation::userDelete, objectId);
    } );

    setEndPoint("users/" + objectId );

    initHeaders(true);
    request( QNetworkAccessManager::DeleteOperation);
}

void Parse::signIn( QString username, QString password )
{
    qDebug() << "login Parse";
    if (!isReady()) return;

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    setEndPoint( "login?" + postData.toString() );

    initHeaders();
    setRawHeader("X-Parse-Revocable-Session","1");
    if (registerInstallationOnLogin)
        setRawHeader("X-Parse-Installation-Id", "1");

    conn = connect(this, &RestManager::finished, [&, username](){
        if ( !disconnect(conn) ) qDebug() << "can't disconnect";
        if ( !namError() )
        {
            QJsonObject obj = httpResponse().object();
            m_sessionId = obj.value("sessionToken").toString();
            m_userId = obj.value("objectId").toString();
            m_userName = obj.value("username").toString();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << m_sessionId;
            emit loginChanged();
            emit operationSuccess( BaasOperation::userSignIn, username);
        }
        else processError() ;

    } );

    request( QNetworkAccessManager::GetOperation);


}

void Parse::signOut( )
{
    if (!isReady()) return;

    setEndPoint("logout");


    QString savedUserName = m_userName;
    conn = connect(this, &RestManager::finished, [&, savedUserName]( ){
        disconnect(conn);
        Q_UNUSED(httpResponse());
        if ( !namError() ) {
            emit operationSuccess( BaasOperation::userSignOut, savedUserName);
            m_sessionId = "";
            m_userId = "";
            m_userName = "";

            emit loginChanged();
        }else processError() ;

    } );

    initHeaders();
    request( QNetworkAccessManager::PostOperation);
}

void Parse::verifyEmail(QString email)
{
    if (!isReady() || email.isEmpty() ) return;

    setEndPoint("verificationEmailRequest");

    QJsonObject obj{
      {"email", email}
    };


    conn = connect(this, &RestManager::finished, [&, email]( ){
        disconnect(conn);
        if ( namError() ) processError() ;
        else emit operationSuccess( BaasOperation::verifyEmail, email);
    } );

    initHeaders();
    request( QNetworkAccessManager::PostOperation, QJsonDocument(obj).toJson() );
}

void Parse::passwordReset( QString email)
{
    if (!isReady()) return;

    setEndPoint("requestPasswordReset");

    QJsonObject obj{
      {"email", email}
    };


    conn = connect(this, &RestManager::finished, [&, email]( ){
        disconnect(conn);
        if ( namError() ) processError() ;
        else emit operationSuccess( BaasOperation::passwordReset, email);
    } );

    initHeaders();
    request( QNetworkAccessManager::PostOperation, QJsonDocument(obj).toJson() );
}

void Parse::updateCurrentUser(QJsonDocument me)
{
    if (!isReady() || m_userId.isEmpty() ) return;

    setEndPoint("users/"+m_userId);


    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( namError() ) processError() ;
        else emit operationSuccess( BaasOperation::userUpdate,m_userId);
    } );

    initHeaders(true);
    request( QNetworkAccessManager::PutOperation, me.toJson() );
}

void Parse::currentUser()
{
    if (!isReady()) return;


    setEndPoint("users/me");


    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( namError() ) processError();
        else emit operationSuccess( BaasOperation::userMe);
    } );

    initHeaders(true);
    request( QNetworkAccessManager::GetOperation);


}





/////////////////////
//Objects related

void Parse::createObject(QString doc)
{
    if (!isReady()) return;

    ensureEndPointHasPrefix("classes");


    conn = connect(this, &RestManager::finished, [=](){
        disconnect(conn);
        if ( httpCode() == 201 ){
            emit currentObjectChanged(httpResponse().object());
            emit operationSuccess( BaasOperation::objectCreate);
        }
        else processError();
    } );

    initHeaders();
    request( QNetworkAccessManager::PostOperation, doc.toUtf8() );
}

void Parse::getObject( QString include)
{
    Q_UNUSED(include); //TODO implement include

    if (!isReady()) return;

    ensureEndPointHasPrefix("classes");


    conn = connect(this, &RestManager::finished, [=](){
        disconnect(conn);
        if ( !namError() ) {
            emit currentObjectChanged( httpResponse().object());
            emit operationSuccess( BaasOperation::objectGet);
        }
        else processError();
    } );

    initHeaders();
    request( QNetworkAccessManager::GetOperation);

}

void Parse::updateObject(QString doc)
{
    if (!isReady()) return;

    ensureEndPointHasPrefix("classes");


    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( !namError() ) {
            emit currentObjectChanged( httpResponse().object() );
            emit operationSuccess( BaasOperation::objectDelete);
        }
        else processError();
    } );

    initHeaders();
    request( QNetworkAccessManager::PutOperation, doc.toUtf8() );

}

void Parse::deleteObject(QString doc)
{
    if (!isReady()) return;

    ensureEndPointHasPrefix("classes");

    //Get objectId to be deleted
    QString deletedObjectId = endPoint();
    int found = deletedObjectId.lastIndexOf('/');
    int length = deletedObjectId.length();
    deletedObjectId = deletedObjectId.right( length - found -1);



    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( !namError() ) {
            emit objectDeleted( deletedObjectId );
            emit operationSuccess( BaasOperation::objectDelete);
        }
        else processError();
    } );

    initHeaders();
    request( QNetworkAccessManager::DeleteOperation, doc.toUtf8() );
}




void Parse::uploadFile(QUrl url, QString name)
{
    QString filePath = url.toLocalFile();
    if (!isReady() || !QFile::exists(filePath)) return;

    if (name.isEmpty()) name = url.fileName();
    setEndPoint( "files/"+name);

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);

    QFile file(filePath);
    if ( (mime.inherits("text/plain")) && (!file.open(QIODevice::ReadOnly | QIODevice::Text)) ) return;
    else if (!file.open(QIODevice::ReadOnly )) return;

    initHeaders();
    setHeader(QNetworkRequest::ContentTypeHeader, mime.name().toUtf8());


    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( httpCode() == 201 ){
            emit operationSuccess( BaasOperation::fileUpload);
            QJsonObject response = httpResponse().object();
            emit fileUploaded( response);

            //store the image by creating a document
            QMimeDatabase db;
            QString nameFile = response.value("name").toString();
            QString mime = db.mimeTypeForFile(nameFile).name();
            QString url = response.value("url").toString();

            QJsonObject fileObject{
                  {"name", nameFile },
                  {"url", response.value("url").toString()},
                  {"__type", "File" }
            };
            QJsonObject obj{
                  {"name", nameFile},
                  {"mime", mime },
                  {"file", fileObject},
            };
            setEndPoint( "classes/FilesList");
            createObject( QJsonDocument(obj).toJson() );
            emit fileAvailable(nameFile, url );

        }
        else processError();

    } );

    request( QNetworkAccessManager::PostOperation, file.readAll() );
}


void Parse::deleteFile(QString fileName)
{
    if (!isReady() || masterKey().isEmpty()) return;

    setEndPoint( "files/" + fileName);


    conn = connect(this, &RestManager::finished, [=]( ){
        disconnect(conn);
        if ( !namError() ) {
            emit operationSuccess( BaasOperation::fileDelete);
            emit objectDeleted( fileName );
        }
        else processError();
    } );

    initHeaders();
    setRawHeader("X-Parse-Master-Key", masterKey().toUtf8());

    request( QNetworkAccessManager::DeleteOperation );

}

