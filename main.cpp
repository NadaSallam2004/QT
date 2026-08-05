#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // كل منطق التطبيق في QML/JavaScript - مفيش أي كود C++ هنا غير التشغيل
    engine.loadFromModule("smart_home", "Main");

    return app.exec();
}
