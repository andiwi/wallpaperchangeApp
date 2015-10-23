﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class StackPanel;
                ref class Button;
                ref class TextBlock;
            }
        }
    }
}

namespace WallpaperChangeApp
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::StackPanel^ contentPanel;
        private: ::Windows::UI::Xaml::Controls::Button^ activateWallpaperChangeBackgroundTaskBtn;
        private: ::Windows::UI::Xaml::Controls::Button^ deactivateWallpaperChangeBackgroundTaskBtn;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Status;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Progress;
    };
}

