#pragma once
#include "pch.h"
#include <agile.h>

using namespace Windows::ApplicationModel::Background;
using namespace Windows::System::Threading;

namespace WallpaperChangeBackgroundTask
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class WallpaperChanger sealed : public IBackgroundTask
    {
    public:
		WallpaperChanger();

		virtual void Run(IBackgroundTaskInstance^ taskInstance);
		void OnCanceled(IBackgroundTaskInstance^ taskInstance, BackgroundTaskCancellationReason reason);

	private:
		~WallpaperChanger();

		BackgroundTaskCancellationReason CancelReason;
		volatile bool CancelRequested;
		Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral> TaskDeferral;
		ThreadPoolTimer^ PeriodicTimer;
		unsigned int Progress;
		IBackgroundTaskInstance^ TaskInstance;

		Platform::String^ getUTCSystemTimeStr();
    };
}
