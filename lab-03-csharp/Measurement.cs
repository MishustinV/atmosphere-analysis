using System;

namespace Atmosphere;

/// <summary>Одно значение параметра атмосферы в точке с координатами и временем.</summary>
public class Measurement
{
    public double Value { get; }
    public double Latitude { get; }
    public double Longitude { get; }
    public string Timestamp { get; }
    public string Parameter { get; }

    public Measurement(double value, double latitude, double longitude,
                       string timestamp, string parameter)
    {
        if (double.IsNaN(value) || double.IsInfinity(value))
        {
            throw new ArgumentException(
                "Значение измерения не может быть NaN или бесконечностью.", nameof(value));
        }

        ArgumentException.ThrowIfNullOrWhiteSpace(timestamp);
        ArgumentException.ThrowIfNullOrWhiteSpace(parameter);

        Value = value;
        Latitude = latitude;
        Longitude = longitude;
        Timestamp = timestamp;
        Parameter = parameter;
    }

    public bool IsNormal => Parameter switch
    {
        "temperature" => Value >= -273.15 && Value <= 100.0,
        "pressure" => Value >= 0.0,
        "humidity" => Value >= 0.0 && Value <= 100.0,
        _ => true
    };

    public override string ToString()
    {
        return $"Измерение: {Parameter} = {Value} в ({Latitude}, {Longitude}) " +
               $"время: {Timestamp} [{(IsNormal ? "норма" : "аномалия")}]";
    }
}
