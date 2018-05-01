/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/

#include <assert.h>
#include <QDebug>
#include <QFileDialog>
#include <QMoveEvent>
#include <QPalette>
#include <QProcess>
#include <QToolButton>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCoreApplication>
#include <QGraphicsBlurEffect>
#include <QAbstractProxyModel>
#include <qwindow.h>

#include "ui_vktraceviewer.h"
#include "vktraceviewer.h"
#include "vktraceviewer_settings.h"
#include "vktraceviewer_output.h"

#include "vktraceviewer_controller_factory.h"
#include "vktraceviewer_qgeneratetracedialog.h"
#include "vktraceviewer_qtracefileloader.h"

#include "vkreplay_main.h"
//----------------------------------------------------------------------------------------------------------------------
// globals
//----------------------------------------------------------------------------------------------------------------------
static QString g_PROJECT_NAME = "VkTrace Viewer";

//-----------------------------------------------------------------------------
void loggingCallback(VktraceLogLevel level, const char* pMessage) {
    switch (level) {
        case VKTRACE_LOG_ERROR:
            gs_OUTPUT.error((uint64_t)-1, QString(pMessage));
            break;
        case VKTRACE_LOG_WARNING:
            gs_OUTPUT.warning((uint64_t)-1, QString(pMessage));
            break;
        case VKTRACE_LOG_DEBUG:
        case VKTRACE_LOG_VERBOSE:
        default:
            gs_OUTPUT.message((uint64_t)-1, QString(pMessage));
            break;
    }

#if defined(WIN32)
#if _DEBUG
    OutputDebugString(pMessage);
#endif
#endif
}

//-----------------------------------------------------------------------------

vktraceviewer::vktraceviewer(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::vktraceviewer),
      m_settingsDialog(this),
      m_pTraceFileModel(NULL),
      m_pProxyModel(NULL),
      m_pController(NULL),
      m_pGenerateTraceButton(NULL),
      m_pTimeline(NULL),
      m_pGenerateTraceDialog(NULL),
      m_bDelayUpdateUIForContext(false),
      m_bGeneratingTrace(false) {
    ui->setupUi(this);
    qRegisterMetaType<uint64_t>("uint64_t");
    qRegisterMetaType<VktraceLogLevel>("VktraceLogLevel");

    m_pTraceStatsTab = new QWidget();
    m_pTraceStatsTab->setObjectName(QStringLiteral("m_pTraceStatsTab"));
    m_pTraceStatsTabLayout = new QGridLayout(m_pTraceStatsTab);
    m_pTraceStatsTabLayout->setSpacing(6);
    m_pTraceStatsTabLayout->setContentsMargins(11, 11, 11, 11);
    m_pTraceStatsTabLayout->setObjectName(QStringLiteral("m_pTraceStatsTabLayout"));
    m_pTraceStatsTabText = new QTextBrowser(m_pTraceStatsTab);
    m_pTraceStatsTabText->setObjectName(QStringLiteral("m_pTraceStatsTabText"));
    m_pTraceStatsTabText->setLineWrapMode(QTextEdit::NoWrap);
    m_pTraceStatsTabLayout->addWidget(m_pTraceStatsTabText, 0, 0, 1, 1);

    QFont font("monospace", 10);
    m_pTraceStatsTabText->setFont(font);
    ui->outputTextBrowser->setFont(font);

    // Hide unused, default tab
    ui->stateTabWidget->removeTab(0);

    memset(&m_traceFileInfo, 0, sizeof(vktraceviewer_trace_file_info));

    m_settingsDialog.resize(g_settings.settings_dialog_width, g_settings.settings_dialog_height);
    connect(&m_settingsDialog, SIGNAL(SaveSettings(vktrace_SettingGroup*, unsigned int)), this,
            SLOT(on_settingsSaved(vktrace_SettingGroup*, unsigned int)));
    connect(&m_settingsDialog, SIGNAL(Resized(unsigned int, unsigned int)), this,
            SLOT(on_settingsDialogResized(unsigned int, unsigned int)));

    this->move(g_settings.window_position_left, g_settings.window_position_top);
    this->resize(g_settings.window_size_width, g_settings.window_size_height);

    connect(ui->outputTextBrowser, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(on_hyperlinkClicked(const QUrl&)));

    // setup Output Window
    vktraceviewer_output_init(ui->outputTextBrowser);
    vktrace_LogSetCallback(loggingCallback);
    vktrace_LogSetLevel(VKTRACE_LOG_ERROR);
    vktrace_LogAlways("Welcome to VkTraceViewer!");

    // cache the original background color of the search text box
    m_searchTextboxBackgroundColor = ui->searchTextBox->palette().base().color();

    // add buttons to toolbar
    m_pGenerateTraceButton = new QToolButton(ui->mainToolBar);
    m_pGenerateTraceButton->setText("Generate Trace...");
    m_pGenerateTraceButton->setEnabled(true);
    connect(m_pGenerateTraceButton, SIGNAL(clicked()), this, SLOT(prompt_generate_trace()));

    ui->mainToolBar->addWidget(m_pGenerateTraceButton);

    ui->treeView->setModel(NULL);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->setUniformRowHeights(true);

    // setup timeline
    m_pTimeline = new vktraceviewer_QTimelineView();
    if (m_pTimeline != NULL) {
        m_pTimeline->setMinimumHeight(100);
        connect(m_pTimeline, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slot_timeline_clicked(const QModelIndex&)));
        ui->timelineLayout->addWidget(m_pTimeline);
        ui->timelineLayout->removeWidget(ui->timelineViewPlaceholder);
        delete ui->timelineViewPlaceholder;
        ui->timelineViewPlaceholder = NULL;
    }

    m_pGenerateTraceDialog = new vktraceviewer_QGenerateTraceDialog(this);
    connect(m_pGenerateTraceDialog, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)), this,
            SLOT(OnOutputMessage(VktraceLogLevel, const QString&)));

    reset_tracefile_ui();

    // for now, remove these widgets since they are not used
    ui->bottomTabWidget->removeTab(ui->bottomTabWidget->indexOf(ui->machineInfoTab));
    ui->bottomTabWidget->removeTab(ui->bottomTabWidget->indexOf(ui->callStackTab));
}

