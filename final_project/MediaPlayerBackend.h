#pragma once

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <qqmlintegration.h>

class MediaPlayerBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Access the running instance through the 'mediaPlayer' context property")

    Q_PROPERTY(QUrl source READ source NOTIFY sourceChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(SourceType sourceType READ sourceType NOTIFY sourceChanged)
    Q_PROPERTY(QMediaPlayer::PlaybackState playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(QMediaPlayer::MediaStatus mediaStatus READ mediaStatus NOTIFY mediaStatusChanged)
    Q_PROPERTY(qint64 position READ position WRITE seek NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(bool isLive READ isLive NOTIFY sourceChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorOccurred)
    Q_PROPERTY(bool reconnecting READ reconnecting NOTIFY reconnectingChanged)

public:
    enum class SourceType { None, Local, Usb, Radio };
    Q_ENUM(SourceType)

    explicit MediaPlayerBackend(QObject *parent = nullptr);

    QUrl source() const { return m_source; }
    QString title() const { return m_title; }
    SourceType sourceType() const { return m_sourceType; }
    QMediaPlayer::PlaybackState playbackState() const;
    QMediaPlayer::MediaStatus mediaStatus() const;
    qint64 position() const;
    qint64 duration() const;
    bool isLive() const { return m_sourceType == SourceType::Radio; }
    float volume() const;
    void setVolume(float v);
    bool muted() const;
    void setMuted(bool m);
    QString errorString() const { return m_lastError; }
    bool reconnecting() const { return m_reconnecting; }

public slots:

    void playLocalFile(const QUrl &fileUrl, const QString &displayTitle = QString());
    void playUsbFile(const QUrl &fileUrl, const QString &displayTitle = QString());
    void playRadioStream(const QUrl &streamUrl, const QString &stationName = QString());

    void play();
    void pause();
    void togglePlayPause();
    void stop();
    void seek(qint64 ms);
    void stepVolume(float delta);

private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void handleErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
    void attemptRadioReconnect();

signals:
    void sourceChanged();
    void titleChanged();
    void playbackStateChanged();
    void mediaStatusChanged();
    void positionChanged();
    void durationChanged();
    void volumeChanged();
    void mutedChanged();
    void errorOccurred(const QString &message);
    void reconnectingChanged();

    void playbackFinished();

private:
    void setSource(const QUrl &url, SourceType type, const QString &displayTitle);

    QMediaPlayer *m_player = nullptr;
    QAudioOutput *m_audioOutput = nullptr;

    QUrl m_source;
    QString m_title;
    SourceType m_sourceType = SourceType::None;
    QString m_lastError;

    QTimer m_reconnectTimer;
    int m_reconnectAttempts = 0;
    static constexpr int kMaxReconnectAttempts = 6;
    bool m_reconnecting = false;
};
