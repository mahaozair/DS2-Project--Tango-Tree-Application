#include <string>
#ifdef _WIN32
#include <windows.h>

void OpenLinkWithoutConsole(const std::string &url)
{
    // This opens the URL using the default web browser
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
#elif __APPLE__
#include <cstdlib>

void OpenLinkWithoutConsole(const std::string &url)
{
    std::string command = "open " + url;
    system(command.c_str());
}
#else
#include <cstdlib>

void OpenLinkWithoutConsole(const std::string &url)
{
    std::string command = "xdg-open " + url;
    system(command.c_str());
}
#endif
#define STB_TEXT_HAS_SELECTION
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "FinalTangi.hpp"

// GUI state variables
static int selectedIndustry = 0;
static int selectedLocation = 0;
static int maxExperience = 0;
static int maxWeeklyHours = 40;

static bool hasRemotePref = false;
static bool hasPaidPref = false;
static bool hasHandsOnPref = false;
static bool hasIndustryPref = false;
static bool hasLocationPref = false;

static bool preferRemote = false;
static bool requirePaid = false;
static bool preferHandsOn = false;

static int resultsCount = 10;

static std::vector<Internship *> currentResults;
static std::vector<std::string> consoleLog;
static bool showDebugWindow = false;

static std::vector<std::string> allIndustries;
static std::vector<std::string> allLocations;

// helper

void cleanupInternships(const std::vector<Internship *> &internships)
{
    for (Internship *intern : internships)
    {
        delete intern;
    }
}

void AddLog(const std::string &message)
{
    consoleLog.push_back(message);
    if (consoleLog.size() > 100)
        consoleLog.erase(consoleLog.begin());
}

// Function to read internships from a CSV file (Adapted for GUI)
std::vector<Internship *> readInternshipsFromCSV(const std::string &filename,
                                                 std::vector<std::string> &allIndustries,
                                                 std::vector<std::string> &allLocations)
{
    std::vector<Internship *> internships;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return internships;
    }

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string title, firm, location, industry;
        std::string remoteStr, paidStr, yearsExpStr, handsOnStr, weeklyHrsStr, description, link;

        std::getline(ss, title, ',');
        std::getline(ss, firm, ',');
        std::getline(ss, location, ',');
        std::getline(ss, remoteStr, ',');
        std::getline(ss, paidStr, ',');
        std::getline(ss, industry, ',');
        std::getline(ss, yearsExpStr, ',');
        std::getline(ss, handsOnStr, ',');
        std::getline(ss, weeklyHrsStr, ',');
        std::getline(ss, description, ',');
        std::getline(ss, link, ',');

        bool remote = (remoteStr == "1" || remoteStr == "true");
        bool paid = (paidStr == "1" || paidStr == "true");
        int yearsExp = std::stoi(yearsExpStr);
        bool handsOn = (handsOnStr == "1" || handsOnStr == "true");
        int weeklyHrs = std::stoi(weeklyHrsStr);

        Internship *intern = new Internship(title, firm, location, remote, paid, industry,
                                            yearsExp, handsOn, weeklyHrs,
                                            allIndustries, allLocations, description, link);
        internships.push_back(intern);
    }

    file.close();
    return internships;
}