vktraceviewer::~vktraceviewer() {
    close_trace_file();

    if (m_pTimeline != NULL) {
        delete m_pTimeline;
        m_pTimeline = NULL;
    }

    reset_view();

    delete ui;
    vktraceviewer_output_deinit();
}

void vktraceviewer::moveEvent(QMoveEvent* pEvent) {
    g_settings.window_position_left = pEvent->pos().x();
    g_settings.window_position_top = pEvent->pos().y();

    vktraceviewer_settings_updated();
}

void vktraceviewer::closeEvent(QCloseEvent* pEvent) {
    vktraceviewer_save_settings();
    close_trace_file();
    pEvent->accept();
}

void vktraceviewer::resizeEvent(QResizeEvent* pEvent) {
    g_settings.window_size_width = pEvent->size().width();
    g_settings.window_size_height = pEvent->size().height();

    vktraceviewer_settings_updated();
}

int vktraceviewer::add_custom_state_viewer(QWidget* pWidget, const QString& title, bool bBringToFront) {
    int tabIndex = ui->stateTabWidget->addTab(pWidget, title);

    if (bBringToFront) {
        ui->stateTabWidget->setCurrentWidget(pWidget);
    }

    return tabIndex;
}

void vktraceviewer::remove_custom_state_viewer(int const tabIndex) { ui->stateTabWidget->removeTab(tabIndex); }

void vktraceviewer::enable_custom_state_viewer(QWidget* pWidget, bool bEnabled) {
    ui->stateTabWidget->setTabEnabled(ui->stateTabWidget->indexOf(pWidget), bEnabled);
}

QToolButton* vktraceviewer::add_toolbar_button(const QString& title, bool bEnabled) {
    QToolButton* pButton = new QToolButton(ui->mainToolBar);
    pButton->setText(title);
    pButton->setEnabled(bEnabled);
    ui->mainToolBar->addWidget(pButton);
    return pButton;
}

void vktraceviewer::set_calltree_model(vktraceviewer_QTraceFileModel* pTraceFileModel, QAbstractProxyModel* pModel) {
    if (m_pTraceFileModel == pTraceFileModel && pModel == m_pProxyModel) {
        // Setting model and proxy to the same thing they are already set to, so there's nothing to do!
        return;
    }

    m_pTraceFileModel = pTraceFileModel;
    m_pProxyModel = pModel;

    if (m_pTimeline != NULL) {
        m_pTimeline->setModel(pTraceFileModel);
    }

    if (pModel == NULL) {
        ui->treeView->setModel(pTraceFileModel);
    } else {
        ui->treeView->setModel(pModel);
    }

    // initially show all columns before hiding others
    int columns = ui->treeView->header()->count();
    for (int i = 0; i < columns; i++) {
        ui->treeView->showColumn(i);
    }

    // hide columns that are not very important right now
    ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_TracerId);
    ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_BeginTime);
    ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_EndTime);
    ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_PacketSize);

    int width = ui->treeView->geometry().width();
    int firstEqualWidthColumnIndex = 0;
    float fSharedEqualWidthPct = 1.0;

    if (pModel != NULL && pModel->inherits("vktraceviewer_QGroupThreadsProxyModel")) {
        ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_EntrypointName);
        ui->treeView->hideColumn(vktraceviewer_QTraceFileModel::Column_ThreadId);

        ui->treeView->setColumnWidth(vktraceviewer_QTraceFileModel::Column_PacketIndex, width * 0.05);
        ui->treeView->setColumnWidth(vktraceviewer_QTraceFileModel::Column_CpuDuration, width * 0.08);
        firstEqualWidthColumnIndex = m_pTraceFileModel->columnCount();
        fSharedEqualWidthPct = 1.0f - 0.05f - 0.08f;
    } else {
        // entrypoint names get the most space
        ui->treeView->setColumnWidth(vktraceviewer_QTraceFileModel::Column_EntrypointName, width * 0.55);
        firstEqualWidthColumnIndex = 1;
        fSharedEqualWidthPct = 1.0f - 0.55f;
    }

    // the remaining space is divided among visible columns
    int visibleColumns = 0;
    for (int i = firstEqualWidthColumnIndex; i < columns; i++) {
        if (!ui->treeView->isColumnHidden(i)) {
            visibleColumns++;
        }
    }

    int scollbarWidth = qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    int columnWidths = (width - scollbarWidth) * (fSharedEqualWidthPct / visibleColumns);

    for (int i = firstEqualWidthColumnIndex; i < columns; i++) {
        if (!ui->treeView->isColumnHidden(i)) {
            ui->treeView->setColumnWidth(i, columnWidths);
        }
    }
}

void vktraceviewer::add_calltree_contextmenu_item(QAction* pAction) { ui->treeView->addAction(pAction); }

int indexOfColumn(QAbstractItemModel* pModel, const QString& text) {
    for (int i = 0; i < pModel->columnCount(); i++) {
        if (pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() == text) {
            return i;
        }
    }
    return -1;
}

