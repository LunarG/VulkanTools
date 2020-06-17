#include "dlgvulkaninstallanalysis.h"

dlgVulkanInstallAnalysis::dlgVulkanInstallAnalysis(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant dlgVulkanInstallAnalysis::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

QModelIndex dlgVulkanInstallAnalysis::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return QModelIndex();
}

QModelIndex dlgVulkanInstallAnalysis::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
    return QModelIndex();
}

int dlgVulkanInstallAnalysis::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

int dlgVulkanInstallAnalysis::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

QVariant dlgVulkanInstallAnalysis::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
