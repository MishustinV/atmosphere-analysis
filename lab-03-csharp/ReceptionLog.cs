using System;

namespace Atmosphere;

/// <summary>Журнал приёма пакетов. Освобождается через IDisposable и using.</summary>
public sealed class ReceptionLog : IDisposable
{
    private readonly string _title;

    public ReceptionLog(string title)
    {
        _title = title;
        Console.WriteLine($"[+] журнал открыт: {_title}");
    }

    public void Write(string line) => Console.WriteLine($"    {line}");

    public void Dispose() => Console.WriteLine($"[-] журнал закрыт: {_title}");
}