void vktraceviewer::select_call_at_packet_index(unsigned long long packetIndex) {
    if (m_pTraceFileModel != NULL) {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QModelIndex start = m_pTraceFileModel->index(0, vktraceviewer_QTraceFileModel::Column_PacketIndex);

        QModelIndexList matches = m_pTraceFileModel->match(start, Qt::DisplayRole, QVariant(packetIndex), 1,
                                                           Qt::MatchFixedString | Qt::MatchRecursive | Qt::MatchWrap);
        if (matches.count() > 0) {
            // for some reason, we need to recreate the index such that the index and parent both are for column 0
            QModelIndex updatedMatch =
                m_pTraceFileModel->index(matches[0].row(), 0, m_pTraceFileModel->index(matches[0].parent().row(), 0));

            selectApicallModelIndex(updatedMatch, true, true);
            ui->treeView->setFocus();

            if (m_pTimeline != NULL) {
                m_pTimeline->setCurrentIndex(m_pTraceFileModel->index(
                    matches[0].row(), vktraceviewer_QTraceFileModel::Column_EntrypointName, QModelIndex()));
            }
        }

        QApplication::restoreOverrideCursor();
    }
}

void vktraceviewer::highlight_timeline_item(unsigned long long packetArrayIndex, bool bScrollTo, bool bSelect) {
    if (m_pTraceFileModel != NULL) {
        QModelIndex location = m_pTraceFileModel->index(packetArrayIndex, 0);

        if (m_pTimeline != NULL && m_pTimeline->currentIndex() != location) {
            // scroll to the index
            if (bScrollTo) {
                m_pTimeline->scrollTo(location);
            }

            // select the index
            if (bSelect) {
                m_pTimeline->setCurrentIndex(location);
            }
        }
    }
}

void vktraceviewer::on_replay_state_changed(bool bReplayInProgress) {
    bool bEnableUi = !bReplayInProgress;
    ui->treeView->setEnabled(bEnableUi);
    this->m_pGenerateTraceButton->setEnabled(bEnableUi);
    ui->nextDrawcallButton->setEnabled(bEnableUi);
    ui->prevDrawcallButton->setEnabled(bEnableUi);
    ui->searchNextButton->setEnabled(bEnableUi);
    ui->searchPrevButton->setEnabled(bEnableUi);
    ui->searchTextBox->setEnabled(bEnableUi);
    if (m_pTimeline != NULL) {
        m_pTimeline->setEnabled(bEnableUi);
    }
}

unsigned long long vktraceviewer::get_current_packet_index() {
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QModelIndex col0Index = currentIndex.sibling(currentIndex.row(), 0);
    QModelIndex index = mapTreeIndexToModel(col0Index);

    unsigned long long packetIndex = 0;
    if (index.isValid()) {
        vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
        if (pHeader != NULL) {
            assert(pHeader != NULL);
            packetIndex = pHeader->global_packet_index;
        }
    }
    return packetIndex;
}

void vktraceviewer::reset_view() {
    int count = ui->stateTabWidget->count();
    while (count > 0) {
        delete ui->stateTabWidget->widget(0);
        count = ui->stateTabWidget->count();
    }
}

void vktraceviewer::LogAlways(const QString& message) { OnOutputMessage(VKTRACE_LOG_VERBOSE, message); }

void vktraceviewer::LogWarning(const QString& message) { OnOutputMessage(VKTRACE_LOG_WARNING, message); }

void vktraceviewer::LogError(const QString& message) { OnOutputMessage(VKTRACE_LOG_ERROR, message); }

void vktraceviewer::add_setting_group(vktrace_SettingGroup* pGroup) {
    vktrace_SettingGroup_merge(pGroup, &g_pAllSettings, &g_numAllSettings);
}

unsigned int vktraceviewer::get_global_settings(vktrace_SettingGroup** ppGroups) {
    if (ppGroups != NULL) {
        *ppGroups = g_pAllSettings;
    }

    return g_numAllSettings;
}

bool vktraceviewer::prompt_load_new_trace(const QString& tracefile) {
    int ret = QMessageBox::warning(this, tr(g_PROJECT_NAME.toStdString().c_str()), tr("Would you like to load the new trace file?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (ret == QMessageBox::Yes) {
        //    // save current session if there is one
        //    if (m_openFilename.size() > 0 && m_pTraceReader != NULL && m_pApiCallTreeModel != NULL)
        //    {
        //        save_session_to_disk(get_sessionfile_path(m_openFilename, *m_pTraceReader), m_openFilename, m_pTraceReader,
        //        m_pApiCallTreeModel);
        //    }

        // try to open the new file
        open_trace_file_threaded(tracefile);
        return true;
    }

    return false;
}

void vktraceviewer::on_actionE_xit_triggered() { close(); }

void vktraceviewer::on_action_Open_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("vktrace Binary Files (*.vktrace *.*)"));

    if (!fileName.isEmpty()) {
        open_trace_file_threaded(fileName);
    }
}

typedef struct {
    uint64_t totalCpuExecutionTime;
    uint64_t totalTraceOverhead;
    uint32_t totalCallCount;
} vtvApiUsageStats;

