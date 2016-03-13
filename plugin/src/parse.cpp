#include "parse.h"
#include <QFile>
#include <QMimeDatabase>


Parse::Parse(): BaaS()
{
    //Defines default headers
    setDefaultExtraHostURI("parse");
}

void Parse::initHeaders( )
{
    setRawHeader("X-Parse-Application-Id",applicationId.toUtf8());
    setRawHeader("Accept","application/json");
    setHeader(QNetworkRequest::ContentTypeHeader, "application/Json");
}

QString Parse::getApplicationId() const
{
    return applicationId;
}

void Parse::setApplicationId(const QString& res)
{
    applicationId = res;
    emit applicationIdChanged();
    emit readyChanged();
}

QString Parse::getMasterKey() const
{
    return masterKey;
}

bool Parse::isReady() const
{
    return ( (!applicationId.isEmpty()) && (!getHostURI().isEmpty()) && (getPercComplete()==100) );
}


void Parse::setMasterKey(const QString& res)
{
    masterKey = res;
    emit masterKeyChanged();
}


void Parse::signup( QString username, QString password )
{
    if (!isReady()) return;

    setEndPoint("users");

    QJsonObject obj{
      {"username", username},
      {"password", password}
    };

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            QJsonObject obj = json.object();
            sessionId = obj.value("sessionToken").toString();
            userId = obj.value("objectId").toString();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << sessionId;
        }

    } );

    initHeaders();
    request( BaaS::POST, QJsonDocument(obj).toJson());
}

void Parse::deleteUser( QString objectId)
{
    if (!isReady() || sessionId.isEmpty()) return;

    setEndPoint("users/" + objectId );

    initHeaders();
    setRawHeader("X-Parse-Session-Token", sessionId.toUtf8());
    request( BaaS::DELETE);
}

void Parse::login( QString username, QString password )
{
    if (!isReady()) return;

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    setEndPoint( "login?" + postData.toString() ); //TODO improve me : not use endpoint to give url encoded params

    initHeaders();
    if (registerInstallationOnLogin)
        setRawHeader("X-Parse-Installation-Id", "1");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) { //getHttpCode() == 201 ){
            QJsonObject obj = json.object();
            sessionId = obj.value("sessionToken").toString();
            userId = obj.value("objectId").toString();
            userName = obj.value("username").toString();
            qDebug() << "objectId" << obj.value("objectId").toString();
            qDebug() << "sessionToken" << sessionId;
            emit loginChanged();
        }

    } );

    request( BaaS::GET);


}

void Parse::logout( )
{
    if (!isReady()) return;

    setEndPoint("logout");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        Q_UNUSED(json);
        if ( isLastRequestSuccessful() ) {
            sessionId = "";
            userId = "";
            userName = "";

            emit loginChanged();
        }

    } );

    initHeaders();
    request( BaaS::POST);
}

void Parse::passwordReset( QString email)
{
    if (!isReady()) return;

    setEndPoint("requestPasswordReset");

    QJsonObject obj{
      {"email", email}
    };

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        Q_UNUSED(json);
        if ( isLastRequestSuccessful() ) {

        }

    } );

    initHeaders();
    request( BaaS::POST, QJsonDocument(obj).toJson() );
}

QNetworkReply *Parse::query(QString endPoint, QUrlQuery extraParams)
{
    if (!isReady()) return NULL;

    if (extraParams.isEmpty())
        setEndPoint( endPoint);
    else setEndPoint( endPoint + "?" + extraParams.toString() ); //TODO improve me : not use endpoint to give url encoded params


    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if (isLastRequestSuccessful()){
                //structure to return from parse
                QHash<int, QByteArray> roles = QHash<int, QByteArray>();
                QVector<QVariantMap> data = QVector<QVariantMap>();

                Q_ASSERT(json.isObject());
                QJsonObject obj = json.object();
                QJsonValue tmp = obj.value("results");

                if (tmp.isArray()){
                    QJsonArray res = tmp.toArray();
                    for ( QJsonValue elem: res){
                        if (elem.isObject()){
                            obj = elem.toObject();

                            //Update roles
                            QStringList keys = obj.keys();
                            for (QString roleName : keys )
                            {
                                if ( roles.key( roleName.toUtf8(), -1) == -1)
                                    roles[roles.size()] = roleName.toUtf8();
                            }
                            //Add values
                            data.push_back( obj.toVariantMap());


                        }
                    }

                    emit querySucceeded(roles, data);
                }
                //else if (tmp.isObject()){
                    //obj = tmp.toObject();//Update roles
                else {
                    QStringList keys = obj.keys();
                    for (QString roleName : keys )
                    {
                        if ( roles.key( roleName.toUtf8(), -1) == -1)
                            roles[roles.size()] = roleName.toUtf8();
                    }
                    //Add values
                    data.push_back( obj.toVariantMap());
                    emit querySucceeded(roles, data);
                }
            }
            else emit queryFailed(json);

    } );

    initHeaders();
    return request( BaaS::GET);

}

