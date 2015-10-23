//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace WallpaperChangeApp
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		
		void RegisterChangeWallpaperTask_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UnregisterChangeWallpaperTask_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void AttachProgressAndCompletedHandlers(Windows::ApplicationModel::Background::IBackgroundTaskRegistration^ task);
		void RegisterChangeWallpaperTask();
		void UnregisterChangeWallpaperTask();
		void UpdateUI();

		Platform::String^ GetWallpaperChangeBackgroundTaskStatus();
		bool isWallpaperChangeBackgroundTaskRegistered();

		static Platform::String^ WallpaperChangeBackgroundTaskProgress;
		static bool WallpaperChangeBackgroundTaskRegistered;
	};
}
