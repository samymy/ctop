#ifndef VIEW_H
#define VIEW_H

#include <QApplication>
#include <QMainWindow>

#include "iview.h"

namespace Ui {
class View;
}

// Singleton wrapper for QApplication object
class App {

    static int argc;
    static char** argv;
    static QApplication app;

public:
    App() = delete;
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    static const QApplication& getApplication();
};

class View : public QMainWindow, public IView
{
    Q_OBJECT

    Ui::View* ui { nullptr };
    const IModel* model { nullptr };

    void setGeneralInfo();
    void setTasksInfo();
    void setCPUInfo();
    void setMemoryInfo();
    void setSwapInfo();
    void setProcessesInfo();

public:
    explicit View(QWidget *parent = 0);
    ~View();

    void setModel(const IModel&) override;
    void update() override;
    int exec() override;
};

#endif // VIEW_H
