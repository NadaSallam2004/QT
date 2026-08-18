#include "AudioOutputModel.h"

#include <QMediaDevices>

AudioOutputModel::AudioOutputModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();

    connect(&m_deviceMonitor, &QMediaDevices::audioOutputsChanged,
            this, &AudioOutputModel::refresh);
}

void AudioOutputModel::bind(QAudioOutput *output)
{
    m_boundOutput = output;

    if (!m_devices.isEmpty() && m_boundOutput)
        m_boundOutput->setDevice(m_devices.at(m_currentIndex));
}

void AudioOutputModel::refresh()
{
    beginResetModel();
    m_devices = QMediaDevices::audioOutputs();
    endResetModel();

    const QAudioDevice def = QMediaDevices::defaultAudioOutput();
    int idx = 0;
    for (int i = 0; i < m_devices.size(); ++i) {
        if (m_devices.at(i).id() == def.id()) {
            idx = i;
            break;
        }
    }

    bool activeDeviceStillPresent = false;
    if (m_boundOutput) {
        for (const auto &d : std::as_const(m_devices)) {
            if (d.id() == m_boundOutput->device().id()) {
                activeDeviceStillPresent = true;
                break;
            }
        }
    }

    if (!activeDeviceStillPresent)
        setCurrentIndex(idx);
    else
        emit currentIndexChanged();
}

int AudioOutputModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_devices.size();
}

QVariant AudioOutputModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_devices.size())
        return {};

    const QAudioDevice &device = m_devices.at(index.row());
    switch (role) {
    case NameRole:
        return device.description();
    case IsDefaultRole:
        return device.isDefault();
    case IsLikelyBluetoothRole:
        return isLikelyBluetooth(device);
    default:
        return {};
    }
}

QHash<int, QByteArray> AudioOutputModel::roleNames() const
{
    return {
        { NameRole, "name" },
        { IsDefaultRole, "isDefault" },
        { IsLikelyBluetoothRole, "isLikelyBluetooth" },
    };
}

void AudioOutputModel::setCurrentIndex(int idx)
{
    if (idx < 0 || idx >= m_devices.size())
        return;

    m_currentIndex = idx;
    if (m_boundOutput)
        m_boundOutput->setDevice(m_devices.at(idx));
    emit currentIndexChanged();
}

QString AudioOutputModel::currentDeviceName() const
{
    if (m_currentIndex < 0 || m_currentIndex >= m_devices.size())
        return QStringLiteral("No output device");
    return m_devices.at(m_currentIndex).description();
}

bool AudioOutputModel::isLikelyBluetooth(const QAudioDevice &device)
{
    static const QStringList patterns = {
        QStringLiteral("bluetooth"), QStringLiteral("a2dp"), QStringLiteral("hands-free"),
        QStringLiteral("hfp"), QStringLiteral("headset"), QStringLiteral("airpods"),
        QStringLiteral("bt "),
    };
    const QString name = device.description().toLower();
    for (const auto &p : patterns) {
        if (name.contains(p))
            return true;
    }
    return false;
}
