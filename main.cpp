#include <QtQml/qqmlcontext.h>
#include <filemodeldata.h>
#include <pkiqml.h>
#include <QGuiApplication>
#include <qmlappengine.h>

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  FileModelData model;
  model.setPath(model.documentPath());
  PKIQml pki;
  QMLAppEngine engine;
  QQmlContext *ctxt = engine.rootContext();
  ctxt->setContextProperty("qmlengine",&engine);
  ctxt->setContextProperty("folderModel", &model);
  ctxt->setContextProperty("pki", &pki);
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}
