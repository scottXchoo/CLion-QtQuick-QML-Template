#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "CarModel.h"

#include <QtQml/qqmlextensionplugin.h>
#include "ribbonui.h"

#ifdef RIBBONUI_BUILD_STATIC_LIB
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#define Q_IMPORT_QML_PLUGIN(PLUGIN) \
Q_IMPORT_PLUGIN(PLUGIN)
extern void qml_static_register_types_RibbonUI();
#endif
Q_IMPORT_QML_PLUGIN(RibbonUIPlugin)
#endif

int main(int argc, char *argv[]) {
  RibbonUI::init(); // Must set before QGuiApplication
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  RibbonUI::registerTypes(nullptr);

  CarModel carModel;
  engine.rootContext()->setContextProperty("carModel", &carModel);

#ifdef RIBBONUI_BUILD_STATIC_LIB
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  qml_static_register_types_RibbonUI();
#endif
#endif

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
				   &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
		  QCoreApplication::exit(-1);
	  }, Qt::QueuedConnection);
  engine.load(url);

  return QGuiApplication::exec();
}