void ShowPreferencesWindow(TangoTree &tree)
{
    ImGui::Begin("Search Preferences");

    // Industry preference
    ImGui::Checkbox("Has Industry Preference", &hasIndustryPref);
    std::vector<const char *> industries;
    for (const auto &ind : allIndustries)
        industries.push_back(ind.c_str());
    industries.push_back("any");
    if (hasIndustryPref)
    {
        ImGui::Combo("Industry", &selectedIndustry, industries.data(), static_cast<int>(industries.size()));
    }

    // Location preference
    ImGui::Checkbox("Has Location Preference", &hasLocationPref);
    std::vector<const char *> locations;
    for (const auto &loc : allLocations)
        locations.push_back(loc.c_str());
    locations.push_back("any");
    if (hasLocationPref)
    {
        ImGui::Combo("Location", &selectedLocation, locations.data(), static_cast<int>(locations.size()));
    }

    // Preferences
    ImGui::Checkbox("Has Remote Preference", &hasRemotePref);
    if (hasRemotePref)
        ImGui::Checkbox("Prefer Remote", &preferRemote);

    ImGui::Checkbox("Has Paid Preference", &hasPaidPref);
    if (hasPaidPref)
        ImGui::Checkbox("Prefer Paid", &requirePaid);

    ImGui::Checkbox("Has Hands-On Preference", &hasHandsOnPref);
    if (hasHandsOnPref)
        ImGui::Checkbox("Prefer Hands-On", &preferHandsOn);

    // Experience & hours
    ImGui::SliderInt("Max Experience (years)", &maxExperience, 0, 4);
    ImGui::SliderInt("Max Weekly Hours", &maxWeeklyHours, 10, 40, "%d hours");

    // Number of results to show
    ImGui::InputInt("Number of Results", &resultsCount);
    resultsCount = std::clamp(resultsCount, 1, 100);

    if (ImGui::Button("Search"))
    {
        currentResults.clear();

        std::string selectedInd = (hasIndustryPref && selectedIndustry < allIndustries.size()) ? allIndustries[selectedIndustry] : "any";
        std::string selectedLoc = (hasLocationPref && selectedLocation < allLocations.size()) ? allLocations[selectedLocation] : "any";

        UserPreferences userPref(
            preferRemote,
            requirePaid,
            selectedInd,
            maxExperience,
            preferHandsOn,
            maxWeeklyHours,
            allIndustries,
            allLocations,
            selectedLoc,
            hasRemotePref,
            hasPaidPref,
            hasIndustryPref,
            hasLocationPref,
            hasHandsOnPref,
            resultsCount);

        std::vector<Internship *> searchResults;
        for (const auto &category : userPref.CategoryId)
        {
            auto matches = tree.search(category);
            for (const auto &match : matches)
            {
                if (match->weeklyHours <= userPref.maxWeeklyHours &&
                    match->yearsExperience <= userPref.maxExperience)
                {
                    searchResults.push_back(match);
                }
            }
        }

        currentResults = searchResults;
        AddLog("Search completed. Found " + std::to_string(currentResults.size()) + " results.");
    }

    ImGui::End();
}

