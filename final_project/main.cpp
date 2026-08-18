#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickStyle>

#include "MediaPlayerBackend.h"
#include "AudioOutputModel.h"
#include "MediaLibraryModel.h"
#include "UsbVolumeWatcher.h"

#ifdef APP_WITH_BLUETOOTH_DISCOVERY
#include "BluetoothManager.h"
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("YourOrg");
    app.setApplicationName("Audio Player Live");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    auto *backend = new MediaPlayerBackend(&app);
    auto *outputModel = new AudioOutputModel(&app);
    outputModel->bind(backend->findChild<QAudioOutput *>());

    auto *library = new MediaLibraryModel(&app);
    QObject::connect(library, &MediaLibraryModel::requestPlay, backend,
                      [backend](const QUrl &url, const QString &title, MediaPlayerBackend::SourceType type) {
                          switch (type) {
                          case MediaPlayerBackend::SourceType::Local:
                              backend->playLocalFile(url, title);
                              break;
                          case MediaPlayerBackend::SourceType::Usb:
                              backend->playUsbFile(url, title);
                              break;
                          case MediaPlayerBackend::SourceType::Radio:
                              backend->playRadioStream(url, title);
                              break;
                          default:
                              break;
                          }
                      });

    QObject::connect(backend, &MediaPlayerBackend::playbackFinished,
                      library, &MediaLibraryModel::playNext);

    auto *usbWatcher = new UsbVolumeWatcher(&app);

    engine.rootContext()->setContextProperty("mediaPlayer", backend);
    engine.rootContext()->setContextProperty("audioOutputs", outputModel);
    engine.rootContext()->setContextProperty("mediaLibrary", library);
    engine.rootContext()->setContextProperty("usbWatcher", usbWatcher);

#ifdef APP_WITH_BLUETOOTH_DISCOVERY
    auto *bluetooth = new BluetoothManager(&app);
    engine.rootContext()->setContextProperty("bluetoothManager", bluetooth);
#else

    engine.rootContext()->setContextProperty("bluetoothManager", QVariant());
#endif

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("audio_player_live", "Main");

    return app.exec();
}
