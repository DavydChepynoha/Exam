#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

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
    int soldSeats = 0;
};

//ФУНКЦИЯ ОТВЕЧАЮЩЯЯ ЗА ПОКАЗ ВСЕЙ ИСТОРИИ ПРОГРАММЫ
void showHistory() {
    std::ifstream history("history.txt");
    if (!history) {
        std::cout << "Історія порожня.\n";
        return;
    }

    std::cout << "Історія квитків:\n";
    std::string line;
    while (std::getline(history, line)) {
        std::cout << line << std::endl;
    }
    history.close();
}

//ФУНКЦИЯ КОТОРАЯ ВЫВОДИТ ОПИСАНИЕ ПРО ГОРОД В КОТОРОМ МЫ НАХОДИМСЯ
void showDescription(City currentCity) {
    //БЛАГОДАРЮ CHAT GPT ЗА ЗГЕНЕРИРОВАНОЕ ОПИСАНИЕ К ГОРОДАМ
    std::cout << "Інформація про місто " << currentCity.name << ":" << std::endl;
    //И ВОТ ТАК ПОГНАЛИ ЧЕРЕЗ УСЛОВИЯ
    if (currentCity.name == "Київ") {
        std::cout << "Київ — столиця України, великий культурний та економічний центр." << std::endl;
    }
    else if (currentCity.name == "Львів") {
        std::cout << "Львів — місто з багатою історією та красивою архітектурою, відоме кавою." << std::endl;
    }
    else if (currentCity.name == "Одеса") {
        std::cout << "Одеса — портове місто на Чорному морі, відоме пляжами та Приморським бульваром." << std::endl;
    }
    else if (currentCity.name == "Дніпро") {
        std::cout << "Дніпро — важливий промисловий та транспортний центр на сході України." << std::endl;
    }
    else if (currentCity.name == "Запоріжжя") {
        std::cout << "Запоріжжя — місто з історією козацтва та відомою ГЕС на Дніпрі." << std::endl;
    }
    else { //НА ВСЯКИЙ ПОЖАРНЫЙ
        std::cout << "Про це місто немає інформації." << std::endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    srand(time(0));
    //ГОРОДА
    const int size = 5;
    City cities[size] = {
        {"Київ", 1000},
        {"Львів", 800},
        {"Одеса", 700},
        {"Дніпро", 400},
        {"Запоріжжя", 300}
    };

    City currentCity = cities[4]; // Запоріжжя
    int money = 5000;

    // ВСЕ РЕЙСЫ СОЗДАЮ СРАЗУ
    Race races[size][size];
    for (int from = 0; from < size; from++) {
        for (int to = 0; to < size; to++) {
            if (from != to) {
                races[from][to].from = cities[from];
                races[from][to].to = cities[to];
            }
        }
    }

    //РАНДОМНО ЗАПОЛНЯЮ МЕСТА
    for (int from = 0; from < size; from++) {
        for (int to = 0; to < size; to++) {
            if (from == to) continue;

            //ДЛЯ БОЛЕЕ ЛЕГКОГО ДОСТУПА (НЕ ПИСАТЬ МНОГО ТЕКСТА)
            Race& race = races[from][to];
            //ГЕНЕРАЦИЯ КОЛЛИЧЕСТВА ПАССАЖИРОВ
            int passengers = rand() % 9;

            //ВОТ ТУТ САМО ЗАПОЛНЕНИЕ РАНДОМНЫМИ ПАССАЖИРАМИ
            for (int i = 0; i < passengers; i++) {
                int seat = race.soldSeats + 1;
                race.tickets[race.soldSeats] = { "Пасажир", cities[from], cities[to], seat };
                race.soldSeats++;
            }
        }
    }

    while (true) {
        std::cout << "Вітаємо на станції м." << currentCity.name << std::endl;
        std::cout << "Ваші гроші: " << money << std::endl;
        std::cout << "1 - Рейси\n2 - Скасування\n3 - Історія\n4 - Поїхати по білету\n5 - Опис цього міста\nІнш - вийти\n: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            // ДОСТУПНЫЕ РЕЙСЫ
            int index = 0;

            // ТОЛЬКО РЕЙСЫ ИЗ НАШЕГО ГОРОДА (ЧТО БЫ ПОТОМ НЕ ПУТАТЬСЯ С ИНДЕКСАМИ КОТОРЫЕ МОГУТ НЕ СОВПАДАТЬ С ДРУГИМ МАССИВОМ)
            int citiesMAP[size];


            int fromIndex = 0;
            //ВОТ ТУТ Я НАХОЖУ ИНДЕКС НАШЕГО ГОРОДА В МАССИВЕ
            for (int i = 0; i < size; i++) {
                if (cities[i].name == currentCity.name) {
                    fromIndex = i;
                    break;
                }
            }

            //ВОТ ТУТ Я ВЫВОЖУ ГОРОДА КУДА МОЖНО ПОЕХАТЬ И ДОПОЛНИТЕЛЬНУЮ ИНФОРМАЦИЮ
            for (int i = 0; i < size; i++) {
                if (currentCity.name != cities[i].name) {
                    std::cout << index << ") " << currentCity.name << " - " << cities[i].name
                        << " Вільних місць: " << (10 - races[fromIndex][i].soldSeats) << "| " << races[fromIndex][i].to.price << " грн" << std::endl;
                    citiesMAP[index] = i;
                    index++;
                }
            }

            std::cout << "Виберіть рейс: ";
            int choice2;
            std::cin >> choice2;
            if (choice2 < 0 || choice2 >= index) {
                continue;
            }
            int toIndex = citiesMAP[choice2];

            //ВОТ ТУТ Я СОЗДАЛ ССЫЛКУ НА МАССИВ, НИЧЕГО НЕ МЕНЯЕТ, ПРОСТО ЧИЩЕ КОД
            Race& race = races[fromIndex][toIndex];
            int choice3;
            std::cout << "1 - Купити 2 - Подивитись список пасажирів ";
            std::cin >> choice3;
            //СПИСОК ПАССАЖИРОВ (ЧИСТО ВЫВОД МАССИВА)
            //КАКОЙ МАССИВ? :МЫ УЖЕ ЗНАЕМ ОТКУДА И КУДА (ИНДЕКСЫ) ПОТОМУ ЧТО ВЫБИРАЛИ РАНЕЕ
            if (choice3 == 2) {
                std::cout << "Пасажири на рейс " << currentCity.name << " - " << cities[toIndex].name << ":" << std::endl;
                if (race.soldSeats == 0) {
                    std::cout << "Немає пасажирів" << std::endl;
                }
                else {
                    for (int i = 0; i < race.soldSeats; i++) {
                        std::cout << race.tickets[i].seatNumber << " - " << race.tickets[i].passengerName << std::endl;
                    }
                }
            }
            //ПОКУПКА БИЛЕТОВ
            else if (choice3 == 1) {
                //СКИПАЕМ ЕСЛИ НЕТ МЕСТ
                if (race.soldSeats >= 10) {
                    std::cout << "Немає місць" << std::endl;
                    continue;
                }
                //СКИПАЕМ ЕСЛИ НЕТ ДЕНЕГ
                if (money < race.to.price) {
                    std::cout << "Недостатньо грошей" << std::endl;
                    continue;
                }

                std::cout << "Введіть своє ім'я: ";
                std::string name;
                std::cin >> name;
                //ВОТ ТУТ ВЫБОР МЕСТА, ОТ 1 ДО 10
                int seat = race.soldSeats + 1; //1 2 3 4 5 6 7 8 9 10
                race.tickets[race.soldSeats] = { name, currentCity, cities[toIndex], seat };



                //БИЛЕТЫ СОХРАНЯЕМ
                std::ofstream file("tickets.txt", std::ios::app);
                file << race.tickets[race.soldSeats].passengerName << " " << race.tickets[race.soldSeats].from.name << " " << race.tickets[race.soldSeats].destination.name << " " << race.tickets[race.soldSeats].seatNumber << " " << race.tickets[race.soldSeats].destination.price << "\n";
                file.close();

                //ИСТОРИЮ СОХРАНЯЕМ
                std::ofstream historyFile("history.txt", std::ios::app);
                historyFile << "Купив: " << race.tickets[race.soldSeats].from.name
                    << " Місто: " << race.tickets[race.soldSeats].destination.name
                    << " Місце: " << race.tickets[race.soldSeats].seatNumber
                    << " Ім'я: " << race.tickets[race.soldSeats].passengerName << "\n";
                historyFile.close();

                race.soldSeats++;
                //СПИСУЕМ ДЕНЬГИ)
                money -= cities[toIndex].price;
                //ВСЕ, ПОЛЬЗОВАТЕЛЬ ЗНАЕТ ЧТО У НЕГО ЕСТЬ БИЛЕТ
                std::cout << "Білет куплено! Місце " << seat << std::endl;


            }
        }
        else if (choice == 2) {
            std::ifstream infile("tickets.txt");
            if (!infile) {
                std::cout << "Файл квитків не знайдено.\n";
                continue;
            }

            std::cout << "Список квитків:\n";
            std::string line;
            int index = 0;
            while (std::getline(infile, line)) {
                std::cout << index << ") " << line << "\n";
                index++;
            }
            infile.close();

            if (index == 0) {
                std::cout << "Немає квитків для скасування.\n";
                continue;
            }

            int cancelIndex;
            std::cout << "Виберіть індекс квитка для скасування: ";
            std::cin >> cancelIndex;

            std::ifstream infile2("tickets.txt");
            std::ofstream outfile("temp.txt");
            int currentIndex = 0;

            std::string name, from, to;
            int price, seat;
            //ЭТО ТОЖЕ ЗАПИСУЕМ В ИСТОРИЮ
            while (infile2 >> name >> from >> to >> seat >> price) {
                //ЕСЛИ НЕ ТОТ БИЛЕТ
                if (currentIndex != cancelIndex) {
                    outfile << name << " " << from << " " << to << " " << seat << " " << price << "\n";
                }
                else {
                    std::ofstream historyFile("history.txt", std::ios::app);
                    historyFile << "Скасував: " << from
                        << " Місто: " << to
                        << " Місце: " << seat
                        << " Ім'я: " << name
                        << " (Повернуто " << price << " грн)\n";
                    historyFile.close();
                    // ВОЗВРАЩАЕМ ДЕНЬГИ(
                    money += price;
                    // СПИСУЕМ КОМИССИЮ)
                    money -= 10;
                    std::cout << "Білет скасовано! Повернуто " << price - 10 << " грн" << std::endl;
                }
                currentIndex++;
            }

            infile2.close();
            outfile.close();

            std::ifstream temp("temp.txt");
            std::ofstream tickets("tickets.txt");
            //ВОТ ТУТ ПЕРЕЗАПИСЬ С ВРЕМЕННОГО ФАЙЛА (КУДА МЫ ВСЕ ЭТО ЗАСОВЫВАЛИ НА ВРЕМЯ) В ПОСТОЯННЫЙ ОРИГИНАЛЬНЫЙ
            while (std::getline(temp, line)) {
                tickets << line << "\n";
            }

            temp.close();
            tickets.close();


        }
        //ПОКАЗ ИСТОРИИ (ФУНКЦИЯ)
        else if (choice == 3) {
            showHistory();
        }
        //ЕДИМ В ДРУГОЙ ГОРОД (ПОЧТИ ТА ЖЕ ОТМЕНА, ТОЛЬКО ЧУТЬ ЧУТЬ ДРУГОЙ ТЕКСТ)
        else if (choice == 4) {
            std::ifstream infile("tickets.txt");
            if (!infile) {
                std::cout << "У вас немає квитків.\n";
                continue;
            }

            std::cout << "Ваші квитки:\n";
            std::string line;
            int index = 0;
            while (std::getline(infile, line)) {
                std::cout << index << ") " << line << "\n";
                index++;
            }
            infile.close();

            if (index == 0) {
                std::cout << "Немає квитків для поїздки.\n";
                continue;
            }


            //ПО СУТИ ТОТ ЖЕ КОД ЧТО И ПРИ ОТМЕНЕ, НО МЕНЯЕМ ГОРОД И НЕ ВОЗВРАЩАЕМ ДЕНЬГИ
            int travelIndex;
            std::cout << "Виберіть індекс квитка для поїздки: ";
            std::cin >> travelIndex;

            std::ifstream infile2("tickets.txt");
            std::ofstream outfile("temp.txt");
            int currentIndex = 0;

            std::string name, from, to;
            int price, seat;

            while (infile2 >> name >> from >> to >> seat >> price) {
                if (currentIndex != travelIndex) {
                    outfile << name << " " << from << " " << to << " " << seat << " " << price << "\n";
                }
                else {
                    //МЕНЯЕМ НАШ ГОРОД
                    currentCity = { to, price };

                    //В ИСТОРИЮ, ОПЯТЬ ЖЕ, ТОЖЕ САМОЕ ЧТО И ПРИ ОТМЕНЕ, ТОЛЬКО ПОЕХАЛ, А НЕ ОТМЕНИЛ
                    std::ofstream historyFile("history.txt", std::ios::app);
                    historyFile << "Поїхав: " << from
                        << " Місто: " << to
                        << " Місце: " << seat
                        << " Ім'я: " << name << "\n";
                    historyFile.close();
                    std::cout << "Ви поїхали з " << from << " до " << to << std::endl;
                }
                currentIndex++;
            }

            infile2.close();
            outfile.close();

            std::ifstream temp("temp.txt");
            std::ofstream tickets("tickets.txt");
            //ТА ЖЕ САМАЯ ПЕРЕЗАПИСЬ С ВРЕМЕННОГО ФАЙЛА В ПОСТОЯННЫЙ
            while (std::getline(temp, line)) {
                tickets << line << "\n";
            }
            temp.close();
            tickets.close();

        }
        //ВОТ ТУТ ТЕКСТ К ГОРОДАМ СГЕНЕРИРОВАЛ CHAT GPT
        else if (choice == 5) {
            showDescription(currentCity);
        }

        else {
            break;
        }
    }

}


//ЗАМЕТКИ И НЬЮАНСЫ
//МОЖНО РЕСТАРТНУТЬ ПРОГУ И ОСТАТЬСЯ С КУПЛЕНЫМИ БИЛЕТАМИ. ЭТО ВСЕ СДЕЛАНО СПЕЦИАЛЬНО ЧТО БЫ ПРИ ОКОНЧАНИИ ДЕНЕГ НЕ ИСПЫТЫВАТЬ ТРУДНОСТИ ОТМЕНЯТЬ БИЛЕТЫ НА КАКОЙ ТО СТАНЦИИ
//ПРОЕКТ СДЕЛАН В Xcode (MacOS) ПО ЭТОМУ ВОЗМОЖНЫ РАСХОДСТВА