void vktraceviewer::GenerateTraceFileStats() {
    // process trace file to extract some API usage stats
    // (NOTE: this could happen in a background thread)
    ui->bottomTabWidget->addTab(m_pTraceStatsTab, "Trace Stats");

    QString statText;
    m_pTraceStatsTabText->setText(statText);

    vtvApiUsageStats tmpNewStat;
    tmpNewStat.totalCallCount = 1;
    tmpNewStat.totalCpuExecutionTime = 0;
    tmpNewStat.totalTraceOverhead = 0;

    vtvApiUsageStats totalStats;
    totalStats.totalCallCount = 0;
    totalStats.totalCpuExecutionTime = 0;
    totalStats.totalTraceOverhead = 0;

    uint64_t totalTraceTime = 0;

    if (m_traceFileInfo.packetCount > 0) {
        uint64_t start = m_traceFileInfo.pPacketOffsets[0].pHeader->entrypoint_begin_time;
        uint64_t end = m_traceFileInfo.pPacketOffsets[m_traceFileInfo.packetCount - 1].pHeader->entrypoint_end_time;
        totalTraceTime = end - start;
    }

    QMap<uint16_t, vtvApiUsageStats> statMap;
    for (uint64_t i = 0; i < m_traceFileInfo.packetCount; i++) {
        vktrace_trace_packet_header* pHeader = m_traceFileInfo.pPacketOffsets[i].pHeader;
        if (pHeader->packet_id >= VKTRACE_TPI_VK_vkApiVersion) {
            totalStats.totalCallCount++;
            totalStats.totalCpuExecutionTime += (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time);
            totalStats.totalTraceOverhead += ((pHeader->vktrace_end_time - pHeader->vktrace_begin_time) -
                                              (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time));
            if (statMap.contains(pHeader->packet_id)) {
                statMap[pHeader->packet_id].totalCpuExecutionTime +=
                    (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time);
                statMap[pHeader->packet_id].totalTraceOverhead += ((pHeader->vktrace_end_time - pHeader->vktrace_begin_time) -
                                                                   (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time));
                statMap[pHeader->packet_id].totalCallCount++;
            } else {
                tmpNewStat.totalCpuExecutionTime = (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time);
                tmpNewStat.totalTraceOverhead = ((pHeader->vktrace_end_time - pHeader->vktrace_begin_time) -
                                                 (pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time));
                statMap.insert(pHeader->packet_id, tmpNewStat);
            }
        }
    }

    uint64_t appTime = totalTraceTime - totalStats.totalCpuExecutionTime - totalStats.totalTraceOverhead;
    uint64_t appDriverTime = totalTraceTime - totalStats.totalTraceOverhead;

    statText += "<table>";
    statText += QString("<tr><td>Total Trace Time:</td><td>%1 ns</td></tr>").arg(totalTraceTime);
    statText += QString("<tr><td>Total App+Driver Time:</td><td>%1 ns (%2%)</td></tr>")
                    .arg(appDriverTime)
                    .arg(100 * (float)appDriverTime / (float)totalTraceTime, 0, 'f', 1);
    statText += QString("<tr><td>Total Driver Time:</td><td>%1 ns (%2%)</td></tr>")
                    .arg(totalStats.totalCpuExecutionTime)
                    .arg(100 * (float)totalStats.totalCpuExecutionTime / (float)totalTraceTime, 0, 'f', 1);
    statText += QString("<tr><td>Total App Time:</td><td>%1 ns (%2%)</td></tr>")
                    .arg(appTime)
                    .arg(100 * (float)appTime / (float)totalTraceTime, 0, 'f', 1);
    statText += QString("<tr><td>Total VkTrace Overhead:</td><td>%1 ns (%2%)</td></tr>")
                    .arg(totalStats.totalTraceOverhead)
                    .arg(100 * (float)totalStats.totalTraceOverhead / (float)totalTraceTime, 0, 'f', 1);
    ;
    statText += QString("<tr><td>Total API Calls:</td><td>%1</td></tr>").arg(totalStats.totalCallCount);
    statText += QString("<tr><td>Total Entrypoints Called:</td><td>%1</td></tr>").arg(statMap.count());
    statText += "</table><br/>";

    statText +=
        "<table><thead><tr><th align='left'>Entrypoint</th><th align='right'># Calls (%Total)</th><th align='right'>Driver Time "
        "(%Total %AppDr %Driver)</th><th align='right'>VkTrace Overhead (%Total %vktrace)</th></tr></thead><tbody>";

    for (QMap<uint16_t, vtvApiUsageStats>::iterator i = statMap.begin(); i != statMap.end(); i++) {
        const vtvApiUsageStats stat = i.value();
        const char* entrypoint = m_pController->GetPacketIdString(i.key());
        if (entrypoint == NULL) {
            // Instead of printing entrypoint name, just print i
            statText += QString("<tr><td>%1</td>").arg(i.key());
        } else {
            statText += QString("<tr><td>%1</td>").arg(entrypoint);
        }

        // Note, because this is being output as HTML, consecutive spaces ' ' get removed, thus I'm using '?' to pad the numbers and
        // replacing them with "&nbsp;" below so that numbers are better aligned.
        // Also, this text field has been created with a fixed-width font.
        statText += QString("<td align='right'>%1 (%2%)</td>")
                        .arg(stat.totalCallCount)
                        .arg(100 * (float)stat.totalCallCount / (float)totalStats.totalCallCount, 5, 'f', 1, '?');
        statText += QString("<td align='right'>%1 ns (%2% %3%??%4%)</td>")
                        .arg(stat.totalCpuExecutionTime)
                        .arg(100 * (float)stat.totalCpuExecutionTime / (float)totalTraceTime, 5, 'f', 1, '?')
                        .arg(100 * (float)stat.totalCpuExecutionTime / (float)appDriverTime, 5, 'f', 1, '?')
                        .arg(100 * (float)stat.totalCpuExecutionTime / (float)totalStats.totalCpuExecutionTime, 5, 'f', 1, '?');
        statText += QString("<td align='right'>%1 ns (%2% %3%)</td></tr>")
                        .arg(stat.totalTraceOverhead)
                        .arg(100 * (float)stat.totalTraceOverhead / (float)totalTraceTime, 5, 'f', 1, '?')
                        .arg(100 * (float)stat.totalTraceOverhead / (float)totalStats.totalTraceOverhead, 5, 'f', 1, '?');

        statText.replace('?', "&nbsp;");
    }

    statText += "</tbody></table>";
    m_pTraceStatsTabText->setHtml(statText);
}

