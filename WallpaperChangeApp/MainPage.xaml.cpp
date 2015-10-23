//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace WallpaperChangeApp;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::System::UserProfile;
using namespace Windows::ApplicationModel::Background;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

bool MainPage::WallpaperChangeBackgroundTaskRegistered = false;
String^ MainPage::WallpaperChangeBackgroundTaskProgress = "Progress initialized";

MainPage::MainPage()
{
	InitializeComponent();
	if (isWallpaperChangeBackgroundTaskRegistered()) {
		WallpaperChangeBackgroundTaskRegistered = true;
	}
	UpdateUI();
}

void MainPage::RegisterChangeWallpaperTask_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	RegisterChangeWallpaperTask();
	UpdateUI();
}

void MainPage::UnregisterChangeWallpaperTask_click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	UnregisterChangeWallpaperTask();
	UpdateUI();
}

void MainPage::RegisterChangeWallpaperTask()
{
	if (!WallpaperChangeBackgroundTaskRegistered)
	{
		auto builder = ref new BackgroundTaskBuilder();
		builder->Name = "WallpaperChangeBackgroundTask";
		builder->TaskEntryPoint = "WallpaperChangeBackgroundTask.WallpaperChanger";
		//builder->SetTrigger(ref new SystemTrigger(SystemTriggerType::TimeZoneChange, false)); //TODO only for debugging

		builder->SetTrigger(ref new TimeTrigger(15, false)); //a trigger that repeats at 1440-minute intervals (every 24h)
		//builder->AddCondition(ref new SystemCondition(SystemConditionType::BackgroundWorkCostNotHigh));

		BackgroundTaskRegistration^ task = builder->Register();
		WallpaperChangeBackgroundTaskRegistered = true;

		//
		// Remove previous completion status from local settings.
		//
		auto settings = ApplicationData::Current->LocalSettings->Values;
		settings->Remove("WallpaperChangeBackgroundTask");

		AttachProgressAndCompletedHandlers(task);
		UpdateUI();

		auto completed = [this](BackgroundTaskRegistration^ task, BackgroundTaskCompletedEventArgs^ args)
		{
			UpdateUI();
		};
		task->Completed += ref new BackgroundTaskCompletedEventHandler(completed);
	}
}

void MainPage::UnregisterChangeWallpaperTask()
{
	auto iter = BackgroundTaskRegistration::AllTasks->First();
	auto hascur = iter->HasCurrent;
	while (hascur)
	{
		auto cur = iter->Current->Value;

		if (cur->Name == "WallpaperChangeBackgroundTask")
		{
			cur->Unregister(true);
			WallpaperChangeBackgroundTaskRegistered = false;
		}

		hascur = iter->MoveNext();
	}
}

void MainPage::AttachProgressAndCompletedHandlers(Windows::ApplicationModel::Background::IBackgroundTaskRegistration^ task)
{
	
	auto progress = [this](BackgroundTaskRegistration^ task, BackgroundTaskProgressEventArgs^ args)
	{
		auto progress = "Progress: " + args->Progress + "%";
		MainPage::WallpaperChangeBackgroundTaskProgress = progress;
		UpdateUI();
	};
	task->Progress += ref new BackgroundTaskProgressEventHandler(progress);
	

	auto completed = [this](BackgroundTaskRegistration^ task, BackgroundTaskCompletedEventArgs^ args)
	{
		UpdateUI();
	};
	task->Completed += ref new BackgroundTaskCompletedEventHandler(completed);
}

bool MainPage::isWallpaperChangeBackgroundTaskRegistered()
{
	bool taskRegistered = false;
	Platform::String^ exampleTaskName = "WallpaperChangeBackgroundTask";

	auto iter = BackgroundTaskRegistration::AllTasks->First();
	auto hascur = iter->HasCurrent;

	while (hascur)
	{
		auto cur = iter->Current->Value;

		if (cur->Name == exampleTaskName)
		{
			taskRegistered = true;
			break;
		}

		hascur = iter->MoveNext();
	}

	return taskRegistered;
}


void MainPage::UpdateUI()
{
	auto uiDelegate = [this]()
	{
		activateWallpaperChangeBackgroundTaskBtn->IsEnabled = !WallpaperChangeBackgroundTaskRegistered;
		deactivateWallpaperChangeBackgroundTaskBtn->IsEnabled = WallpaperChangeBackgroundTaskRegistered;
		Status->Text = GetWallpaperChangeBackgroundTaskStatus();
		Progress->Text = MainPage::WallpaperChangeBackgroundTaskProgress;
	};

	auto handler = ref new Windows::UI::Core::DispatchedHandler(uiDelegate, Platform::CallbackContext::Any);
	Dispatcher->RunAsync(CoreDispatcherPriority::Normal, handler);
}

String^ MainPage::GetWallpaperChangeBackgroundTaskStatus()
{
	String^ status = WallpaperChangeBackgroundTaskRegistered ? "Registered" : "Unregistered";
	auto settings = ApplicationData::Current->LocalSettings->Values;
	if (settings->HasKey("WallpaperChangeBackgroundTask"))
	{
		status += " - " + dynamic_cast<String^>(settings->Lookup("WallpaperChangeBackgroundTask"));
	}

	return status;
}
