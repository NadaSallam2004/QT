#include "MediaPlayerBackend.h"

#include <QFileInfo>
#include <algorithm>

MediaPlayerBackend::MediaPlayerBackend(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
{
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &MediaPlayerBackend::playbackStateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &MediaPlayerBackend::handleMediaStatusChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &MediaPlayerBackend::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &MediaPlayerBackend::durationChanged);
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, &MediaPlayerBackend::handleErrorOccurred);
    connect(m_audioOutput, &QAudioOutput::volumeChanged,
            this, &MediaPlayerBackend::volumeChanged);
    connect(m_audioOutput, &QAudioOutput::mutedChanged,
            this, &MediaPlayerBackend::mutedChanged);

    m_reconnectTimer.setSingleShot(true);
    connect(&m_reconnectTimer, &QTimer::timeout,
            this, &MediaPlayerBackend::attemptRadioReconnect);
}

QMediaPlayer::PlaybackState MediaPlayerBackend::playbackState() const
{
    return m_player->playbackState();
}

QMediaPlayer::MediaStatus MediaPlayerBackend::mediaStatus() const
{
    return m_player->mediaStatus();
}

qint64 MediaPlayerBackend::position() const
{
    return m_player->position();
}

qint64 MediaPlayerBackend::duration() const
{
    return m_player->duration();
}

float MediaPlayerBackend::volume() const
{
    return m_audioOutput->volume();
}

void MediaPlayerBackend::setVolume(float v)
{
    m_audioOutput->setVolume(std::clamp(v, 0.0f, 1.0f));
}

bool MediaPlayerBackend::muted() const
{
    return m_audioOutput->isMuted();
}

void MediaPlayerBackend::setMuted(bool m)
{
    m_audioOutput->setMuted(m);
}

void MediaPlayerBackend::setSource(const QUrl &url, SourceType type, const QString &displayTitle)
{
    m_reconnectTimer.stop();
    m_reconnectAttempts = 0;
    if (m_reconnecting) {
        m_reconnecting = false;
        emit reconnectingChanged();
    }

    m_source = url;
    m_sourceType = type;
    m_title = displayTitle.isEmpty() ? QFileInfo(url.isLocalFile() ? url.toLocalFile() : url.toString()).fileName()
                                      : displayTitle;
    m_lastError.clear();

    m_player->setSource(url);
    emit sourceChanged();
    emit titleChanged();

    m_player->play();
}

void MediaPlayerBackend::playLocalFile(const QUrl &fileUrl, const QString &displayTitle)
{
    setSource(fileUrl, SourceType::Local, displayTitle);
}

void MediaPlayerBackend::playUsbFile(const QUrl &fileUrl, const QString &displayTitle)
{
    setSource(fileUrl, SourceType::Usb, displayTitle);
}

void MediaPlayerBackend::playRadioStream(const QUrl &streamUrl, const QString &stationName)
{
    setSource(streamUrl, SourceType::Radio, stationName);
}

void MediaPlayerBackend::play()
{
    if (m_source.isValid())
        m_player->play();
}

void MediaPlayerBackend::pause()
{
    m_player->pause();
}

void MediaPlayerBackend::togglePlayPause()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState)
        pause();
    else
        play();
}

void MediaPlayerBackend::stop()
{
    m_reconnectTimer.stop();
    m_reconnectAttempts = 0;
    if (m_reconnecting) {
        m_reconnecting = false;
        emit reconnectingChanged();
    }
    m_player->stop();
}

void MediaPlayerBackend::seek(qint64 ms)
{

    if (isLive())
        return;
    m_player->setPosition(ms);
}

void MediaPlayerBackend::stepVolume(float delta)
{
    setVolume(volume() + delta);
}

void MediaPlayerBackend::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    emit mediaStatusChanged();

    if (status == QMediaPlayer::EndOfMedia && !isLive()) {
        emit playbackFinished();
    }
}

void MediaPlayerBackend::handleErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    if (error == QMediaPlayer::NoError)
        return;

    m_lastError = errorString;
    emit errorOccurred(errorString);

    if (m_sourceType == SourceType::Radio) {
        if (m_reconnectAttempts < kMaxReconnectAttempts) {
            if (!m_reconnecting) {
                m_reconnecting = true;
                emit reconnectingChanged();
            }
            const int delayMs = std::min(1000 * (1 << m_reconnectAttempts), 30000);
            ++m_reconnectAttempts;
            m_reconnectTimer.start(delayMs);
        } else {
            m_reconnecting = false;
            emit reconnectingChanged();
        }
    }
}

void MediaPlayerBackend::attemptRadioReconnect()
{
    if (m_sourceType != SourceType::Radio || !m_source.isValid())
        return;

    m_player->setSource(m_source);
    m_player->play();
}
