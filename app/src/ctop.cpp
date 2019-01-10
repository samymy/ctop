#include "model.h"
#include "view.h"
#include <thread>

int main()
{
    Model model;
    View view;

    std::thread data_flow([&](){

        view.setModel(model);
        while (true) {
            model.readProcInfo();
            view.update();
            std::this_thread::sleep_for(std::chrono::seconds{3});
        }
    });
    data_flow.detach();

    view.exec();
}
