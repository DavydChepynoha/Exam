
#include <iostream>
#include <fstream>

struct City {
    std::string name;
    int price; // В гривнах
};

struct Ticket {
    std::string passengerName;
    City from;
    City destination;
    int seatNumber;
};

struct Race {
    City from;
    City to;
    Ticket tickets[10]; // максимум 10 мест
    int ticketCount;
};

int main() {
    srand(time(0));

    const int size = 8;
    City cities[size] = {
        {"Київ", 1000},
        {"Дніпро", 400},
        {"Львів", 800},
        {"Одеса", 700},
        {"Вінниця", 500},
        {"Харків", 600},
        {"Ужгород", 900},
        {"Запоріжжя", 300}
    };

    City currentCity = {"Запоріжжя", 300};
    int money = 10000;
    
    Race races[7]; // Пока что пустые рейсы

    
    while (true) { // Здесь будет основной цикл программы
        int choice;
        std::cout << "Вітаємо на станції " << currentCity.name << "!\n";
        std::cout << "Ваші гроші: " << money << "\n";
        std::cout << "1 - Рейси\n2 - Скасування білету\n3 - Довідатися про місто\n4 - Поїхати по білету\nІнше - Вийти\n: ";
        std::cin >> choice;

        if (choice == 1) {
            // Тут будет выбор рейса
        }
        else if (choice == 2) {
            // Тут будет отмена билета
        }
        else if (choice == 3) {
            // Про город
        }
        else if (choice == 4) {
            // Отправляемся в город
        }
        else {
            break;
        }
    }

    return 0;
}
