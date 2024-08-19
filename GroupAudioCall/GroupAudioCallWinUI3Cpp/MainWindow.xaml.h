#pragma once

#include "MainWindow.g.h"

namespace winrt::GroupAudioCallWinUI3Cpp::implementation {
    struct MainWindow : MainWindowT<MainWindow> {
        MainWindow() {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();

        GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel CallViewModel();

    private :
        GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel m_groupAudioCallViewModel{ nullptr };
    };
}

namespace winrt::GroupAudioCallWinUI3Cpp::factory_implementation {
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow> {
    };
}
