#ifndef BAAS_REGISTER_H
#define BAAS_REGISTER_H
#include <QtCore/QCoreApplication>
#include <QtQml/QQmlEngine>

#include "qqml.h"

#include "parse.h"
#include "baasmodel.h"

void registerFilterTypes()
{
    qmlRegisterType<Parse>("BaaS", 1, 0, "Parse");
    qmlRegisterType<BaaSModel>("BaaS", 1, 0, "BaaSModel");

}

Q_COREAPP_STARTUP_FUNCTION(registerFilterTypes)

#endif // BAAS_REGISTER_H