void ShowResultsWindow()
{
    ImGui::Begin("Search Results");

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    if (ImGui::BeginTable("ResultsTable", 3, flags))
    {
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Location");
        ImGui::TableSetupColumn("Industry");
        ImGui::TableHeadersRow();

        for (const auto &intern : currentResults)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", intern->title.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", allLocations[intern->location].c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", allIndustries[intern->industry].c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void ShowDetailsWindow()
{
    ImGui::Begin("Internship Details");

    // static int selected = -1;
    // for (int n = 0; n < currentResults.size(); n++)
    // {
    //     if (ImGui::Selectable(currentResults[n]->title.c_str(), selected == n))
    //     {
    //         selected = n;
    //     }
    // }

    // if (selected >= 0 && selected < currentResults.size())
    // {
    //     const auto &intern = currentResults[selected];
    //     ImGui::Separator();
    //     ImGui::Text("Title: %s", intern->title.c_str());
    //     ImGui::Text("Description: %s", intern->description.c_str());
    //     ImGui::Text("Link: %s", intern->link.c_str());
    //     ImGui::Text("Location: %s", allLocations[intern->location].c_str());
    //     ImGui::Text("Industry: %s", allIndustries[intern->industry].c_str());
    //     ImGui::Text("Remote: %s", intern->remote ? "Yes" : "No");
    //     ImGui::Text("Paid: %s", intern->paid ? "Yes" : "No");
    //     ImGui::Text("Experience Required: %d years", intern->yearsExperience);
    //     ImGui::Text("Hands-on: %s", intern->handsOn ? "Yes" : "No");
    //     ImGui::Text("Weekly Hours: %d", intern->weeklyHours * 10);
    // }
    for (int i = 0; i < currentResults.size(); ++i)
    {
        const auto &intern = currentResults[i];
        std::string heading = std::to_string(i + 1) + ". " + intern->title;
        ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.2f, 1.0f), "%s", heading.c_str()); // Highlighted title
        ImGui::Separator();

        ImGui::TextWrapped("Description: %s", intern->description.c_str());
        ImGui::TextWrapped("Firm: %s", intern->firm.c_str());
        ImGui::Text("Location: %s", allLocations[intern->location].c_str());
        ImGui::Text("Industry: %s", allIndustries[intern->industry].c_str());
        ImGui::Text("Remote: %s", intern->remote ? "Yes" : "No");
        ImGui::Text("Paid: %s", intern->paid ? "Yes" : "No");
        ImGui::Text("Experience Required: %d years", intern->yearsExperience);
        ImGui::Text("Hands-on: %s", intern->handsOn ? "Yes" : "No");
        ImGui::Text("Weekly Hours: %d", intern->weeklyHours);
        // ImGui::Text("Link: %s", intern->link.c_str());

        // Apply button
        // std::string buttonLabel = "Apply##" + std::to_string(i); 
        // if (ImGui::Button(buttonLabel.c_str()))
        // {
// #ifdef _WIN32
//             std::string command = "start " + intern->link;
// #elif __APPLE__
//             std::string command = "open " + intern->link;
// #else
//             std::string command = "xdg-open " + intern->link;
// #endif
//             system(command.c_str());
//         }
        std::string buttonLabel = "Apply##" + std::to_string(i);
        if (ImGui::Button(buttonLabel.c_str()))
        {
            // Open the link without a console window
            OpenLinkWithoutConsole(intern->link);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
    }

    ImGui::End();
}
// void ShowDetailsWindow()
// {
//     ImGui::Begin("Internship Details");

//     for (int i = 0; i < currentResults.size(); ++i)
//     {
//         const auto &intern = currentResults[i];
//         std::string heading = std::to_string(i + 1) + ". " + intern->title;
//         ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.2f, 1.0f), "%s", heading.c_str()); // Highlighted title
//         ImGui::Separator();

//         ImGui::TextWrapped("Description: %s", intern->description.c_str());
//         ImGui::Text("Location: %s", allLocations[intern->location].c_str());
//         ImGui::Text("Industry: %s", allIndustries[intern->industry].c_str());
//         ImGui::Text("Remote: %s", intern->remote ? "Yes" : "No");
//         ImGui::Text("Paid: %s", intern->paid ? "Yes" : "No");
//         ImGui::Text("Experience Required: %d years", intern->yearsExperience);
//         ImGui::Text("Hands-on: %s", intern->handsOn ? "Yes" : "No");
//         ImGui::Text("Weekly Hours: %d", intern->weeklyHours);
//         ImGui::Text("Link: %s", intern->link.c_str());

//         // Apply button
//         std::string buttonLabel = "Apply" + std::to_string(i); // Unique ID for each button
//         if (ImGui::Button(buttonLabel.c_str()))
//         {
// #ifdef _WIN32
//             std::string command = "start " + intern->link;
// #elif __APPLE__
//             std::string command = "open " + intern->link;
// #else
//             std::string command = "xdg-open " + intern->link;
// #endif
//             system(command.c_str());
//         }

//         ImGui::Spacing();
//         ImGui::Separator();
//         ImGui::Spacing();
//     }

//     ImGui::End();
// }


int main()
{
    std::cout << "entered the main" << std::endl;
    // Initialize GLFW
    if (!glfwInit()){
        std::cout << "coult not do initialising GLFW" << std::endl;
        return 1;
    }
    // Create window
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Internship Browser", NULL, NULL);
    if (!window)
    {
        std::cout << "no window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Initialize Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Load data
    std::string csvFilename = "internships_test.csv";
    std::vector<Internship *> internships = readInternshipsFromCSV(csvFilename, allIndustries, allLocations);

    // Check if internships were loaded successfully
    if (internships.empty())
    {
        std::cerr << "No internships loaded from CSV file." << std::endl;
        return 1;
    }

    // Initialize the TangoTree
    TangoTree tree;
    for (Internship *intern : internships)
    {
        tree.insertInternship(intern);
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        std::cout<<"entering main loop"<<std::endl;
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show main windows
        ShowPreferencesWindow(tree);
        // ShowResultsWindow();
        ShowDetailsWindow();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    cleanupInternships(internships);
    return 0;
}