void vktraceviewer::onTraceFileLoaded(bool bSuccess, const vktraceviewer_trace_file_info& fileInfo,
                                      const QString& controllerFilename) {
    QApplication::restoreOverrideCursor();

    if (fileInfo.packetCount == 0) {
        LogWarning("The trace file has 0 packets.");
    } else if (fileInfo.pPacketOffsets == NULL) {
        LogError("No packet offsets read from trace file.");
        bSuccess = false;
    }

    if (!bSuccess) {
        LogAlways("...FAILED!");
        QMessageBox::critical(this, tr("Error"), tr("Could not open trace file."));
        close_trace_file();

        if (m_bGeneratingTrace) {
            // if the user was generating a trace file, but the trace failed to load,
            // then re-spawn the generate trace dialog.
            prompt_generate_trace();
        }
    } else {
        m_traceFileInfo = fileInfo;

        setWindowTitle(QString(m_traceFileInfo.filename) + " - " + g_PROJECT_NAME);
        LogAlways("...success!");

        // update settings to reflect the currently open file
        g_settings.trace_file_to_open = vktrace_allocate_and_copy(m_traceFileInfo.filename);
        vktraceviewer_settings_updated();

#ifndef USE_STATIC_CONTROLLER_LIBRARY
        if (!controllerFilename.isEmpty()) {
            m_pController = m_controllerFactory.Load(controllerFilename.toStdString().c_str());
        }
#else
        m_pController = vtvCreateQController();
#endif

        if (m_pController != NULL) {
            connect(m_pController, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)), this,
                    SLOT(OnOutputMessage(VktraceLogLevel, const QString&)));
            connect(m_pController, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)), this,
                    SLOT(OnOutputMessage(VktraceLogLevel, uint64_t, const QString&)));

            // Merge in settings from the controller.
            // This won't replace settings that may have already been loaded from disk.
            vktrace_SettingGroup_merge(m_pController->GetSettings(), &g_pAllSettings, &g_numAllSettings);

            // now update the controller with the loaded settings
            m_pController->UpdateFromSettings(g_pAllSettings, g_numAllSettings);

            //// trace file was loaded, now attempt to open additional session data
            // if (load_or_create_session(filename.c_str(), m_pTraceReader) == false)
            //{
            //    // failing to load session data is not critical, but may result in unexpected behavior at times.
            //    vktraceviewer_output_error("VkTraceViewer was unable to create a session folder to save viewing information.
            //    Functionality may be limited.");
            //}

            // Update the UI with the controller
            m_pController->LoadTraceFile(&m_traceFileInfo, this);
        }

        // update toolbar
        ui->searchTextBox->setEnabled(true);
        ui->searchPrevButton->setEnabled(true);
        ui->searchNextButton->setEnabled(true);

        ui->action_Close->setEnabled(true);
        ui->actionExport_API_Calls->setEnabled(true);

        ui->prevDrawcallButton->setEnabled(true);
        ui->nextDrawcallButton->setEnabled(true);

        // reset flag indicating that the ui may have been generating a trace file.
        m_bGeneratingTrace = false;

        GenerateTraceFileStats();
    }
}

void vktraceviewer::on_action_Close_triggered() { close_trace_file(); }

void vktraceviewer::close_trace_file() {
    if (m_pController != NULL) {
        ui->bottomTabWidget->removeTab(ui->bottomTabWidget->indexOf(m_pTraceStatsTab));
        m_pController->UnloadTraceFile();
#ifndef USE_STATIC_CONTROLLER_LIBRARY
        m_controllerFactory.Unload(&m_pController);
#else
        vtvDeleteQController(&m_pController);
#endif
    }

    if (m_pTimeline != NULL && m_pTimeline->model() != NULL) {
        m_pTimeline->setModel(NULL);
        m_pTimeline->repaint();
    }

    if (m_traceFileInfo.packetCount > 0) {
        for (unsigned int i = 0; i < m_traceFileInfo.packetCount; i++) {
            if (m_traceFileInfo.pPacketOffsets[i].pHeader != NULL) {
                vktrace_free(m_traceFileInfo.pPacketOffsets[i].pHeader);
                m_traceFileInfo.pPacketOffsets[i].pHeader = NULL;
            }
        }

        VKTRACE_DELETE(m_traceFileInfo.pPacketOffsets);
        m_traceFileInfo.pPacketOffsets = NULL;
        m_traceFileInfo.packetCount = 0;
    }

    if (m_traceFileInfo.pFile != NULL) {
        fclose(m_traceFileInfo.pFile);
        m_traceFileInfo.pFile = NULL;
    }

    if (m_traceFileInfo.filename != NULL) {
        vktrace_free(m_traceFileInfo.filename);
        m_traceFileInfo.filename = NULL;

        LogAlways("Closing trace file.");
        LogAlways("-------------------");

        // update settings
        if (g_settings.trace_file_to_open != NULL) {
            vktrace_free(g_settings.trace_file_to_open);
            g_settings.trace_file_to_open = NULL;
            vktraceviewer_settings_updated();
        }
    }

    setWindowTitle(g_PROJECT_NAME);

    reset_tracefile_ui();
}

