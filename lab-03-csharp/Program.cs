using System;
using System.Globalization;
using System.Runtime.CompilerServices;
using System.Text;

namespace Atmosphere;

internal static class Program
{
    private static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8;
        CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

        DemoCompositionAndAggregation();
        DemoRule();
        DemoReferencesAndCopies();
        DemoDisposable();
        DemoGarbageCollector();
    }

    private static Measurement CreateMeasurement(double value, string parameter, string time)
    {
        return new Measurement(value, 55.75, 37.62, time, parameter);
    }

    private static void DemoCompositionAndAggregation()
    {
        Console.WriteLine("===== Композиция: DataStream сам создаёт Packet =====");
        var stream = new DataStream("NOAA-20", "P001", 3);
        stream.AddPacket("P002");

        Console.WriteLine("\n===== Агрегация: измерения создаются снаружи =====");
        Measurement m1 = CreateMeasurement(25.5, "temperature", "2026-10-15T10:00");
        Measurement m2 = CreateMeasurement(1013.2, "pressure", "2026-10-15T10:00");
        Measurement m3 = CreateMeasurement(60.0, "humidity", "2026-10-15T10:00");

        stream.AddMeasurement("P001", m1);
        stream.AddMeasurement("P001", m2);
        stream.AddMeasurement("P002", m2);
        Console.WriteLine(stream);

        Console.WriteLine("\n===== Отдельный пакет, созданный снаружи =====");
        var packet = new Packet("P100", 5);
        packet.AddMeasurement(m1);
        packet.AddMeasurement(m3);
        Console.WriteLine(packet);

        Console.WriteLine("\n===== Удаляем m1 из пакета - сама m1 жива =====");
        packet.RemoveMeasurement(m1);
        Console.WriteLine($"В пакете P100 осталось: {packet.Count}");
        Console.WriteLine(m1);
    }

    private static void DemoRule()
    {
        Console.WriteLine("\n===== Проверка правил =====");

        try
        {
            _ = CreateMeasurement(double.NaN, "temperature", "2026-10-15T10:02");
        }
        catch (ArgumentException ex)
        {
            Console.WriteLine($"Ошибка: {ex.Message}");
        }

        Measurement extra = CreateMeasurement(0.0, "temperature", "2026-10-15T10:02");
        var packet = new Packet("P003", 10);
        for (int i = 1; i <= 12; ++i)
        {
            bool added = packet.AddMeasurement(extra);
            if (!added)
            {
                Console.WriteLine($"Попытка {i}: пакет {packet.Id} переполнен (максимум {packet.MaxCapacity})");
            }
        }
        Console.WriteLine($"В пакете {packet.Id}: {packet.Count} измерений");

        var stream = new DataStream("MetOp-B", "A1", 2);
        stream.AddPacket("A2");
        bool third = stream.AddPacket("A3");
        Console.WriteLine($"Третий пакет в поток на 2 пакета добавлен: {third}");
    }

    private static void DemoReferencesAndCopies()
    {
        Console.WriteLine("\n===== Ссылки и копии: класс =====");
        Measurement m = CreateMeasurement(15.0, "temperature", "2026-10-15T10:01");

        var first = new Packet("P200");
        Packet second = first;
        second.AddMeasurement(m);

        Console.WriteLine($"first.Count  = {first.Count}");
        Console.WriteLine($"second.Count = {second.Count}");
        Console.WriteLine($"first == second: {first == second}");

        Console.WriteLine("\n===== Ссылки и копии: структура =====");
        var a = new GeoPoint { Latitude = 55.75, Longitude = 37.62 };
        GeoPoint b = a;
        b.Latitude = 10.0;

        Console.WriteLine($"a = {a}");
        Console.WriteLine($"b = {b}");
    }

    private static void DemoDisposable()
    {
        Console.WriteLine("\n===== Освобождение ресурса: IDisposable и using =====");
        using (var log = new ReceptionLog("Приём NOAA-20"))
        {
            log.Write("принят пакет P001");
            log.Write("принят пакет P002");
        }
        Console.WriteLine("Вышли из блока using");
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static WeakReference<Measurement> CreateTemporary()
    {
        var temp = new Measurement(0.0, 0.0, 0.0, "2026-10-15T12:00", "temperature");
        return new WeakReference<Measurement>(temp);
    }

    private static void DemoGarbageCollector()
    {
        Console.WriteLine("\n===== Сборщик мусора =====");
        WeakReference<Measurement> watcher = CreateTemporary();
        Console.WriteLine($"Вышли из метода, ссылок на объект нет. Жив: {watcher.TryGetTarget(out _)}");

        GC.Collect();
        GC.WaitForPendingFinalizers();
        Console.WriteLine($"После GC.Collect(). Жив: {watcher.TryGetTarget(out _)}");
    }
}
