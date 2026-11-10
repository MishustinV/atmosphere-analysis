using System;
using System.Collections.Generic;
using System.Text;

namespace Atmosphere;

/// <summary>
/// Последовательность пакетов от спутника.
/// Связь с Packet - композиция: поток сам создаёт пакеты и наружу их не отдаёт.
/// </summary>
public class DataStream
{
    private readonly List<Packet> _packets = new();

    public string SatelliteName { get; }
    public int MaxPackets { get; }
    public int Count => _packets.Count;

    /// <summary>Поток при создании сам создаёт свой первый пакет.</summary>
    public DataStream(string satelliteName, string firstPacketId, int maxPackets = 5)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(satelliteName);
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(maxPackets);

        SatelliteName = satelliteName;
        MaxPackets = maxPackets;
        AddPacket(firstPacketId);
    }

    /// <summary>Создаёт новый пакет внутри потока.</summary>
    /// <returns>false, если поток переполнен.</returns>
    public bool AddPacket(string packetId, int maxCapacity = 10)
    {
        if (Count >= MaxPackets)
        {
            return false;
        }

        _packets.Add(new Packet(packetId, maxCapacity));
        return true;
    }

    /// <summary>Кладёт готовое измерение в пакет потока по идентификатору пакета.</summary>
    /// <returns>false, если пакета нет или он переполнен.</returns>
    public bool AddMeasurement(string packetId, Measurement measurement)
    {
        Packet? packet = _packets.Find(p => p.Id == packetId);
        return packet != null && packet.AddMeasurement(measurement);
    }

    public override string ToString()
    {
        var sb = new StringBuilder();
        sb.Append($"Поток спутника {SatelliteName} содержит {Count} пакетов");
        foreach (Packet p in _packets)
        {
            sb.AppendLine();
            sb.Append(p);
        }
        return sb.ToString();
    }
}