bool Parse::ensureEndPointHasPrefix(QString prefix)
{
    QString endpt = getEndPoint();
    //if ( endpt.left( prefix.length() ) == prefix )
    if (endpt.startsWith( prefix ))
        return true;
    else{
        setEndPoint( prefix + "/" + endpt);
    }
    return false;
}

//Objects related
QNetworkReply* Parse::create( QString doc)
{
    if (!isReady()) return NULL;

    ensureEndPointHasPrefix("classes");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            currentObject = json.object();
            emit currentObjectChanged( currentObject);
        }

    } );

    initHeaders();
    return request( BaaS::POST, doc.toUtf8() );
}

QNetworkReply* Parse::get( QString include)
{
    Q_UNUSED(include); //TODO implement include

    if (!isReady()) return NULL;

    ensureEndPointHasPrefix("classes");

    /*
    QUrlQuery postData;
    postData.addQueryItem("include", include);
    if (!include.isEmpty())
        setEndPoint( "login?" + postData.toString() );
        */

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            currentObject = json.object();
            emit currentObjectChanged( currentObject);
        }
    } );

    initHeaders();
    return request( BaaS::GET);

}

QNetworkReply* Parse::update(QString doc)
{
    if (!isReady()) return NULL;

    ensureEndPointHasPrefix("classes");

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            currentObject = json.object();
            emit currentObjectChanged( currentObject);
        }
    } );

    initHeaders();
    return request( BaaS::PUT, doc.toUtf8() );

}

QNetworkReply* Parse::deleteObject(QString doc)
{
    if (!isReady()) return NULL;

    ensureEndPointHasPrefix("classes");

    //Get objectId to be deleted
    QString deletedObjectId = getEndPoint();
    int found = deletedObjectId.lastIndexOf('/');
    int length = deletedObjectId.length();
    deletedObjectId = deletedObjectId.right( length - found -1);


    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        Q_UNUSED(json);
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            emit objectDeleted( deletedObjectId );

        }
    } );

    initHeaders();
    return request( BaaS::DELETE, doc.toUtf8() );
}


QNetworkReply* Parse::uploadFile(QUrl url, QString name)
{
    QString filePath = url.toLocalFile();
    if (!isReady() || !QFile::exists(filePath)) return NULL;

    if (name.isEmpty()) name = url.fileName();
    setEndPoint( "files/"+name);

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);

    QFile file(filePath);
    if (mime.inherits("text/plain")){
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return NULL;
    }
    else{
        if (!file.open(QIODevice::ReadOnly ))
            return NULL;
    }

    initHeaders();
    setHeader(QNetworkRequest::ContentTypeHeader, mime.name().toUtf8());

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            currentObject = json.object();
            emit fileUploaded( currentObject);
        }

    } );

    return request( BaaS::POST, file.readAll() );
}


QNetworkReply* Parse::deleteFile(QString fileName)
{
    if (!isReady() || getMasterKey().isEmpty()) return NULL;

    setEndPoint( "files/" + fileName);

    m_conn = connect(this, &BaaS::replyFinished, [=]( QJsonDocument json){
        Q_UNUSED(json);
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            emit objectDeleted( fileName );

        }
    } );

    initHeaders();
    setRawHeader("X-Parse-Master-Key", getMasterKey().toUtf8());

    return request( BaaS::DELETE );

}

