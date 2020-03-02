#include <QFileDialog>
#include <QMessageBox>

#include "dlgcustompaths.h"
#include "ui_dlgcustompaths.h"

dlgCustomPaths::dlgCustomPaths(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgCustomPaths)
    {
    ui->setupUi(this);

    ui->treeWidget->headerItem()->setText(0,tr("Custom Search Paths & Layers"));

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    // Populate the tree
    for(uint32_t i = 0; i < pVulkanConfig->nAdditionalSearchPathCount; i++ ) {
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, pVulkanConfig->additionalSearchPaths[i]);
        ui->treeWidget->addTopLevelItem(pItem);





        }

    }

dlgCustomPaths::~dlgCustomPaths()
    {
    delete ui;
    }



void dlgCustomPaths::on_pushButtonAdd_clicked()
    {

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString customFolder =
        dialog.getExistingDirectory(this, tr("Add Custom Layer Folder"), "");

    if(!customFolder.isEmpty()) {
        pVulkanConfig->additionalSearchPaths << customFolder;
        pVulkanConfig->nAdditionalSearchPathCount++;
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, customFolder);
        ui->treeWidget->addTopLevelItem(pItem);
        pVulkanConfig->saveAdditionalSearchPaths();

        // Update this list of layers available if any are found.
        pVulkanConfig->loadLayersFromPath(customFolder, pVulkanConfig->customLayers, LAYER_TYPE_CUSTOM);
        }
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_treeWidget_itemSelectionChanged
/// Don't make remove button accessable unless an item has been selected
void dlgCustomPaths::on_treeWidget_itemSelectionChanged()
    {
    ui->pushButtonRemove->setEnabled(true);
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief dlgCustomPaths::on_pushButtonRemove_clicked
/// Remove the selected custom search path
void dlgCustomPaths::on_pushButtonRemove_clicked()
    {
    // Which one is selected?
    QTreeWidgetItem *pSelected = ui->treeWidget->currentItem();

    QMessageBox msg;
    msg.setText(pSelected->text(0));
    msg.setInformativeText(tr("Delete this custom path?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if(msg.exec() == QMessageBox::No)
        return;

    }