void vktraceviewer::on_actionExport_API_Calls_triggered() {
    QString suggestedName(m_traceFileInfo.filename);

    int lastIndex = suggestedName.lastIndexOf('.');
    if (lastIndex != -1) {
        suggestedName = suggestedName.remove(lastIndex, suggestedName.size() - lastIndex);
    }
    suggestedName += "-ApiCalls.txt";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export API Calls"), suggestedName, tr("Text (*.txt)"));

    if (!fileName.isEmpty()) {
        FILE* pFile = fopen(fileName.toStdString().c_str(), "w");
        if (pFile == NULL) {
            LogError("Failed to open file for write. Can't export API calls.");
            return;
        }

        // iterate through every packet
        for (unsigned int i = 0; i < m_traceFileInfo.packetCount; i++) {
            vktraceviewer_trace_file_packet_offsets* pOffsets = &m_traceFileInfo.pPacketOffsets[i];
            vktrace_trace_packet_header* pHeader = pOffsets->pHeader;
            assert(pHeader != NULL);
            QString string = m_pTraceFileModel->get_packet_string(pHeader);

            // output packet string
            fprintf(pFile, "%s\n", string.toStdString().c_str());
        }

        fclose(pFile);
    }
}

void vktraceviewer::on_actionEdit_triggered() {
    // make sure dialog is at the size specified by the settings
    m_settingsDialog.resize(g_settings.settings_dialog_width, g_settings.settings_dialog_height);

    // set the groups so that the dialog is displaying the most recent information
    m_settingsDialog.setGroups(g_pAllSettings, g_numAllSettings);

    // execute the dialog
    m_settingsDialog.exec();
}

void vktraceviewer::on_settingsDialogResized(unsigned int width, unsigned int height) {
    // the dialog was resized, so update the settings
    g_settings.settings_dialog_width = width;
    g_settings.settings_dialog_height = height;

    // Update the setting groups with the new values.
    vktraceviewer_settings_updated();

    // re-set the groups so that the dialog is displaying the most recent information.
    m_settingsDialog.setGroups(g_pAllSettings, g_numAllSettings);
}

void vktraceviewer::on_settingsSaved(vktrace_SettingGroup* pUpdatedSettings, unsigned int numGroups) {
    // pUpdatedSettings is already pointing to the same location as g_pAllSettings
    g_numAllSettings = numGroups;

    // apply updated settings to the settingGroup so that the UI will respond to the changes
    vktrace_SettingGroup_Apply_Overrides(&g_settingGroup, pUpdatedSettings, numGroups);

    if (m_pController != NULL) {
        m_pController->UpdateFromSettings(pUpdatedSettings, numGroups);
    }

    vktraceviewer_save_settings();

    // react to changes in settings
    this->move(g_settings.window_position_left, g_settings.window_position_top);
    this->resize(g_settings.window_size_width, g_settings.window_size_height);
}

void vktraceviewer::open_trace_file_threaded(const QString& filename) {
    // close any existing trace
    close_trace_file();

    LogAlways("*********************");
    LogAlways("Opening trace file...");
    LogAlways(filename);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    vktraceviewer_QTraceFileLoader* pTraceLoader = new vktraceviewer_QTraceFileLoader();
    m_traceLoaderThread.setObjectName("TraceLoaderThread");
    pTraceLoader->moveToThread(&m_traceLoaderThread);

    connect(pTraceLoader, SIGNAL(OutputMessage(VktraceLogLevel, uint64_t, const QString&)), this,
            SLOT(OnOutputMessage(VktraceLogLevel, uint64_t, const QString&)), Qt::QueuedConnection);
    connect(pTraceLoader, SIGNAL(OutputMessage(VktraceLogLevel, const QString&)), this,
            SLOT(OnOutputMessage(VktraceLogLevel, const QString&)), Qt::QueuedConnection);

    connect(this, SIGNAL(LoadTraceFile(const QString&)), pTraceLoader, SLOT(loadTraceFile(QString)), Qt::QueuedConnection);

    connect(pTraceLoader, SIGNAL(TraceFileLoaded(bool, vktraceviewer_trace_file_info, const QString&)), this,
            SLOT(onTraceFileLoaded(bool, vktraceviewer_trace_file_info, const QString&)));
    connect(pTraceLoader, SIGNAL(Finished()), &m_traceLoaderThread, SLOT(quit()));
    connect(pTraceLoader, SIGNAL(Finished()), pTraceLoader, SLOT(deleteLater()));

    m_traceLoaderThread.start();

    // Signal the loader to start
    emit LoadTraceFile(filename);
}

void vktraceviewer::reset_tracefile_ui() {
    ui->action_Close->setEnabled(false);
    ui->actionExport_API_Calls->setEnabled(false);

    ui->prevDrawcallButton->setEnabled(false);
    ui->nextDrawcallButton->setEnabled(false);
    ui->searchTextBox->clear();
    ui->searchTextBox->setEnabled(false);
    ui->searchPrevButton->setEnabled(false);
    ui->searchNextButton->setEnabled(false);

    // VKTRACEVIEWER_DISABLE_BOTTOM_TAB(ui->machineInfoTab);
    // VKTRACEVIEWER_DISABLE_BOTTOM_TAB(ui->callStackTab);
}

void vktraceviewer::on_treeView_clicked(const QModelIndex& index) {
    QModelIndex col0Index = index.sibling(index.row(), vktraceviewer_QTraceFileModel::Column_EntrypointName);
    QModelIndex srcIndex = mapTreeIndexToModel(col0Index);
    if (srcIndex.isValid()) {
        selectApicallModelIndex(srcIndex, true, true);
    }
}

void vktraceviewer::on_hyperlinkClicked(const QUrl& link) {
    if (link.fileName() == "packet") {
        if (link.hasFragment()) {
            qulonglong index = link.fragment().toULongLong();
            this->select_call_at_packet_index(index);
        }
    }
}

