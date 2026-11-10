#include "Measurement.hpp"
#include "Packet.hpp"
#include "DataStream.hpp"

#include <iostream>
#include <memory>
#include <utility>

int main()
{
    std::cout << "===== shared_ptr: создание измерений (владелец - main) =====" << std::endl;
    std::shared_ptr<Measurement> m1 = std::make_shared<Measurement>(25.5, 55.75, 37.62, "2026-10-15T10:00", "temperature");
    std::shared_ptr<Measurement> m2 = std::make_shared<Measurement>(1013.2, 55.75, 37.62, "2026-10-15T10:00", "pressure");
    std::shared_ptr<Measurement> m3 = std::make_shared<Measurement>(60.0, 55.75, 37.62, "2026-10-15T10:00", "humidity");
    std::shared_ptr<Measurement> m4 = std::make_shared<Measurement>(15.0, 55.75, 37.62, "2026-10-15T10:01", "temperature");

    std::cout << "\n===== Создание пакета (агрегация через weak_ptr) =====" << std::endl;
    {
        Packet packet("P001", 10);
        packet.AddMeasurement(m1);
        packet.AddMeasurement(m2);
        packet.AddMeasurement(m3);
        packet.AddMeasurement(m4);
        std::cout << "После добавления в пакет m1.use_count() = " << m1.use_count()
                  << " (пакет не владеет измерениями)" << std::endl;

        std::cout << "\n===== Попытка нарушить правило (переполнение) =====" << std::endl;
        std::shared_ptr<Measurement> extra = std::make_shared<Measurement>(0.0, 0.0, 0.0, "2026-10-15T10:02", "temperature");
        for (int i = 0; i < 10; ++i)
        {
            packet.AddMeasurement(extra);
        }

        std::cout << "\n===== Состояние пакета =====" << std::endl;
        packet.Print();

        std::cout << "\n===== Уничтожение пакета =====" << std::endl;
    }
    std::cout << "Пакет уничтожен. Проверяем, что измерения живы:" << std::endl;
    m1->Print();
    m4->Print();

    std::cout << "\n===== Композиция: DataStream содержит Packet =====" << std::endl;
    {
        DataStream stream("NOAA-20", 3);
        Packet p("P002", 5);
        p.AddMeasurement(m2);
        stream.AddPacket(p);

        std::cout << "\n===== Состояние потока =====" << std::endl;
        stream.Print();

        std::cout << "\n===== Уничтожение потока =====" << std::endl;
    }
    std::cout << "Поток уничтожен. Измерение m2 всё ещё живо:" << std::endl;
    m2->Print();

    std::cout << "\n===== unique_ptr: один владелец =====" << std::endl;
    {
        std::unique_ptr<Measurement> first = std::make_unique<Measurement>(5.0, 0.0, 0.0, "2026-10-15T13:00", "temperature");
        first->Print();

        std::unique_ptr<Measurement> second = std::move(first);
        if (!first)
        {
            std::cout << "first пуст: владение передано в second" << std::endl;
        }
        second->Print();

        std::cout << "Выходим из блока" << std::endl;
    }
    std::cout << "Блок закончился: объект уничтожен автоматически" << std::endl;

    std::cout << "\n===== unique_ptr: массив объектов класса =====" << std::endl;
    {
        std::unique_ptr<Measurement[]> arr = std::make_unique<Measurement[]>(2);
        arr[0] = Measurement(10.0, 0.0, 0.0, "2026-10-15T11:00", "temperature");
        arr[1] = Measurement(20.0, 0.0, 0.0, "2026-10-15T11:01", "temperature");
        std::cout << "Выходим из блока" << std::endl;
    }

    std::cout << "\n===== unique_ptr: массив умных указателей =====" << std::endl;
    {
        std::unique_ptr<Measurement> dynArr[2];
        dynArr[0] = std::make_unique<Measurement>(30.0, 0.0, 0.0, "2026-10-15T12:00", "temperature");
        dynArr[1] = std::make_unique<Measurement>(40.0, 0.0, 0.0, "2026-10-15T12:01", "temperature");
        std::cout << "Выходим из блока" << std::endl;
    }

    std::cout << "\n===== shared_ptr: счётчик владельцев =====" << std::endl;
    std::cout << "m2.use_count() = " << m2.use_count() << std::endl;
    {
        std::shared_ptr<Measurement> owner2 = m2;
        std::cout << "Копия owner2: use_count() = " << m2.use_count() << std::endl;
        {
            std::shared_ptr<Measurement> owner3 = owner2;
            std::cout << "Копия owner3: use_count() = " << m2.use_count() << std::endl;
        }
        std::cout << "owner3 уничтожен: use_count() = " << m2.use_count() << std::endl;
    }
    std::cout << "owner2 уничтожен: use_count() = " << m2.use_count() << std::endl;

    std::cout << "\n===== shared_ptr: объект уничтожается с последним владельцем =====" << std::endl;
    {
        std::shared_ptr<Measurement> a = std::make_shared<Measurement>(70.0, 0.0, 0.0, "2026-10-15T14:00", "humidity");
        std::shared_ptr<Measurement> b = a;
        std::cout << "use_count() = " << a.use_count() << std::endl;
        a.reset();
        std::cout << "a.reset(): объект жив, use_count() = " << b.use_count() << std::endl;
        b.reset();
        std::cout << "b.reset(): последний владелец ушёл" << std::endl;
    }

    std::cout << "\n===== weak_ptr: наблюдатель =====" << std::endl;
    {
        std::shared_ptr<Measurement> owner = std::make_shared<Measurement>(99.0, 0.0, 0.0, "2026-10-15T15:00", "temperature");
        std::weak_ptr<Measurement> watcher = owner;
        std::cout << "weak_ptr не владеет: use_count() = " << owner.use_count() << std::endl;

        if (std::shared_ptr<Measurement> alive = watcher.lock())
        {
            std::cout << "lock(): объект жив, use_count() = " << alive.use_count() << std::endl;
            alive->Print();
        }

        owner.reset();
        if (std::shared_ptr<Measurement> alive = watcher.lock())
        {
            alive->Print();
        }
        else
        {
            std::cout << "lock() вернул пустой указатель: объект уже уничтожен" << std::endl;
        }
    }

    std::cout << "\n===== weak_ptr в Packet: измерение уничтожено раньше пакета =====" << std::endl;
    {
        Packet observer("P003", 5);
        observer.AddMeasurement(m3);
        observer.AddMeasurement(m4);
        observer.Print();

        std::cout << "\nУдаляем владельца m3" << std::endl;
        m3.reset();
        observer.Print();
    }

    std::cout << "\n===== Освобождение m4 (reset вместо ручного освобождения) =====" << std::endl;
    m4.reset();

    std::cout << "\n===== Конец main =====" << std::endl;
    return 0;
}
