﻿#include "CREditorUI.h"
#include "Engine.h"
#include "Extras/ImGUI/imgui.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CREditorUI::Initialize()
{
    CameraUI.SetCamera( GCamera );
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CREditorUI::Draw()
{
    ImGui::Begin( "Camera UI" );
        CameraUI.Draw();
    ImGui::End();

    ImGui::Begin( "Logs" );
        LogUI.Draw();
    ImGui::End();
}
