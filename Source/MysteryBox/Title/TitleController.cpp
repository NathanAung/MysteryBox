// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleController.h"
#include "Kismet/GameplayStatics.h"
#include "string"
#include "ImGuiModule.h"
#include "../Fonts/FONT_Tron.h"

// Sets default values
ATitleController::ATitleController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATitleController::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	ImGuiIO &io = ImGui::GetIO();

	if (io.Fonts->Fonts.Size == 1)
	{
		if (TSharedPtr<ImFontConfig> FAFontConfig = MakeShareable(new ImFontConfig()))
		{

			FAFontConfig->FontDataOwnedByAtlas = false;		 // Global font data lifetime
			FAFontConfig->FontData = (void *)FONT_Tron_data; // Declared in binary C .h file
			FAFontConfig->FontDataSize = FONT_Tron_size;	 // Declared in binary C .h file
			FAFontConfig->SizePixels = 18;
			FAFontConfig->MergeMode = false;	   // Forces ImGui to place this font into the same atlas as the previous font
			FAFontConfig->GlyphMinAdvanceX = 18.f; // Use for monospaced icons
			FAFontConfig->PixelSnapH = true;	   // Better rendering (align to pixel grid)

			FImGuiModule::Get().GetProperties().AddCustomFont("TRON", FAFontConfig);
			FImGuiModule::Get().RebuildFontAtlas();
		}
	}

	FImGuiModule::Get().SetInputMode(true);

	UE_LOG(LogTemp, Warning, TEXT("Fonts: %d"), io.Fonts->Fonts.Size);

	CustomFont = io.Fonts->Fonts.back();
}

// Called every frame
void ATitleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShowTitleMenu();
}

void ATitleController::ShowTitleMenu()
{
	ImGuiIO &io = ImGui::GetIO();
	// Calculate center of the screen
	ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

	// Make the window large enough to hold a big title and spacious buttons
	ImVec2 menuSize = ImVec2(500.0f, 350.0f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);

	// Added NoTitleBar and NoBackground to make it a clean, floating overlay
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground;

	// --- STYLING: The "Mystery Box" Palette ---
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
	// Deep mysterious purple for buttons
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.15f, 0.35f, 0.8f));
	// Bright neon violet glow on hover
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.25f, 0.65f, 1.0f));
	// Darker tone when clicked
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.05f, 0.25f, 1.0f));

	ImGui::PushFont(CustomFont);

	// Padding and rounding to make buttons look modern and sleek
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 15.0f)); // Elegant spacing between elements

	ImGui::Begin("MysteryBoxMainMenu", nullptr, flags);

	float windowWidth = ImGui::GetWindowSize().x;
	float buttonWidth = 250.0f;
	float buttonHeight = 45.0f; // Taller buttons look way more professional

	// --- GAME TITLE ---
	ImGui::SetWindowFontScale(2.5f); // Make the title huge
	std::string titleText = "MYSTERY BOX";
	float titleWidth = ImGui::CalcTextSize(titleText.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
	// Colored text for the title (a soft, mysterious gold/yellow)
	ImGui::TextColored(ImVec4(0.9f, 0.75f, 0.4f, 1.0f), "%s", titleText.c_str());

	// Reset font scale down for the buttons
	ImGui::SetWindowFontScale(1.2f);
	ImGui::Spacing();
	ImGui::Spacing(); // Give the title some breathing room

	// --- START BUTTON ---
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("START GAME", ImVec2(buttonWidth, buttonHeight)))
	{
		FImGuiModule::Get().SetInputMode(false);
		UGameplayStatics::OpenLevel(GetWorld(), "FirstLevel");
	}

	// --- CREDITS BUTTON ---
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("CREDITS", ImVec2(buttonWidth, buttonHeight)))
	{
		// credits here
	}

	// --- QUIT BUTTON ---
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("QUIT", ImVec2(buttonWidth, buttonHeight)))
	{
		if (PC)
			PC->ConsoleCommand("quit");
	}

	ImGui::End();

	// Clean up styles
	ImGui::PopFont();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(4);
}
