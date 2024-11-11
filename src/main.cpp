#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include <windows.h>
#include <vector>
#include <iomanip>

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void displayBanner() {
    setColor(14); // Yellow
    std::cout << R"(
    ╔═══════════════════════════════════════════╗
    ║           DEADLY NUMBER ROLLER            ║
    ║         Test Your Luck or Die!            ║
    ╚═══════════════════════════════════════════╝
    )" << std::endl;
    setColor(7);
}

void displayHistory(const std::vector<int>& history) {
    setColor(8); // Gray
    std::cout << "\n╔══════════════════╗\n";
    std::cout << "║ Previous Rolls:  ║\n";
    std::cout << "╠══════════════════╣\n";
    
    int start = std::max(0, static_cast<int>(history.size()) - 5);
    for (int i = start; i < history.size(); i++) {
        std::cout << "║ " << std::setw(3) << history[i] 
                 << std::string(11, ' ') << "║\n";
    }
    
    for (int i = 0; i < 5 - std::min(5, static_cast<int>(history.size())); i++) {
        std::cout << "║ ---" << std::string(11, ' ') << "║\n";
    }
    
    std::cout << "╚══════════════════╝\n";
    setColor(7);
}

void displayRollingAnimation(int maxVal) {
    const char spinner[] = {'|', '/', '-', '\\'};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> visualRoll(0, maxVal);
    
    for(int i = 0; i < 20; i++) {
        setColor(13); // Magenta
        std::cout << "\r" << "Rolling: " << std::setw(5) << visualRoll(gen) 
                 << " " << spinner[i % 4] << "   " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::vector<int> rollHistory;
    
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        int maxNumber;
        
        clearScreen();
        displayBanner();
        
        do {
            setColor(11);
            std::cout << "\n[?] Enter a number above 100: ";
            setColor(7);
            if (!(std::cin >> maxNumber)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                setColor(12);
                std::cout << "[!] Invalid input. Please enter a number.\n";
                setColor(7);
                continue;
            }
            if (maxNumber <= 100) {
                setColor(12);
                std::cout << "[!] Number must be above 100!\n";
                setColor(7);
            }
        } while (maxNumber <= 100);

        clearScreen();
        displayBanner();
        setColor(14);
        std::cout << "\n[!] Generating your starting number...\n";
        displayRollingAnimation(maxNumber);
        
        std::uniform_int_distribution<> firstRoll(0, maxNumber);
        int currentMax = firstRoll(gen);
        rollHistory.push_back(currentMax);
        
        setColor(10);
        std::cout << "\r[+] Your starting number is: " << currentMax << "!" << std::string(20, ' ') << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string rollAgain;
        while (currentMax > 0) {
            clearScreen();
            displayBanner();
            displayHistory(rollHistory);
            
            setColor(14);
            std::cout << "\n╔════════════════════════╗\n";
            std::cout << "║ Current number: " << std::string(8-std::to_string(currentMax).length(), ' ') 
                     << currentMax << " ║\n";
            std::cout << "╚════════════════════════╝\n";
            
            setColor(11);
            std::cout << "\n[>] Type 'roll' to continue: ";
            setColor(7);
            std::getline(std::cin >> std::ws, rollAgain);

            if (rollAgain == "roll") {
                displayRollingAnimation(currentMax);

                std::uniform_int_distribution<> dist(0, currentMax);
                currentMax = dist(gen);
                rollHistory.push_back(currentMax);
                
                setColor(10);
                std::cout << "\r[+] You rolled: " << currentMax << "!" << std::string(20, ' ') << std::endl;

                if (currentMax == 0) {
                    clearScreen();
                    displayBanner();
                    displayHistory(rollHistory);
                    setColor(12);
                    std::cout << R"(
    ╔═══════════════════════════════════════════╗
    ║                GAME OVER                  ║
    ║               YOU DIED! ☠                 ║
    ╚═══════════════════════════════════════════╝
    )" << std::endl;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            } else {
                setColor(14);
                std::cout << "\n[!] Game ended by user.\n";
                break;
            }
        }
        
        setColor(11);
        std::cout << "\nPress Enter to exit...";
        setColor(7);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

    } catch (const std::exception& e) {
        setColor(12);
        std::cerr << "[ERROR] " << e.what() << std::endl;
        setColor(7);
        return 1;
    }

    return 0;
}