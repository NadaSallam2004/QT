#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QSettings>
#include <QObject>
class LanguageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ currentLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool isRtl READ isRtl NOTIFY languageChanged)

public:
    explicit LanguageManager(QGuiApplication &app, QQmlApplicationEngine &engine, QObject *parent = nullptr)
        : QObject(parent), m_app(app), m_engine(engine)
    {
        QSettings settings;
        m_currentLanguage = settings.value("language", "en").toString();
        loadLanguage(m_currentLanguage);
    }

    QString currentLanguage() const { return m_currentLanguage; }
    bool isRtl() const { return m_currentLanguage == "ar"; }

    Q_INVOKABLE void switchLanguage(const QString &languageCode)
    {
        if (languageCode == m_currentLanguage)
            return;

        loadLanguage(languageCode);

        QSettings settings;
        settings.setValue("language", languageCode);
    }

signals:
    void languageChanged();

private:
    void loadLanguage(const QString &languageCode)
    {
        m_app.removeTranslator(&m_translator);

        const QString qmPath = QStringLiteral(":/qt/qml/smart_home/i18n/SmartHomeDashboard_%1.qm").arg(languageCode);

        if (languageCode != "en" && m_translator.load(qmPath))
            m_app.installTranslator(&m_translator);

        m_currentLanguage = languageCode;

        m_app.setLayoutDirection(isRtl() ? Qt::RightToLeft : Qt::LeftToRight);

        m_engine.retranslate();

        emit languageChanged();
    }

    QGuiApplication &m_app;
    QQmlApplicationEngine &m_engine;
    QTranslator m_translator;
    QString m_currentLanguage;
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("SmartHome");
    QCoreApplication::setApplicationName("SmartHomeDashboard");

    QQmlApplicationEngine engine;

    LanguageManager languageManager(app, engine);
    engine.rootContext()->setContextProperty("languageManager", &languageManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("smart_home", "Main");

    return app.exec();
}

#include "main.moc"
