#include "pch.h"
#include "WallpaperChanger.h"

using namespace WallpaperChangeBackgroundTask;
using namespace Platform;

using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::System::UserProfile;
using namespace Windows::Foundation;



WallpaperChanger::WallpaperChanger() :
	CancelReason(BackgroundTaskCancellationReason::Abort), CancelRequested(false), TaskDeferral(nullptr), TaskInstance(nullptr)
{}

WallpaperChanger::~WallpaperChanger()
{}

void WallpaperChanger::Run(IBackgroundTaskInstance^ taskInstance)
{
	// simple example with a Toast, to enable this go to manifest file
	// and mark App as TastCapable - it won't work without this
	// The Task will start but there will be no Toast.
	ToastTemplateType toastTemplate = ToastTemplateType.ToastText02;
	XmlDocument toastXml = ToastNotificationManager.GetTemplateContent(toastTemplate);
	XmlNodeList textElements = toastXml.GetElementsByTagName("text");
	textElements[0].AppendChild(toastXml.CreateTextNode("My first Task - Yeah"));
	textElements[1].AppendChild(toastXml.CreateTextNode("I'm a message from your background task!"));
	ToastNotificationManager.CreateToastNotifier().Show(new ToastNotification(toastXml));



	/*
	//
	// Associate a cancellation handler with the background task.
	//
	taskInstance->Canceled += ref new BackgroundTaskCanceledEventHandler(this, &WallpaperChanger::OnCanceled);

	//
	// Get the deferral object from the task instance, and take a reference to the taskInstance.
	//
	TaskDeferral = taskInstance->GetDeferral();
	TaskInstance = taskInstance;

	String^ timeStr = getUTCSystemTimeStr();

	//get image
	Uri^ uri = ref new Uri("http://www.andreaswittmann.com/wallpaperchanger/wallpapers/" + timeStr);
	taskInstance->Progress = 10;
	// download image from uri into temp storagefile
	create_task(StorageFile::CreateStreamedFileFromUriAsync("image.JPG", uri, nullptr)).then([this, taskInstance](concurrency::task<StorageFile^> fileTask)
	{
		StorageFile^ image;

		try
		{
			image = fileTask.get();
			auto directory = ApplicationData::Current->LocalFolder;

			create_task(image->CopyAsync(ApplicationData::Current->LocalFolder, "image.JPG", NameCollisionOption::ReplaceExisting)).then([this, taskInstance](task<StorageFile^> fileTask)
			{
				StorageFile^ image;
				try
				{
					image = fileTask.get();
					UserProfilePersonalizationSettings^ ups = UserProfilePersonalizationSettings::Current;

					create_task(ups->TrySetWallpaperImageAsync(image)).then([this, taskInstance](task<Boolean> boolTask) {
						if (!boolTask.get())
						{
							//TODO logging unsuccessful
						}
						//
						// Indicate that the background task has completed.
						//
						taskInstance->Progress = 100;
						TaskDeferral->Complete();
					});
				}
				catch (Platform::COMException^ exception)
				{
					image = nullptr;
					//TODO logging
				}
			});
		} catch (Platform::COMException^ exception)
		{
			image = nullptr;
			//TODO logging
		}
	});
	*/
}


void WallpaperChangeBackgroundTask::WallpaperChanger::OnCanceled(IBackgroundTaskInstance ^ taskInstance, BackgroundTaskCancellationReason reason)
{
	//
	// Indicate that the background task is canceled.
	//
	CancelRequested = true;
	CancelReason = reason;
}

String^ WallpaperChanger::getUTCSystemTimeStr()
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	//systemtime to UTC formatted Platform::String
	char buffer[256];
	sprintf_s(buffer,
		"%d-%02d-%02d",
		st.wYear,
		st.wMonth,
		st.wDay);

	size_t newsize = strlen(buffer) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, buffer, _TRUNCATE);
	String^ timeStr = ref new Platform::String(wcstring);
	delete[] wcstring;

	return timeStr;
}
