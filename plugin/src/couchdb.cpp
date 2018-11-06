#include "couchdb.h"
#include <QFile>
#include <QMimeDatabase>
#include <QNetworkCookie>
#include <QFuture>

CouchDB::CouchDB(): RestManager()
{
    //Defines default headers
    //setDefaultExtraHostURI("");
}

void CouchDB::initHeaders( )
{
    //setRawHeader("X-CouchDB-Application-Id",applicationId.toUtf8());
    //setRawHeader("Accept","application/json");
    setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!sessionId.isEmpty())
        setHeader(QNetworkRequest::CookieHeader, sessionId.toUtf8());
}

QString CouchDB::getApplicationId() const
{
    return applicationId;
}

void CouchDB::setApplicationId(const QString& res)
{
    applicationId = res;
    emit applicationIdChanged();
    emit readyChanged();
}

QString CouchDB::getMasterKey() const
{
    return masterKey;
}

bool CouchDB::isReady() const
{
    return RestManager::isReady();
}


void CouchDB::setMasterKey(const QString& res)
{
    masterKey = res;
    emit masterKeyChanged();
}


void CouchDB::signup( QString username, QString password )
{
    if (!isReady()) return;

    setEndPoint("_users/org.couchdb.user:"+username);

    QJsonObject obj{
        {"name", username},
        {"password", password},
        {"roles", QJsonArray()},
        {"type", "user"},
    };

    m_conn = connect(this, &RestManager::replyFinished, [=]( ){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            qDebug()<<"successfuly created "<< userName;
        }

    } );

    initHeaders();
    request( RestManager::PUT, QJsonDocument(obj).toJson());
}

void CouchDB::deleteUser( QString objectId)
{
    if (!isReady()) return;

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json, QNetworkReply *reply){
        bool bdOk = disconnect(m_conn);

        if ( isLastRequestSuccessful() ) {
            QJsonObject obj = json.object();

            setEndPoint("_users/org.couchdb.user:" + objectId + "?rev=" + obj.value("_rev").toString() );

            m_conn = connect(this, &RestManager::replyFinished, [=]( ){
                disconnect(m_conn);

                if ( getHttpCode() == 200 ) {
                    qDebug()<<"successfuly deleted "<< userName;
                }

                request( RestManager::DELETE);

            } );
        }

    } );

    setEndPoint("_users/org.couchdb.user:" + objectId );

    initHeaders();

    request( RestManager::GET);


}



void CouchDB::login( QString username, QString password )
{
    if (!isReady()) return;

    setEndPoint( "_session");
    initHeaders();

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json, QNetworkReply *reply){
        bool bdOk = disconnect(m_conn);
        sessionId = "";
        if ( isLastRequestSuccessful() ) { //getHttpCode() == 201 ){

            QVariant ret = reply->header(QNetworkRequest::SetCookieHeader);
            //if (!ret) return QVariant();

            QList<QNetworkCookie> cookies = ret.value<QList<QNetworkCookie>>();
            //QList<QNetworkCookie>
            for (QNetworkCookie cookie : cookies)
            {
                //QNetworkCookie cookie = var.value<QNetworkCookie>();
                if (cookie.name() == "AuthSession"){
                    sessionId = cookie.value();
                }
            }
            QJsonObject obj = json.object();
            qDebug() << "name" << obj.value("name").toString();
            qDebug() << "roles" << obj.value("roles").toString();
            if (!sessionId.isEmpty())
            {
                userId = userName = obj.value("name").toString();
            }
            emit loginChanged();
        }

    } );

    QJsonObject obj{
        {"name", username},
        {"password", password}
    };

    request( RestManager::POST,  QJsonDocument(obj).toJson() );


}

void CouchDB::logout( )
{
    if (!isReady()) return;

    setEndPoint("_session");

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        Q_UNUSED(json);
        if ( isLastRequestSuccessful() ) {
            sessionId = "";
            userId = "";
            userName = "";

            emit loginChanged();
        }
        else {
            qDebug()<<"error during logout";
        }

    } );

    initHeaders();
    request( RestManager::DELETE);
}