void vktraceviewer::slot_timeline_clicked(const QModelIndex& index) { selectApicallModelIndex(index, true, true); }

void vktraceviewer::selectApicallModelIndex(QModelIndex index, bool scrollTo, bool select) {
    // make sure the index is visible in tree view
    QModelIndex treeIndex = mapTreeIndexFromModel(index);

    if (ui->treeView->currentIndex() != treeIndex && ui->treeView->isEnabled()) {
        QModelIndex parentIndex = treeIndex.parent();
        while (parentIndex.isValid()) {
            if (ui->treeView->isExpanded(parentIndex) == false) {
                ui->treeView->expand(parentIndex);
            }
            parentIndex = parentIndex.parent();
        }

        // scroll to the index
        if (scrollTo) {
            ui->treeView->scrollTo(treeIndex);
        }

        // select the index
        if (select) {
            ui->treeView->setCurrentIndex(treeIndex);
        }
    }

    if (m_pTimeline != NULL && m_pTimeline->currentIndex() != index) {
        // scroll to the index
        if (scrollTo) {
            m_pTimeline->scrollTo(index);
        }

        // select the index
        if (select) {
            m_pTimeline->setCurrentIndex(index);
        }
    }
}

void vktraceviewer::on_searchTextBox_textChanged(const QString& searchText) {
    QPalette palette(ui->searchTextBox->palette());
    palette.setColor(QPalette::Base, m_searchTextboxBackgroundColor);
    ui->searchTextBox->setPalette(palette);

    if (m_pTraceFileModel != NULL) {
        m_pTraceFileModel->set_highlight_search_string(searchText);
    }

    // need to briefly give the treeview focus so that it properly redraws and highlights the matching rows
    // then return focus to the search textbox so that typed keys are not lost
    ui->treeView->setFocus();
    ui->searchTextBox->setFocus();
}

