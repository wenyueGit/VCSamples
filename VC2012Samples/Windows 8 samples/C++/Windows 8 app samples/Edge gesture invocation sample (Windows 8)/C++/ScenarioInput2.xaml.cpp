﻿// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

//
// ScenarioInput2.xaml.cpp
// Implementation of the ScenarioInput2 class
//

#include "pch.h"
#include "ScenarioInput2.xaml.h"

using namespace EdgeGestureSample;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

ScenarioInput2::ScenarioInput2()
{
    InitializeComponent();
}

ScenarioInput2::~ScenarioInput2()
{
}

#pragma region Template-Related Code - Do not remove
void ScenarioInput2::OnNavigatedTo(NavigationEventArgs^ e)
{
    // Get a pointer to our main page.
    rootPage = dynamic_cast<MainPage^>(e->Parameter);

    // We want to be notified when the OutputFrame is loaded so we can get to the content.
    _frameLoadedToken = rootPage->OutputFrameLoaded += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &ScenarioInput2::rootPage_OutputFrameLoaded);
}

void ScenarioInput2::OnNavigatedFrom(NavigationEventArgs^ e)
{
    RightClickRegion->RightTapped -= RightClickRegionEventToken;
    rootPage->RightTapped -= RightClickEventToken;
    EdgeGesture^ edgeGesture = Windows::UI::Input::EdgeGesture::GetForCurrentView();
    edgeGesture->Starting -= EdgeGestureStartingEventToken;
    edgeGesture->Completed -= EdgeGestureCompletedEventToken;
    edgeGesture->Canceled -= EdgeGestureCanceledEventToken;

    rootPage->OutputFrameLoaded -= _frameLoadedToken;
}

#pragma endregion

#pragma region Use this code if you need access to elements in the output frame - otherwise delete
void ScenarioInput2::rootPage_OutputFrameLoaded(Object^ sender, Object^ e)
{
    // Get a pointer to the content within the OutputFrame.
    Page^ outputFrame = dynamic_cast<Page^>(rootPage->OutputFrame->Content);

    outputText = dynamic_cast<TextBlock^>(outputFrame->FindName("OutputText"));
    RightClickRegion = dynamic_cast<ListBox^>(outputFrame->FindName("RightClickRegion"));

    InitializeEdgeGestureHandlers();
}

#pragma endregion

#pragma region Event handlers

void ScenarioInput2::OnStarting(EdgeGesture^ sender, EdgeGestureEventArgs^ e)
{
    outputText->Text = "Invoking with touch.";
}

void ScenarioInput2::OnCompleted(EdgeGesture^ sender, EdgeGestureEventArgs^ e)
{
    // Determine whether it was touch or keyboard invocation
    if (e->Kind == Windows::UI::Input::EdgeGestureKind::Touch)
    {
        outputText->Text = "Invoked with touch.";
    }
    else if (e->Kind == Windows::UI::Input::EdgeGestureKind::Keyboard)
    {
        outputText->Text = "Invoked with keyboard.";
    }
    else if (e->Kind == Windows::UI::Input::EdgeGestureKind::Mouse)
    {
        outputText->Text = "Invoked with right-click.";
    }
}

void ScenarioInput2::OnCanceled(EdgeGesture^ sender, EdgeGestureEventArgs^ e)
{
    outputText->Text = "Canceled with touch.";
}

void ScenarioInput2::OnContextMenu(Object^ sender, RightTappedRoutedEventArgs^ e)
{
    outputText->Text = "Invoked with right-click.";
}

void ScenarioInput2::RightClickOverride(Object^ sender, RightTappedRoutedEventArgs^ e)
{
    outputText->Text = "The ContextMenu event was handled.  The EdgeGesture event will not fire.";
    e->Handled = true;
}

void ScenarioInput2::InitializeEdgeGestureHandlers()
{
    EdgeGesture^ edgeGesture = Windows::UI::Input::EdgeGesture::GetForCurrentView();

    EdgeGestureStartingEventToken = edgeGesture->Starting += ref new TypedEventHandler<EdgeGesture^, EdgeGestureEventArgs^>(this, &ScenarioInput2::OnStarting);
    EdgeGestureCompletedEventToken = edgeGesture->Completed += ref new TypedEventHandler<EdgeGesture^, EdgeGestureEventArgs^>(this, &ScenarioInput2::OnCompleted);
    EdgeGestureCanceledEventToken = edgeGesture->Canceled += ref new TypedEventHandler<EdgeGesture^, EdgeGestureEventArgs^>(this, &ScenarioInput2::OnCanceled);

    RightClickEventToken = rootPage->RightTapped += ref new RightTappedEventHandler(this, &ScenarioInput2::OnContextMenu);
    RightClickRegionEventToken = RightClickRegion->RightTapped += ref new RightTappedEventHandler(this, &ScenarioInput2::RightClickOverride);

    outputText->Text = "Sample initialized and events registered.";
}

#pragma endregion
