#include "UIController.h"
#include "ImGuiModule.h"

// Sets default values
AUIController::AUIController()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    this->Tags.AddUnique(TEXT("UI_CONTROLLER"));
}

// Called when the game starts or when spawned
void AUIController::BeginPlay()
{
    Super::BeginPlay();

    PC = GetWorld()->GetFirstPlayerController();

    ImGuiIO &io = ImGui::GetIO();
    CustomFont = io.Fonts->Fonts.back();
}

// Called every frame
void AUIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Player1UI();
    Player2UI();

    if (P1StunTime > 0.f)
    {
        P1StunTime -= DeltaTime;
    }
    if (P2StunTime > 0.f)
    {
        P2StunTime -= DeltaTime;
    }
    if (P1SpeedTime > 0.f)
    {
        P1SpeedTime -= DeltaTime;
    }
    if (P2SpeedTime > 0.f)
    {
        P2SpeedTime -= DeltaTime;
    }

    if (gameOver)
        WinningUI();
}

void AUIController::Player1UI()
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d / 3", P1Score);

    // Using a stack-allocated char array for the static label
    char headerText[] = "PLAYER 1";

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.05f, io.DisplaySize.y * 0.05f);

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(200.0f, 180.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushFont(CustomFont);

    ImGui::Begin("PLAYER 1 SCORE", nullptr, flags);

    float windowWidth = ImGui::GetWindowSize().x;

    // Header label: "PLAYER 1"
    ImGui::SetWindowFontScale(1.2f);
    float headerWidth = ImGui::CalcTextSize(headerText).x;
    ImGui::SetCursorPosX((windowWidth - headerWidth) * 0.5f);
    ImGui::Text("%s", headerText);

    // Score display (Big, prominent gold text)
    ImGui::SetWindowFontScale(2.0f);
    float scoreWidth = ImGui::CalcTextSize(buffer).x;
    ImGui::SetCursorPosX((windowWidth - scoreWidth) * 0.5f);
    ImGui::TextColored(ImVec4(0.9f, 0.75f, 0.4f, 1.0f), "%s", buffer);

    if (P1StunTime > 0.f)
    {
        float stunProgress = P1StunTime / 2.f;

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.85f, 0.15f, 0.15f, 1.0f));

        ImGui::TextColored(ImVec4(0.85f, 0.15f, 0.15f, 1.0f), "!");
        ImGui::SameLine();
        ImGui::ProgressBar(stunProgress, ImVec2(-1.0f, 0.0f), "");
        ImGui::PopStyleColor(1);
    }

    if (P1SpeedTime > 0.f)
    {
        float speedProgress = P1SpeedTime / 3.f;

        ImVec4 speedColor = P1SpeedUp ? ImVec4(0.1f, 0.6f, 1.0f, 1.0f) : ImVec4(0.5f, 0.2f, 0.8f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, speedColor);

        ImGui::TextColored(speedColor, P1SpeedUp ? ">>" : "<<");
        ImGui::SameLine();
        ImGui::ProgressBar(speedProgress, ImVec2(-1.0f, 0.0f), "");
        ImGui::PopStyleColor(1);
    }

    ImGui::End();

    ImGui::PopStyleColor(2);
    ImGui::PopFont();
}

void AUIController::Player2UI()
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d / 3", P2Score);

    char headerText[] = "PLAYER 2";

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.95f, io.DisplaySize.y * 0.05f);

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(200.0f, 90.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushFont(CustomFont);

    ImGui::Begin("PLAYER 2 SCORE", nullptr, flags);

    float windowWidth = ImGui::GetWindowSize().x;

    // Header label: "PLAYER 2"
    ImGui::SetWindowFontScale(1.2f);
    float headerWidth = ImGui::CalcTextSize(headerText).x;
    ImGui::SetCursorPosX((windowWidth - headerWidth) * 0.5f);
    ImGui::Text("%s", headerText);

    // Score display (Big, prominent gold text)
    ImGui::SetWindowFontScale(2.0f);
    float scoreWidth = ImGui::CalcTextSize(buffer).x;
    ImGui::SetCursorPosX((windowWidth - scoreWidth) * 0.5f);
    ImGui::TextColored(ImVec4(0.9f, 0.75f, 0.4f, 1.0f), "%s", buffer);

    if (P2StunTime > 0.f)
    {
        float stunProgress = P2StunTime / 2.f;

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.1f, 0.8f, 0.3f, 1.0f));
        ImGui::ProgressBar(stunProgress, ImVec2(-1.0f, 0.0f), "");
        ImGui::PopStyleColor(1);
    }

    if (P2SpeedTime > 0.f)
    {
        float speedProgress = P2SpeedTime / 3.f;

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.1f, 0.8f, 0.3f, 1.0f));
        ImGui::ProgressBar(speedProgress, ImVec2(-1.0f, 0.0f), ">>");
        ImGui::PopStyleColor(1);
    }

    ImGui::End();

    ImGui::PopStyleColor(2);
    ImGui::PopFont();
}

void AUIController::WinningUI()
{
    char headerText[] = "GAME OVER";

    ImGuiIO &io = ImGui::GetIO();
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.50f, io.DisplaySize.y * 0.45f);

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
    ImGui::PushFont(CustomFont);

    ImGui::Begin("GAME OVER", nullptr, flags);

    float windowWidth = ImGui::GetWindowSize().x;

    // --- GAME OVER HEADER ---
    ImGui::SetWindowFontScale(2.5f);
    float headerWidth = ImGui::CalcTextSize(headerText).x;
    ImGui::SetCursorPosX((windowWidth - headerWidth) * 0.5f);
    ImGui::Text("%s", headerText);

    ImGui::Spacing();
    ImGui::Spacing();

    // --- WINNING TEXT ---
    ImGui::SetWindowFontScale(1.5f);
    // Assumes winningText is a native char[] or const char* from your class setup
    float textWidth = ImGui::CalcTextSize(winningText).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextColored(ImVec4(0.9f, 0.75f, 0.4f, 1.0f), "%s", winningText);

    ImGui::End();
    ImGui::PopFont();
    ImGui::PopStyleColor(1);
}

void AUIController::UpdatePlayerScore(int32 playerId)
{
    playerId != 1 ? P2Score++ : P1Score++;
}

void AUIController::CallWinScreen(int32 playerId)
{
    snprintf(winningText, sizeof(winningText), "PLAYER %d WINS", playerId);
    gameOver = true;
}

void AUIController::StunPlayerUI(int32 playerId)
{
    playerId != 1 ? P2StunTime = 2.f : P1StunTime = 2.f;
}

void AUIController::SpeedBuffUI(int32 playerId, bool bonus)
{
    if (playerId != 1)
    {
        P2StunTime = 3.f;
        P2SpeedUp = bonus;
    }
    else
    {
        P1StunTime = 3.f;
        P1SpeedUp = bonus;
    }
}