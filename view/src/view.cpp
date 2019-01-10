#include "view.h"
#include "ui_view.h"

#include <sstream>
#include <iomanip>

int App::argc = 0;
char** App::argv = nullptr;
QApplication App::app(App::argc, App::argv);

const QApplication& App::getApplication()
{
    return App::app;
}

View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);

    ui->general->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->tasks->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->cpu->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->memory->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->swap->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

View::~View()
{
    delete ui;
}

void View::setModel(const IModel& imodel)
{
    model = &imodel;
}

void View::setGeneralInfo()
{
    std::stringstream ss;

    ss << "ctop - " << model->getCurrentTime() << " up " << model->getRunningTime();
    ss << ", " << model->getUserSessions() << (model->getUserSessions() > 1 ? " users" : " user");
    ss << ", load average: " << model->getLoadAverageOverOneMinute() << ", ";
    ss << model->getLoadAverageOverFiveMinutes() << ", " << model->getLoadAverageOverFifteenMinutes();

    ui->general->setText(ss.str().c_str());
}

void View::setTasksInfo()
{
    std::stringstream ss;
    unsigned total = model->getRunningTasksCount() +
                        model->getSleepingTasksCount() +
                        model->getStoppedTasksCount() +
                        model->getZombieTasksCount();

    ss << "Tasks: " << total << " total, " << model->getRunningTasksCount() << " running, ";
    ss << model->getSleepingTasksCount() << " sleeping, ";
    ss << model->getStoppedTasksCount() << " stopped, ";
    ss << model->getZombieTasksCount() << " zombie";

    ui->tasks->setText(ss.str().c_str());
}

void View::setCPUInfo()
{
    std::stringstream ss;

    std::fixed(ss);
    ss.precision(1);

    ss << "%Cpu(s): " << model->getUserspaceCPUUsageTime() << " us, ";
    ss << model->getKernelspaceCPUUsageTime() << " sy, ";
    ss << model->getNIProcessesCPUUsageTime() << " ni, ";
    ss << model->getIdleCPUTime() << " id, ";
    ss << model->getIOCPUUsageTime() << " wa, ";
    ss << model->getHardwareInterruptsCPUUsageTime() << " hi, ";
    ss << model->getSoftwareInterruptsCPUUsageTime() << " si, ";
    ss << model->getStealCPUUsageTime() << " st";

    ui->cpu->setText(ss.str().c_str());
}

void View::setMemoryInfo()
{
    std::stringstream ss;
    unsigned total = model->getFreeMemorySize() +
                        model->getUsedMemorySize() +
                        model->getCacheMemorySize();

    ss << "KiB Mem: " << total << " total, ";
    ss << model->getFreeMemorySize() << " free, ";
    ss << model->getUsedMemorySize() << " used, ";
    ss << model->getCacheMemorySize() << " buff/cache";

    ui->memory->setText(ss.str().c_str());
}

void View::setSwapInfo()
{
    std::stringstream ss;
    unsigned total = model->getFreeSwapSize() +
                        model->getUsedSwapSize();

    ss << "KiB Swap: " << total << " total, ";
    ss << model->getFreeSwapSize() << " free, ";
    ss << model->getUsedSwapSize() << " used, ";
    ss << model->getAvailableMemorySize() << " avail Mem";

    ui->swap->setText(ss.str().c_str());
}

void View::setProcessesInfo()
{
    const std::vector<const ProcessT*>& processes = model->getProcesses();
    std::stringstream ss;

    ui->processesTableWidget->clearContents();
    ui->processesTableWidget->setRowCount(processes.size());

    for (unsigned row = 0; row < processes.size(); ++row) {

        ui->processesTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(processes[row]->pid)));
        ui->processesTableWidget->setItem(row, 1, new QTableWidgetItem(QString(processes[row]->user.c_str())));
        ui->processesTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(processes[row]->priority)));
        ui->processesTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(processes[row]->nice_value)));
        ui->processesTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(processes[row]->virtual_memory)));
        ui->processesTableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(processes[row]->resident_memory)));
        ui->processesTableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(processes[row]->shared_memory)));
        ui->processesTableWidget->setItem(row, 7, new QTableWidgetItem(QString(processes[row]->status)));
        ss.str("");
        ss << std::fixed << std::setprecision(1) << processes[row]->cpu_usage;
        ui->processesTableWidget->setItem(row, 8, new QTableWidgetItem(QString(ss.str().c_str())));
        ss.str("");
        ss << std::fixed << std::setprecision(1) << processes[row]->memory_usage;
        ui->processesTableWidget->setItem(row, 9, new QTableWidgetItem(QString(ss.str().c_str())));
        ui->processesTableWidget->setItem(row, 10, new QTableWidgetItem(QString(processes[row]->time.c_str())));
        ui->processesTableWidget->setItem(row, 11, new QTableWidgetItem(QString(processes[row]->command.c_str())));

        for (unsigned col = 0; col < (unsigned)ui->processesTableWidget->columnCount(); ++col)
            ui->processesTableWidget->item(row, col)->setFlags(ui->processesTableWidget->item(row, col)->flags() ^
                                                               Qt::ItemIsEditable);
    }
}

void View::update()
{
    if (model) {

        setGeneralInfo();
        setTasksInfo();
        setCPUInfo();
        setMemoryInfo();
        setSwapInfo();
        setProcessesInfo();
    }
}

int View::exec()
{
    show();

    return App::getApplication().exec();
}