void CouchDB::passwordReset( QString email)
{
    Q_UNUSED(email);
}

QNetworkReply *CouchDB::query(QString endPoint, QUrlQuery extraParams)
{
    if (!isReady()) return NULL;

    if (extraParams.isEmpty())
        setEndPoint(endPoint);
    else setEndPoint( endPoint + "?" + extraParams.toString() ); //TODO improve me : not use endpoint to give url encoded params

    ensureEndPointHasPrefix("classes");

    m_conn = connect(this, &RestManager::replyFinished, [=](QJsonDocument json, QNetworkReply *reply){
        disconnect(m_conn);
        if (isLastRequestSuccessful()){
            //structure to return from CouchDB
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

                emit querySucceeded(roles, data, reply);
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
                emit querySucceeded(roles, data, reply);
            }
        }
    } );

    initHeaders();
    return request( RestManager::GET);

}

bool CouchDB::ensureEndPointHasPrefix(QString prefix)
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
void CouchDB::create(QString doc)
{
    if (!isReady()) {
        m_createQueue.append(doc);
        return;
    }

    ensureEndPointHasPrefix("classes");

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            currentObject = json.object();
            emit currentObjectChanged(currentObject);
            if(!m_createQueue.isEmpty()) {
                create(m_createQueue.takeFirst());
            }
        }
        else {
            // error, drop the queue
            m_createQueue.empty();
        }
    } );

    initHeaders();
    request( RestManager::POST, doc.toUtf8() );
}

QNetworkReply* CouchDB::get( QString include)
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

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            currentObject = json.object();
            emit currentObjectChanged( currentObject);
        }
    } );

    initHeaders();
    return request( RestManager::GET);

}

void CouchDB::update(QString doc)
{
    if (!isReady()) {
        m_updateQueue.append(doc);
        return;
    }

    ensureEndPointHasPrefix("classes");

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            currentObject = json.object();
            emit currentObjectChanged(currentObject);
            if(!m_updateQueue.isEmpty()) {
                update(m_updateQueue.takeFirst());
            }
        }
        else {
            // error, drop the queue
            m_updateQueue.empty();
        }
    } );

    initHeaders();
    request( RestManager::PUT, doc.toUtf8() );

}

void CouchDB::deleteObject(QString doc)
{
    if (!isReady()) {
        m_deleteQueue.append(doc);
        return;
    }

    ensureEndPointHasPrefix("classes");

    //Get objectId to be deleted
    QString deletedObjectId = getEndPoint();
    int found = deletedObjectId.lastIndexOf('/');
    int length = deletedObjectId.length();
    deletedObjectId = deletedObjectId.right( length - found -1);


    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        Q_UNUSED(json);
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            emit objectDeleted( deletedObjectId );
            if(!m_deleteQueue.isEmpty()) {
                deleteObject(m_deleteQueue.takeFirst());
            }
        }
        else {
            // error, drop the queue
            m_deleteQueue.empty();
        }
    } );

    initHeaders();
    request( RestManager::DELETE, doc.toUtf8() );
}


QNetworkReply* CouchDB::uploadFile(QUrl url, QString name)
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

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        disconnect(m_conn);
        if ( getHttpCode() == 201 ){
            currentObject = json.object();
            emit fileUploaded( currentObject);
        }

    } );

    return request( RestManager::POST, file.readAll() );
}


QNetworkReply* CouchDB::deleteFile(QString fileName)
{
    if (!isReady() || getMasterKey().isEmpty()) return NULL;

    setEndPoint( "files/" + fileName);

    m_conn = connect(this, &RestManager::replyFinished, [=]( QJsonDocument json){
        Q_UNUSED(json);
        disconnect(m_conn);
        if ( isLastRequestSuccessful() ) {
            emit objectDeleted( fileName );

        }
    } );

    initHeaders();
    setRawHeader("X-CouchDB-Master-Key", getMasterKey().toUtf8());

    return request( RestManager::DELETE );

}

