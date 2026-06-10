#include "UIController.h"

#include "imgui.h"
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
}

// Called every frame
void AUIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Player1UI();
    Player2UI();

    if(gameOver)
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
    ImGui::SetNextWindowSize(ImVec2(200.0f, 90.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

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

    ImGui::End();

    ImGui::PopStyleColor(1);
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

    ImGui::End();

    ImGui::PopStyleColor(1);
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

    ImGui::PopStyleColor(1);
}


void AUIController::UpdatePlayerScore(int32 playerId){
    playerId != 1 ? P2Score++ : P1Score++;
}

void AUIController::CallWinScreen(int32 playerId){
    snprintf(winningText, sizeof(winningText), "PLAYER %d WINS", playerId);
    gameOver = true;
}