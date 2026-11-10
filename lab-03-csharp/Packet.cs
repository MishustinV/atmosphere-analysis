using System;
using System.Collections.Generic;
using System.Text;

namespace Atmosphere;

/// <summary>
/// Фрагмент потока, содержащий набор измерений.
/// Связь с Measurement - агрегация: измерения создаются снаружи и передаются готовыми.
/// </summary>
public class Packet
{
    private readonly List<Measurement> _measurements = new();

    public string Id { get; }
    public int MaxCapacity { get; }
    public int Count => _measurements.Count;

    public Packet(string id, int maxCapacity = 10)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(id);
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(maxCapacity);

        Id = id;
        MaxCapacity = maxCapacity;
    }

    /// <returns>false, если пакет переполнен.</returns>
    public bool AddMeasurement(Measurement measurement)
    {
        ArgumentNullException.ThrowIfNull(measurement);

        if (Count >= MaxCapacity)
        {
            return false;
        }

        _measurements.Add(measurement);
        return true;
    }

    public bool RemoveMeasurement(Measurement measurement)
    {
        return _measurements.Remove(measurement);
    }

    public override string ToString()
    {
        var sb = new StringBuilder();
        sb.Append($"Пакет {Id} содержит {Count} измерений:");
        foreach (Measurement m in _measurements)
        {
            sb.AppendLine();
            sb.Append(m);
        }
        return sb.ToString();
    }
}