void vktraceviewer::on_searchNextButton_clicked() {
    if (m_pTraceFileModel != NULL) {
        QModelIndex index = ui->treeView->currentIndex();
        if (!index.isValid()) {
            // If there was no valid current index, then get the first index in the trace file model.
            index = m_pTraceFileModel->index(0, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }

        // Need to make sure this index is in model-space
        if (index.model() == m_pProxyModel) {
            index = mapTreeIndexToModel(index);
        }

        // get the next item in the list
        // TODO: this means that we won't be able to hit "Next" and select the first item in the trace file.
        // However, if we move this into the loop below, then hitting "Next" will always result in finding the same item.
        index = index.sibling(index.row() + 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);

        // Can't get column count from the TreeView, so need to ask both the model and proxy if it exists.
        int columnCount = m_pTraceFileModel->columnCount(index);
        if (m_pProxyModel != NULL) {
            columnCount = m_pProxyModel->columnCount(index);
        }

        while (index.isValid()) {
            for (int column = 0; column < columnCount; column++) {
                if (ui->treeView->isColumnHidden(column)) continue;

                QModelIndex treeIndex = mapTreeIndexFromModel(m_pTraceFileModel->index(index.row(), column, index.parent()));
                if (treeIndex.data(Qt::DisplayRole).toString().contains(ui->searchTextBox->text(), Qt::CaseInsensitive)) {
                    // Get the first column so that it can be selected
                    QModelIndex srcIndex =
                        m_pTraceFileModel->index(index.row(), vktraceviewer_QTraceFileModel::Column_EntrypointName, index.parent());
                    selectApicallModelIndex(srcIndex, true, true);
                    ui->treeView->setFocus();
                    return;
                }
            }

            // wasn't found in that row, so check the next one
            index = index.sibling(index.row() + 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }
    }
}

void vktraceviewer::on_searchPrevButton_clicked() {
    if (m_pTraceFileModel != NULL) {
        QModelIndex index = ui->treeView->currentIndex();
        if (!index.isValid()) {
            // If there was no valid current index, then get the first index in the trace file model.
            index = m_pTraceFileModel->index(0, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }

        // Need to make sure this index is in model-space
        if (index.model() == m_pProxyModel) {
            index = mapTreeIndexToModel(index);
        }

        // get the next item in the list
        // TODO: this means that we won't be able to hit "Prev" and select the first item in the trace file.
        // However, if we move this into the loop below, then hitting "Prev" will always result in finding the same item.
        index = index.sibling(index.row() - 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);

        // Can't get column count from the TreeView, so need to ask both the model and proxy if it exists.
        int columnCount = m_pTraceFileModel->columnCount(index);
        if (m_pProxyModel != NULL) {
            columnCount = m_pProxyModel->columnCount(index);
        }

        while (index.isValid()) {
            for (int column = 0; column < columnCount; column++) {
                if (ui->treeView->isColumnHidden(column)) continue;

                QModelIndex treeIndex = mapTreeIndexFromModel(m_pTraceFileModel->index(index.row(), column, index.parent()));
                if (treeIndex.data(Qt::DisplayRole).toString().contains(ui->searchTextBox->text(), Qt::CaseInsensitive)) {
                    // Get the first column so that it can be selected
                    QModelIndex srcIndex =
                        m_pTraceFileModel->index(index.row(), vktraceviewer_QTraceFileModel::Column_EntrypointName, index.parent());
                    selectApicallModelIndex(srcIndex, true, true);
                    ui->treeView->setFocus();
                    return;
                }
            }

            // wasn't found in that row, so check the next one
            index = index.sibling(index.row() - 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }
    }
}

void vktraceviewer::on_prevDrawcallButton_clicked() {
    if (m_pTraceFileModel != NULL) {
        QModelIndex index = ui->treeView->currentIndex();

        index = mapTreeIndexToModel(index);

        QModelIndex indexAbove = index.sibling(index.row() - 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        while (indexAbove.isValid()) {
            vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)indexAbove.internalPointer();
            if (pHeader != NULL && m_pTraceFileModel->isDrawCall((VKTRACE_TRACE_PACKET_ID_VK)pHeader->packet_id)) {
                selectApicallModelIndex(indexAbove, true, true);
                ui->treeView->setFocus();
                return;
            }

            // that row is not a draw call, so check the prev one
            indexAbove = indexAbove.sibling(indexAbove.row() - 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }
    }
}

QModelIndex vktraceviewer::mapTreeIndexToModel(const QModelIndex& treeIndex) const {
    if (m_pProxyModel != NULL) {
        return m_pProxyModel->mapToSource(treeIndex);
    }

    return treeIndex;
}

QModelIndex vktraceviewer::mapTreeIndexFromModel(const QModelIndex& modelIndex) const {
    if (m_pProxyModel != NULL) {
        return m_pProxyModel->mapFromSource(modelIndex);
    }

    return modelIndex;
}

void vktraceviewer::on_nextDrawcallButton_clicked() {
    if (m_pTraceFileModel != NULL) {
        QModelIndex index = ui->treeView->currentIndex();

        index = mapTreeIndexToModel(index);

        QModelIndex indexBelow = index.sibling(index.row() + 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        while (indexBelow.isValid()) {
            vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)indexBelow.internalPointer();
            if (pHeader != NULL && m_pTraceFileModel->isDrawCall((VKTRACE_TRACE_PACKET_ID_VK)pHeader->packet_id)) {
                selectApicallModelIndex(indexBelow, true, true);
                ui->treeView->setFocus();
                return;
            }

            // that row is not a draw call, so check the next one
            indexBelow = indexBelow.sibling(indexBelow.row() + 1, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }
    }
}

void vktraceviewer::on_searchTextBox_returnPressed() {
    if (m_pTraceFileModel != NULL) {
        QModelIndex index = ui->treeView->indexBelow(ui->treeView->currentIndex());
        bool bFound = false;

        // search down from the current index
        while (index.isValid()) {
            for (int column = 0; column < m_pTraceFileModel->columnCount(index); column++) {
                if (m_pTraceFileModel->data(m_pTraceFileModel->index(index.row(), column, index.parent()), Qt::DisplayRole)
                        .toString()
                        .contains(ui->searchTextBox->text(), Qt::CaseInsensitive)) {
                    bFound = true;
                    break;
                }
            }

            if (bFound) {
                break;
            } else {
                // wasn't found in that row, so check the next one
                index = ui->treeView->indexBelow(index);
            }
        }

        // if not found yet, then search from the root down to the current node
        if (!bFound) {
            index = m_pTraceFileModel->index(0, 0);

            while (index.isValid() && index != ui->treeView->currentIndex()) {
                for (int column = 0; column < m_pTraceFileModel->columnCount(index); column++) {
                    if (m_pTraceFileModel->data(m_pTraceFileModel->index(index.row(), column, index.parent()), Qt::DisplayRole)
                            .toString()
                            .contains(ui->searchTextBox->text(), Qt::CaseInsensitive)) {
                        bFound = true;
                        break;
                    }
                }

                if (bFound) {
                    break;
                } else {
                    // wasn't found in that row, so check the next one
                    index = ui->treeView->indexBelow(index);
                }
            }
        }

        if (bFound && index.isValid()) {
            // a valid item was found, scroll to it and select it
            selectApicallModelIndex(index, true, true);
            ui->searchTextBox->setFocus();
        } else {
            // no items were found, so set the textbox background to red (it will get cleared to the original color if the user
            // edits the search text)
            QPalette palette(ui->searchTextBox->palette());
            palette.setColor(QPalette::Base, Qt::red);
            ui->searchTextBox->setPalette(palette);
        }
    }
}

void vktraceviewer::on_contextComboBox_currentIndexChanged(int index) {}

void vktraceviewer::prompt_generate_trace() {
    m_bGeneratingTrace = true;

    bool bShowDialog = true;
    while (bShowDialog) {
        int code = m_pGenerateTraceDialog->exec();
        if (code != vktraceviewer_QGenerateTraceDialog::Succeeded) {
            m_bGeneratingTrace = false;
            bShowDialog = false;
        } else {
            QFileInfo fileInfo(m_pGenerateTraceDialog->get_trace_file_path());
            if (code == vktraceviewer_QGenerateTraceDialog::Succeeded && fileInfo.exists()) {
                bShowDialog = false;
                if (prompt_load_new_trace(fileInfo.canonicalFilePath()) == false) {
                    // The user decided not to load the trace file, so clear the generatingTrace flag.
                    m_bGeneratingTrace = false;
                }
            } else {
                LogError("Failed to trace the application.");
                QMessageBox::critical(this, "Error", "Failed to trace application.");
            }
        }
    }
}

void vktraceviewer::OnOutputMessage(VktraceLogLevel level, const QString& message) {
    OnOutputMessage(level, (uint64_t)-1, message);
}

void vktraceviewer::OnOutputMessage(VktraceLogLevel level, uint64_t packetIndex, const QString& message) {
    switch (level) {
        case VKTRACE_LOG_ERROR: {
            gs_OUTPUT.error(packetIndex, message);
            break;
        }
        case VKTRACE_LOG_WARNING: {
            gs_OUTPUT.warning(packetIndex, message);
            break;
        }
        case VKTRACE_LOG_DEBUG:
        case VKTRACE_LOG_VERBOSE:
        default: {
            gs_OUTPUT.message(packetIndex, message);
            break;
        }
    }
}
