
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
    int soldSeats = 0;
};

int main() {
    srand(time(0));
    
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
            if (from != to){
                races[from][to].from = cities[from];
                races[from][to].to = cities[to];
            }
        }
    }
    
    //РАНДОМНО ЗАПОЛНЯЮ МЕСТА
    for (int from = 0; from < size; from++) {
        for (int to = 0; to < size; to++) {
            if (from == to) continue;

            Race &race = races[from][to];
            int passengers = rand() % 9;

            for (int i = 0; i < passengers; i++) {
                int seat = race.soldSeats + 1;
                race.tickets[race.soldSeats] = {"Пасажир", cities[from], cities[to], seat};
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
            
            for(int i = 0; i < size; i++){
                if(cities[i].name == currentCity.name){
                    fromIndex = i;
                    break;
                }
            }
            
            
            for (int i = 0; i < size; i++) {
                if (currentCity.name != cities[i].name){
                    std::cout << index << ") " << currentCity.name << " - " << cities[i].name
                    << " Вільних місць: " << (10 - races[fromIndex][i].soldSeats)<<"| "<<races[fromIndex][i].to.price<<" грн" << std::endl;
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
            
            Race &race = races[fromIndex][toIndex]; //Эта ссылка на массив нужна чисто что бы был код чище
            int choice3;
            std::cout<<"1 - Купити 2 - Подивитись список пасажирів ";
            std::cin>>choice3;
            if (choice3 == 2) {
                std::cout << "Пасажири на рейс " << currentCity.name << " - " << cities[toIndex].name << ":\n";
                if (race.soldSeats == 0) {
                    std::cout << "Немає пасажирів"<<std::endl;
                }
                else {
                    for (int i = 0; i < race.soldSeats; i++) {
                        std::cout << race.tickets[i].seatNumber << " - " << race.tickets[i].passengerName << std::endl;
                    }
                }
            }
            else if(choice3 == 1){
                if (race.soldSeats >= 10) {
                    std::cout << "Немає місць" <<std::endl;
                    continue;
                }
                
                if (money < race.to.price){
                    std::cout<<"Недостатньо грошей"<<std::endl;
                    continue;
                }
                
                std::cout << "Введіть своє ім'я: ";
                std::string name;
                std::cin >> name;
                
                int seat = race.soldSeats + 1; //1 2 3 4 5 6 7 8 9 10
                race.tickets[race.soldSeats] = {name, currentCity, cities[toIndex], seat};
                
                
                
                //БИЛЕТЫ
                std::ofstream file("tickets.txt", std::ios::app);
                file << race.tickets[race.soldSeats].passengerName << " " << race.tickets[race.soldSeats].from.name << " " << race.tickets[race.soldSeats].destination.name << " " << race.tickets[race.soldSeats].seatNumber << " " << race.tickets[race.soldSeats].destination.price << "\n";
                file.close();
                
                //ИСТОРИЯ
                std::ofstream historyFile("history.txt", std::ios::app);
                historyFile << "Купив: " << race.tickets[race.soldSeats].from.name
                            << " Місто: " << race.tickets[race.soldSeats].destination.name
                            << " Місце: " << race.tickets[race.soldSeats].seatNumber
                            << " Ім'я: " << race.tickets[race.soldSeats].passengerName << "\n";
                historyFile.close();
                
                race.soldSeats++;
                //СПИСУЕМ ДЕНЬГИ
                money -= cities[toIndex].price;
                std::cout << "Білет куплено! Місце " << seat << std::endl;
                
                
            }
        }
        else if(choice == 2){
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
            while (infile2 >> name >> from >> to >> seat >> price) {
                if (currentIndex != cancelIndex) {
                    outfile << name << " " << from << " " << to << " " << seat << " " << price << "\n";
                } else {
                    std::ofstream historyFile("history.txt", std::ios::app);
                    historyFile << "Скасував: " << from
                    << " Місто: " << to
                    << " Місце: " << seat
                    << " Ім'я: " << name
                    << " (Повернуто " << price << " грн)\n";
                    historyFile.close();
                    money += price; // возвращаем деньги
                    std::cout << "Білет скасовано! Повернуто " << price << " грн\n";
                }
                currentIndex++;
            }
            
            infile2.close();
            outfile.close();
            
            std::ifstream temp("temp.txt");
            std::ofstream tickets("tickets.txt");
            
            while (std::getline(temp, line)) {
                tickets << line << "\n";
            }
            
            temp.close();
            tickets.close();
            
            
        }
        else if (choice == 3) {
            std::ifstream history("history.txt");
            if (!history) {
                std::cout << "Історія порожня.\n";
            }
            else {
                std::cout << "Історія квитків:\n";
                std::string line;
                while (std::getline(history, line)) {
                    std::cout << line << std::endl;
                }
            }
            history.close();
        }
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
                } else {
                    currentCity = {to, price};
                    
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
            while (std::getline(temp, line)) {
                tickets << line << "\n";
            }
            temp.close();
            tickets.close();
            
        }
        else if (choice == 5) {
            std::cout << "Інформація про місто " << currentCity.name << ":\n";
            if (currentCity.name == "Київ") {
                std::cout << "Київ — столиця України, великий культурний та економічний центр.\n";
            } else if (currentCity.name == "Львів") {
                std::cout << "Львів — місто з багатою історією та красивою архітектурою, відоме кавою.\n";
            } else if (currentCity.name == "Одеса") {
                std::cout << "Одеса — портове місто на Чорному морі, відоме пляжами та Приморським бульваром.\n";
            } else if (currentCity.name == "Дніпро") {
                std::cout << "Дніпро — важливий промисловий та транспортний центр на сході України.\n";
            } else if (currentCity.name == "Запоріжжя") {
                std::cout << "Запоріжжя — місто з історією козацтва та відомою ГЕС на Дніпрі.\n";
            } else {
                std::cout << "Про це місто немає інформації.\n";
            }
        }

        else {
            break;
        }
    }
    
}


//ЗАМЕТКИ И НЬЮАНСЫ
//МОЖНО РЕСТАРТНУТЬ ПРОГУ И ОСТАТЬСЯ С КУПЛЕНЫМИ БИЛЕТАМИ. ЭТО ВСЕ СДЕЛАНО СПЕЦИАЛЬНО ЧТО БЫ ПРИ ОКОНЧАНИИ ДЕНЕГ НЕ ИСПЫТЫВАТЬ ТРУДНОСТИ ОТМЕНЯТЬ БИЛЕТЫ НА КАКОЙ ТО СТАНЦИИ
//ПРОЕКТ СДЕЛАН В Xcode (MacOS) ПО ЭТОМУ ВОЗМОЖНЫ РАСХОДСТВА
