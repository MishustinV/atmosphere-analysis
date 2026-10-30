#include "Measurement.hpp"
#include "Packet.hpp"
#include "DataStream.hpp"

#include <iostream>

int main()
{
    std::cout << "===== Создание измерений (статическая инициализация) =====" << std::endl;
    Measurement m1(25.5, 55.75, 37.62, "2026-10-15T10:00", "temperature");
    Measurement m2(1013.2, 55.75, 37.62, "2026-10-15T10:00", "pressure");
    Measurement m3(60.0, 55.75, 37.62, "2026-10-15T10:00", "humidity");

    std::cout << "\n===== Динамическая инициализация =====" << std::endl;
    Measurement* m4 = new Measurement(15.0, 55.75, 37.62, "2026-10-15T10:01", "temperature");

    std::cout << "\n===== Создание пакета (агрегация) =====" << std::endl;
    {
        Packet packet("P001", 10);
        packet.AddMeasurement(&m1);
        packet.AddMeasurement(&m2);
        packet.AddMeasurement(&m3);
        packet.AddMeasurement(m4);

        std::cout << "\n===== Попытка нарушить правило (переполнение) =====" << std::endl;
        Measurement extra(0.0, 0.0, 0.0, "2026-10-15T10:02", "temperature");
        for (int i = 0; i < 10; ++i)
        {
            packet.AddMeasurement(&extra);
        }

        std::cout << "\n===== Состояние пакета =====" << std::endl;
        packet.Print();

        std::cout << "\n===== Уничтожение пакета =====" << std::endl;
    }
    std::cout << "Пакет уничтожен. Проверяем, что измерения живы:" << std::endl;
    m1.Print();
    m4->Print();

    std::cout << "\n===== Композиция: DataStream содержит Packet =====" << std::endl;
    {
        DataStream stream("NOAA-20", 3);
        Packet p("P002", 5);
        p.AddMeasurement(&m2);
        stream.AddPacket(p);

        std::cout << "\n===== Состояние потока =====" << std::endl;
        stream.Print();

        std::cout << "\n===== Уничтожение потока =====" << std::endl;
    }
    std::cout << "Поток уничтожен. Измерение m2 всё ещё живо:" << std::endl;
    m2.Print();

    std::cout << "\n===== Динамический массив объектов класса =====" << std::endl;
    Measurement* arr = new Measurement[2]{
        Measurement(10.0, 0.0, 0.0, "2026-10-15T11:00", "temperature"),
        Measurement(20.0, 0.0, 0.0, "2026-10-15T11:01", "temperature")
    };
    delete[] arr;

    std::cout << "\n===== Массив динамических объектов =====" << std::endl;
    Measurement* dynArr[2];
    dynArr[0] = new Measurement(30.0, 0.0, 0.0, "2026-10-15T12:00", "temperature");
    dynArr[1] = new Measurement(40.0, 0.0, 0.0, "2026-10-15T12:01", "temperature");
    delete dynArr[0];
    delete dynArr[1];

    std::cout << "\n===== Удаление динамического измерения =====" << std::endl;
    delete m4;

    std::cout << "\n===== Конец main =====" << std::endl;
    return 0;
}
